#include "deviceproperty.h"

#include <QJSValueList>

DeviceProperty::DeviceProperty(QObject *parent)
    : QObject(parent),
      device_(Q_NULLPTR),
      prevAvailable_(false),
      prevWritable_(false)
{
}

DeviceProperty::~DeviceProperty()
{
}

DeviceSettings *DeviceProperty::device() const
{
    return device_;
}

void DeviceProperty::setDevice(DeviceSettings *device)
{
    if (device_) {
        disconnect(device_, &DeviceSettings::changed,
                   this, &DeviceProperty::update);
        disconnect(device_, &QObject::destroyed,
                   this, &DeviceProperty::handleDeviceDestroyed);
    }
    device_ = device;
    if (device_) {
        connect(device_, &DeviceSettings::changed,
                this, &DeviceProperty::update);
        connect(device_, &QObject::destroyed,
                this, &DeviceProperty::handleDeviceDestroyed);
    }
    update();
    Q_EMIT deviceChanged(device);
}

bool DeviceProperty::isAvailable() const
{
    return device_ && device_->isPropertySupported(name_);
}

bool DeviceProperty::isWritable() const
{
    return isAvailable() && device_->isPropertyWritable(name_);
}

QString DeviceProperty::name() const
{
    return name_;
}

QVariant DeviceProperty::value() const
{
    if (!isAvailable()) {
        return QVariant();
    }
    return device_->value(name_);
}

void DeviceProperty::setName(const QString &name)
{
    if (name != name_) {
        name_ = name;
        update();
        Q_EMIT nameChanged(name);
    }
}

void DeviceProperty::setValue(const QVariant &value)
{
    if (value.userType() == qMetaTypeId<QJSValue>()) {
        setValue(value.value<QJSValue>().toVariant());
    } else {
        device_->setValue(name_, value);
    }
}

void DeviceProperty::handleDeviceDestroyed(QObject *o)
{
    if (device_ == o) {
        device_ = Q_NULLPTR;
        update();
        Q_EMIT deviceChanged(device_);
    }
}

void DeviceProperty::update()
{
    auto newAvailabe = isAvailable();
    auto newWritable = isWritable();
    auto newValue = value();

    if (!newWritable && prevWritable_) {
        Q_EMIT writableChanged(newWritable);
    }
    if (!newAvailabe && prevAvailable_) {
        Q_EMIT availableChanged(newAvailabe);
    }
    if (newValue != prevValue_) {
        Q_EMIT valueChanged(newValue);
    }
    if (newAvailabe && !prevAvailable_) {
        Q_EMIT availableChanged(newAvailabe);
    }
    if (newWritable && !prevWritable_) {
        Q_EMIT writableChanged(newWritable);
    }

    prevValue_ = newValue;
    prevAvailable_ = newAvailabe;
    prevWritable_ = newWritable;
}
