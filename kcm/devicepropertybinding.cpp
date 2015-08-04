#include "devicepropertybinding.h"

DevicePropertyBinding::DevicePropertyBinding(QObject *parent)
    : QObject(parent),
      prevEnabled_(false)
{
}

DevicePropertyBinding::~DevicePropertyBinding()
{
}

void DevicePropertyBinding::setDevice(DeviceSettings *device)
{
    if (device_ != device) {
        device_ = device;
        update();
        Q_EMIT deviceChanged(device);
    }
}

void DevicePropertyBinding::setDevice(QObject *device)
{
    setDevice(qobject_cast<DeviceSettings *>(device));
}

void DevicePropertyBinding::setDeviceProperty(const QString &property)
{
    if (deviceProperty_ != property) {
        deviceProperty_ = property;
        update();
        Q_EMIT devicePropertyChanged(property);
    }
}

void DevicePropertyBinding::setTarget(const QQmlProperty &target)
{
    target_ = target;
    update();
}

void DevicePropertyBinding::update()
{
    if (prevDevice_) {
        disconnect(device_, &DeviceSettings::supportedPropertiesChanged,
                   this, &DevicePropertyBinding::update);
        prevDevice_->disconnect(this, SLOT(devToTarget()));
        prevDevice_ = Q_NULLPTR;
    }

    if (prevTarget_) {
        prevTarget_->disconnect(this, SLOT(targetToDev()));
        prevTarget_ = Q_NULLPTR;
    }

    if (!target_.isValid()) {
        prevEnabled_ = false;
        return;
    }

    bool enable = false;
    if (device_ && device_->supportedProperties().contains(deviceProperty_)) {
        deviceProp_ = QQmlProperty(device_, deviceProperty_);

        connect(device_, &DeviceSettings::supportedPropertiesChanged,
                this, &DevicePropertyBinding::update);

        deviceProp_.connectNotifySignal(this, SLOT(devToTarget()));
        prevDevice_ = device_;

        target_.connectNotifySignal(this, SLOT(targetToDev()));
        prevTarget_ = target_.object();

        enable = true;
    }

    if (enable != prevEnabled_) {
        devToTarget();
        prevEnabled_ = enable;
    }

    QQmlProperty enabled(target_.object(), QStringLiteral("enabled"));
    if (enabled.isValid() && enabled.read().toBool() != enable) {
        enabled.write(enable);
    }
}

void DevicePropertyBinding::devToTarget()
{
    if (deviceProp_.isValid() && target_.isValid()) {
        auto dev = deviceProp_.read();
        if (dev != target_.read()) {
            target_.write(dev);
        }
    }
}

void DevicePropertyBinding::targetToDev()
{
    if (deviceProp_.isValid() && target_.isValid()) {
        auto target = target_.read();
        if (target != deviceProp_.read()) {
            deviceProp_.write(target_.read());
        }
    }
}
