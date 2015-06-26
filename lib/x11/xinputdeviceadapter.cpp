#include "xinputdeviceadapter.h"

#include "xinputdevice.h"

XInputDeviceAdapter::XInputDeviceAdapter(XInputDevice *impl, QObject *parent)
    : InputDevice(parent), impl(impl)
{
    connect(impl, &XInputDevice::devicePropertyAdded, this, &XInputDeviceAdapter::handlePropertyAdd);
    connect(impl, &XInputDevice::devicePropertyRemoved, this, &XInputDeviceAdapter::handlePropertyRemove);
    connect(impl, &XInputDevice::devicePropertyChanged, this, &XInputDeviceAdapter::handlePropertyChange);
    connect(impl, &XInputDevice::enabledChanged, this, &XInputDeviceAdapter::enabledChanged);
}

XInputDeviceAdapter::~XInputDeviceAdapter()
{
}

QByteArrayList XInputDeviceAdapter::supportedProperties() const
{
    QByteArrayList interfaceProperties;
    interfaceProperties << QByteArrayLiteral("name")
                        << QByteArrayLiteral("identifier")
                        << QByteArrayLiteral("enabled");
    return interfaceProperties;
}

QString XInputDeviceAdapter::name() const
{
    return impl->name();
}

QString XInputDeviceAdapter::identifier() const
{
    auto id = impl->deviceProperty(QByteArrayLiteral("Device Product ID")).toStringList();
    id.push_front(name());
    return id.join(':');
}

bool XInputDeviceAdapter::isEnabled() const
{
    return impl->enabled();
}

void XInputDeviceAdapter::setEnabled(bool enable)
{
    impl->setDeviceProperty(QByteArrayLiteral("Device Enabled"), enable);
}

void XInputDeviceAdapter::handlePropertyAdd(const QByteArray &name)
{
    handlePropertyChange(name);
    Q_EMIT supportedPropertiesChanged();
}

void XInputDeviceAdapter::handlePropertyRemove(const QByteArray &)
{
    Q_EMIT supportedPropertiesChanged();
}

void XInputDeviceAdapter::handlePropertyChange(const QByteArray &)
{
}
