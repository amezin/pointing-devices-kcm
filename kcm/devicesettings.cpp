#include "devicesettings.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <KConfig>

#include "log.h"

namespace {

QStringList subtract(const QStringList &from, const QStringList &what)
{
    QStringList result;
    result.reserve(from.size());
    Q_FOREACH (const QString &s, from) {
        if (!what.contains(s)) {
            result.append(s);
        }
    }
    return result;
}

template<typename T>
bool fuzzySettingsCompare(T a, T b)
{
    if (qFuzzyIsNull(a) && qFuzzyIsNull(b)) {
        return true;
    }
    return qFuzzyCompare(a, b);
}

bool isFloatType(const QVariant &v)
{
    auto t = static_cast<QMetaType::Type>(v.type());
    return t == QMetaType::Float || t == QMetaType::Double;
}

bool compareSettings(const QVariant &a, const QVariant &b)
{
    if (isFloatType(a) || isFloatType(b)) {
        float d_a = a.toFloat();
        float d_b = b.toFloat();
        if (qFuzzyIsNull(d_a) && qFuzzyIsNull(d_b)) {
            return true;
        }
        if (qFuzzyIsNull(d_a) || qFuzzyIsNull(d_b)) {
            d_a += 1.0f;
            d_b += 1.0f;
        }
        return qFuzzyCompare(d_a, d_b);
    }
    return a == b;
}

}

DeviceSettings::DeviceSettings(KConfig *config,
                               KConfig *defaults,
                               InputDevice *device,
                               QObject *parent)
    : QQmlPropertyMap(this, parent),
      device_(device),
      configGroup_(device ? config->group(device->identifier()) : KConfigGroup()),
      defaultsGroup_(device ? defaults->group(device->identifier()) : KConfigGroup())
{
    if (!device) {
        return;
    }

    connect(device, &QObject::destroyed, this, [this](QObject *) {
        Q_EMIT deviceDestroyed();
    });

    connect(device, &InputDevice::supportedPropertiesChanged,
            this, &DeviceSettings::updatePropertySet);
    connect(device, &InputDevice::supportedPropertiesChanged,
            this, &DeviceSettings::supportedPropertiesChanged);
    connect(this, &QQmlPropertyMap::valueChanged,
            this, &DeviceSettings::handlePropertyChange);

    updatePropertySet();
}

DeviceSettings::~DeviceSettings()
{
}

QStringList DeviceSettings::supportedProperties() const
{
    if (!device_) {
        return QStringList();
    }
    return device_->supportedProperties();
}

QVariant DeviceSettings::activeValue(const QString &name) const
{
    if (!device_) {
        return QVariant();
    }
    return device_->property(qPrintable(name));
}

QVariant DeviceSettings::savedValue(const QString &name) const
{
    return configGroup_.readEntry(name, defaultValue(name));
}

QVariant DeviceSettings::defaultValue(const QString &name) const
{
    return defaultsGroup_.readEntry(name, activeValue(name));
}

void DeviceSettings::load(DeviceSettings::Getter getter, const QStringList &keys, bool overwrite)
{
    QStringList loadedKeys;
    Q_FOREACH(const QString &key, keys) {
        if (!overwrite && contains(key)) {
            continue;
        }
        insert(key, (this->*getter)(key));
        loadedKeys.append(key);
    }
    updateDiffs(loadedKeys);
}

void DeviceSettings::load(Getter getter, bool overwrite)
{
    load(getter, supportedProperties(), overwrite);
}

QStringList DeviceSettings::differsFrom(Getter getter, const QStringList &keys) const
{
    QStringList diff;
    Q_FOREACH(const QString &key, keys) {
        if (!compareSettings((this->*getter)(key), value(key))) {
            diff.append(key);
        }
    }
    return diff;
}

InputDevice *DeviceSettings::device() const
{
    return device_.data();
}

bool DeviceSettings::needsSave() const
{
    return !needsSave_.isEmpty();
}

bool DeviceSettings::differsFromDefaults() const
{
    return !differsFromDefaults_.isEmpty();
}

bool DeviceSettings::differsFromActive() const
{
    return !differsFromActive_.isEmpty();
}

void DeviceSettings::setNeedsSave(const QStringList &value)
{
    bool prev = needsSave_.isEmpty();
    needsSave_ = value;
    if (prev != needsSave_.isEmpty()) {
        Q_EMIT needsSaveChanged();
    }
}

void DeviceSettings::setDiffersFromActive(const QStringList &value)
{
    bool prev = differsFromActive_.isEmpty();
    differsFromActive_ = value;
    if (prev != differsFromActive_.isEmpty()) {
        Q_EMIT differsFromActiveChanged();
    }
}

void DeviceSettings::setDiffersFromDefaults(const QStringList &value)
{
    bool prev = differsFromDefaults_.isEmpty();
    differsFromDefaults_ = value;
    if (prev != differsFromDefaults_.isEmpty()) {
        Q_EMIT differsFromDefaultsChanged();
    }
}

void DeviceSettings::updatePropertySet()
{
    load(&DeviceSettings::savedValue, false);
    clearUnsupportedProperties();
}

void DeviceSettings::clearUnsupportedProperties()
{
    auto supported = supportedProperties();
    Q_FOREACH (const auto &key, keys()) {
        if (!supported.contains(key)) {
            clear(key);
        }
    }
}

void DeviceSettings::updateDiffs(const QStringList &keys, bool discardOld)
{
    QStringList save, active, defaults;
    if (!discardOld) {
        save = subtract(needsSave_, keys);
        active = subtract(differsFromActive_, keys);
        defaults = subtract(differsFromDefaults_, keys);
    }

    save.append(differsFrom(&DeviceSettings::savedValue, keys));
    defaults.append(differsFrom(&DeviceSettings::defaultValue, keys));

    Q_FOREACH (const QString &key, keys) {
        if (!compareSettings(savedValue(key), activeValue(key))) {
            active.append(key);
        }
    }

    setNeedsSave(save);
    setDiffersFromActive(active);
    setDiffersFromDefaults(defaults);
}

void DeviceSettings::handlePropertyChange(const QString &name, const QVariant &)
{
    return updateDiffs(QStringList(name));
}

void DeviceSettings::loadDefaults()
{
    load(&DeviceSettings::defaultValue);
}

void DeviceSettings::loadSaved()
{
    load(&DeviceSettings::savedValue);
}

void DeviceSettings::loadActive()
{
    load(&DeviceSettings::activeValue);
}

void DeviceSettings::apply(bool updateDiff)
{
    Q_FOREACH (const QString &key, supportedProperties()) {
        if (contains(key)) {
            device_->setProperty(qPrintable(key), value(key));
        }
    }
    if (updateDiff) {
        updateDiffs(supportedProperties(), true);
    }
}

void DeviceSettings::save(bool updateDiff)
{
    auto supported = supportedProperties();
    Q_FOREACH (const QString &key, supported) {
        if (value(key) == defaultValue(key)) {
            configGroup_.revertToDefault(key);
        } else {
            configGroup_.writeEntry(key, value(key));
        }
    }
    Q_FOREACH (const QString &key, configGroup_.keyList()) {
        if (!supported.contains(key)) {
            configGroup_.deleteEntry(key);
        }
    }
    configGroup_.sync();

    if (!updateDiff) {
        return;
    }

    updateDiffs(supported, true);
    if (needsSave()) {
        Q_FOREACH (const QString &key, needsSave_) {
            qWarning(POINTINGDEVICES) << "Values not saved:" << key
                                      << value(key) << savedValue(key);
        }
    }
}

QVariant DeviceSettings::updateValue(const QString &key, const QVariant &input)
{
    if (!device_) {
        return input;
    }
    int index = device_->metaObject()->indexOfProperty(qPrintable(key));
    if (index < 0) {
        return input;
    }
    QMetaProperty prop = device_->metaObject()->property(index);
    if (input.type() == prop.type()) {
        return input;
    }
    QVariant converted(input);
    converted.convert(prop.type());
    return converted;
}
