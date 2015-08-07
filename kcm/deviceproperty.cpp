#include "deviceproperty.h"

#include <QJSValueList>

DeviceProperty::DeviceProperty(QObject *parent)
    : QObject(parent),
      device_(Q_NULLPTR),
      available_(false),
      writable_(false)
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
    return available_;
}

void DeviceProperty::setAvailable(bool value)
{
    if (value != available_) {
        available_ = value;
        Q_EMIT availableChanged(value);
    }
}

bool DeviceProperty::isWritable() const
{
    return writable_;
}

void DeviceProperty::setWritable(bool value)
{
    if (value != writable_) {
        writable_ = value;
        Q_EMIT writableChanged(value);
    }
}

QString DeviceProperty::name() const
{
    return name_;
}

QVariant DeviceProperty::value() const
{
    return value_;
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
        return;
    }
    if (value != value_) {
        value_ = value;
        if (device_) {
            device_->setValue(name_, value);
        }
        Q_EMIT valueChanged(value);
    }
}

void DeviceProperty::handleDeviceDestroyed(QObject *o)
{
    if (device_ == o) {
        device_ = Q_NULLPTR;
        Q_EMIT deviceChanged(device_);
    }

    setAvailable(false);
}

void DeviceProperty::update()
{
    if (!device_) {
        setAvailable(false);
        return;
    }

    bool available = device_->isPropertySupported(name_);
    if (available) {
        setValue(device_->value(name_));
    }
    setAvailable(available);
    setWritable(available && device_->isPropertyWritable(name_));
}
