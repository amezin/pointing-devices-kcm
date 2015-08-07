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
      defaultsGroup_(device ? defaults->group(device->identifier()) : KConfigGroup())
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
    return readonly_.contains(prop);
}

bool DeviceSettings::isPropertyWritable(const QString &prop) const
{
    return values_.contains(prop);
}

bool DeviceSettings::needsSave() const
{
    return needsSave_;
}

void DeviceSettings::setNeedsSave(bool needs)
{
    if (needsSave_ != needs) {
        needsSave_ = needs;
        Q_EMIT needsSaveChanged();
    }
}

QVariant DeviceSettings::value(const QString &prop) const
{
    return values_.value(prop, readonly_.value(prop, QVariant()));
}

void DeviceSettings::setValue(const QString &prop, const QVariant &value)
{
    if (setValueNoSignal(prop, value)) {
        Q_EMIT changed();
        updateStatus();
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
    readonly_.insert(name, device_->deviceProperty(name));
    if (device_ && device_->isPropertyWritable(name)) {
        values_.insert(name, fixupType(savedValue(name), deviceValue(name)));
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
    if (device_ && !device_->isPropertyWritable(name)) {
        values_.remove(name);
    }
    addProperty(name);
}

QVariant DeviceSettings::savedValue(const QString &name) const
{
    return configGroup_.readEntry(name, defaultValue(name));
}

QVariant DeviceSettings::defaultValue(const QString &name) const
{
     return defaultsGroup_.readEntry(name, deviceValue(name));
}

QVariant DeviceSettings::deviceValue(const QString &name) const
{
    return readonly_.value(name, QVariant());
}

QVariant DeviceSettings::fixupType(const QVariant &value, const QVariant &pattern) const
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
    Q_FOREACH (const QString &prop, values_.keys()) {
        if (!device_->setDeviceProperty(prop, value(prop))) {
            qWarning(POINTINGDEVICES) << "Failed to set" << prop << "to" << value(prop);
        }
    }
    updateStatus();
}

void DeviceSettings::save()
{
    Q_FOREACH (const QString &prop, configGroup_.keyList()) {
        if (!isPropertyWritable(prop)) {
            configGroup_.deleteEntry(prop);
        }
    }
    Q_FOREACH (const QString &prop, values_.keys()) {
        auto v = value(prop);
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
    Q_FOREACH (const QString &prop, values_.keys()) {
        if (!compareSettings(value(prop), savedValue(prop))) {
            needsSave = true;
        }
    }
    setNeedsSave(needsSave);
}
