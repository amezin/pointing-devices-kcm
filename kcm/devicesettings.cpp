#include "devicesettings.h"

#include <KConfig>

#include "log.h"

namespace
{

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
    : QObject(parent),
      device_(device),
      configGroup_(device ? config->group(device->identifier()) : KConfigGroup()),
      defaultsGroup_(device ? defaults->group(device->identifier()) : KConfigGroup()),
      needsSave_(false), differsFromActive_(false), savedDiffersFromActive_(false)
{
    if (!device) {
        return;
    }

    connect(device, &InputDevice::propertyAdded,
            this, &DeviceSettings::addProperty);
    connect(device, &InputDevice::propertyRemoved,
            this, &DeviceSettings::removeProperty);
    connect(device, &InputDevice::propertyChanged,
            this, &DeviceSettings::updateProperty);

    connect(device, &QObject::destroyed,
            this, &DeviceSettings::deviceDisconnected);
    connect(device, &QObject::destroyed,
            this, &DeviceSettings::changed);

    Q_FOREACH (const QString &prop, device->supportedProperties()) {
        addProperty(prop);
    }

    connect(this, &DeviceSettings::changed,
            this, &DeviceSettings::updateStatus);
    updateStatus();
}

DeviceSettings::~DeviceSettings()
{
}

InputDevice *DeviceSettings::device() const
{
    return device_;
}

bool DeviceSettings::isPropertySupported(const QString &prop) const
{
    return device_ && readonly_.contains(prop);
}

bool DeviceSettings::isPropertyWritable(const QString &prop) const
{
    return device_ && values_.contains(prop);
}

bool DeviceSettings::needsSave() const
{
    return needsSave_;
}

bool DeviceSettings::differsFromActive() const
{
    return differsFromActive_;
}

bool DeviceSettings::savedDiffersFromActive() const
{
    return savedDiffersFromActive_;
}

QVariant DeviceSettings::value(const QString &prop) const
{
    return values_.value(prop, readonly_.value(prop, QVariant()));
}

void DeviceSettings::setValue(const QString &prop, const QVariant &value)
{
    if (setValueNoSignal(prop, value)) {
        Q_EMIT changed();
    }
}

bool DeviceSettings::setValueNoSignal(const QString &prop, const QVariant &value)
{
    if (!values_.contains(prop)) {
        return false;
    }
    auto fixed = fixupType(value, deviceValue(prop));
    if (values_.value(prop) == fixed) {
        return false;
    }
    values_.insert(prop, fixed);
    return true;
}

void DeviceSettings::addProperty(const QString &name)
{
    if (!device_) {
        return;
    }

    auto dev = device_->deviceProperty(name);
    readonly_.insert(name, dev);
    if (device_->isPropertyWritable(name)) {
        values_.insert(name, fixupType(savedValue(name), dev));
    }
    Q_EMIT changed();
}

void DeviceSettings::removeProperty(const QString &name)
{
    if (values_.remove(name) || readonly_.remove(name)) {
        Q_EMIT changed();
    }
}

void DeviceSettings::updateProperty(const QString &name)
{
    if (!device_) {
        return;
    }

    auto writable = device_->isPropertyWritable(name);
    if (!writable) {
        values_.remove(name);
    }
    auto dev = device_->deviceProperty(name);
    readonly_.insert(name, dev);
    if (writable) {
        values_.insert(name, fixupType(values_.value(name, savedValue(name)), dev));
    }
    Q_EMIT changed();
}

QVariant DeviceSettings::savedValue(const QString &name) const
{
    return configGroup_.readEntry(name, defaultValue(name));
}

QVariant DeviceSettings::defaultValue(const QString &name) const
{
    if (device_) {
        auto v = device_->defaultValue(name);
        if (v.isValid()) {
            return v;
        }
    }
    return defaultsGroup_.readEntry(name, deviceValue(name));
}

QVariant DeviceSettings::deviceValue(const QString &name) const
{
    return readonly_.value(name, QVariant());
}

QVariant DeviceSettings::fixupType(const QVariant &value, const QVariant &pattern)
{
    if (!value.isValid()) {
        return value;
    }

    QVariant converted(value);
    converted.convert(pattern.userType());

    if (converted.userType() != QMetaType::QVariantList &&
            converted.userType() != QMetaType::QStringList)
    {
        return converted;
    }

    QVariantList list(converted.toList());
    QVariantList listPattern(pattern.toList());

    QVariantList listConverted;
    listConverted.reserve(listPattern.size());
    for (int i = 0; i < listPattern.size(); i++) {
        listConverted.append(fixupType(list.value(i), listPattern.at(i)));
    }

    return listConverted;
}

void DeviceSettings::loadSaved()
{
    Q_FOREACH (const QString &prop, values_.keys()) {
        setValueNoSignal(prop, savedValue(prop));
    }
    Q_EMIT changed();
}

void DeviceSettings::loadDefaults()
{
    Q_FOREACH (const QString &prop, values_.keys()) {
        setValueNoSignal(prop, defaultValue(prop));
    }
    Q_EMIT changed();
}

void DeviceSettings::loadActive()
{
    Q_FOREACH (const QString &prop, values_.keys()) {
        setValueNoSignal(prop, deviceValue(prop));
    }
    Q_EMIT changed();
}

void DeviceSettings::apply()
{
    if (!device_) {
        return;
    }

    device_->setProperties(values_);
    updateStatus();
}

void DeviceSettings::applySaved()
{
    QVariantHash changes;
    Q_FOREACH (const QString &prop, values_.keys()) {
        auto current = deviceValue(prop);
        auto fixed = fixupType(savedValue(prop), current);
        if (current != fixed) {
            changes.insert(prop, fixed);
        }
    }
    device_->setProperties(changes);
}

void DeviceSettings::save()
{
    Q_FOREACH (const QString &prop, configGroup_.keyList()) {
        if (!isPropertyWritable(prop)) {
            configGroup_.deleteEntry(prop);
        }
    }
    Q_FOREACH (const QString &prop, values_.keys()) {
        auto v = values_.value(prop);
        if (defaultsGroup_.hasKey(prop) && compareSettings(v, defaultValue(prop))) {
            configGroup_.revertToDefault(prop);
        } else {
            configGroup_.writeEntry(prop, v);
        }
    }
    configGroup_.sync();
    updateStatus();
}

void DeviceSettings::updateStatus()
{
    bool needsSave = false;
    bool differsFromActive = false;
    bool savedDiffersFromActive = false;

    Q_FOREACH (const QString &prop, values_.keys()) {
        auto current = values_.value(prop);
        auto device = deviceValue(prop);
        auto saved = fixupType(savedValue(prop), device);

        if (!compareSettings(current, saved)) {
            needsSave = true;
        }
        if (!compareSettings(current, device)) {
            differsFromActive = true;
        }
        if (!compareSettings(saved, device)) {
            savedDiffersFromActive = true;
        }
    }

    std::swap(needsSave, needsSave_);
    std::swap(differsFromActive, differsFromActive_);
    std::swap(savedDiffersFromActive, savedDiffersFromActive_);

    if (needsSave_ != needsSave) {
        Q_EMIT needsSaveChanged();
    }
    if (differsFromActive_ != differsFromActive) {
        Q_EMIT differsFromActiveChanged();
    }
    if (savedDiffersFromActive_ != savedDiffersFromActive) {
        Q_EMIT savedDiffersFromActiveChanged();
    }
}
