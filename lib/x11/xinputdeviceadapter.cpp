#include "xinputdeviceadapter.h"

#include <QMetaObject>
#include <QMetaProperty>

#include "xinputdevice.h"

XInputDeviceAdapter::XInputDeviceAdapter(XInputDevice *impl, QObject *parent)
    : InputDevice(parent), impl(impl)
{
    connect(impl, &XInputDevice::devicePropertyAdded, this, &XInputDeviceAdapter::handlePropertyAdd);
    connect(impl, &XInputDevice::devicePropertyRemoved, this, &XInputDeviceAdapter::handlePropertyRemove);
    connect(impl, &XInputDevice::devicePropertyChanged, this, &XInputDeviceAdapter::handlePropertyChange);
    connect(impl, &XInputDevice::enabledChanged, this, &XInputDeviceAdapter::enabledChanged);

#define MAP_PROPERTY(name, xiName) \
    requiredDevProperties_[QByteArrayLiteral(name)].append(QByteArrayLiteral(xiName))

    MAP_PROPERTY("accelProfile", "Device Accel Profile");
    MAP_PROPERTY("accelConstantDeceleration", "Device Accel Constant Deceleration");
    MAP_PROPERTY("accelAdaptiveDeceleration", "Device Accel Adaptive Deceleration");
    MAP_PROPERTY("accelVelocityScaling", "Device Accel Velocity Scaling");

    for (auto i = requiredDevProperties_.constBegin(); i != requiredDevProperties_.constEnd(); ++i) {
        Q_FOREACH (const auto &prop, i.value()) {
            auto &list = notifyOnChange_[prop];
            if (!list.contains(i.key())) {
                list.append(i.key());
            }
        }
    }
}

XInputDeviceAdapter::~XInputDeviceAdapter()
{
}

QByteArrayList XInputDeviceAdapter::supportedProperties() const
{
    auto interfaceProperties = InputDevice::supportedProperties();

    for (auto i = requiredDevProperties_.constBegin(); i != requiredDevProperties_.constEnd(); ++i) {
        bool allPropertiesArePresent = true;
        Q_FOREACH (const auto &prop, i.value()) {
            if (!impl->devicePropertyExists(prop)) {
                allPropertiesArePresent = false;
                break;
            }
        }
        if (allPropertiesArePresent && !interfaceProperties.contains(i.key())) {
            interfaceProperties.append(i.key());
        }
    }

    return interfaceProperties;
}

QString XInputDeviceAdapter::name() const
{
    return impl->name();
}

QString XInputDeviceAdapter::identifier() const
{
    static const auto name = QByteArrayLiteral("Device Product ID");
    auto id = impl->deviceProperty(name).toStringList();

    id.push_front(this->name());

    static const auto xi = QByteArrayLiteral("xinput");
    id.push_back(xi);

    return id.join(':');
}

bool XInputDeviceAdapter::isEnabled() const
{
    return impl->enabled();
}

void XInputDeviceAdapter::setEnabled(bool enable)
{
    static const auto name = QByteArrayLiteral("Device Enabled");
    impl->setDeviceProperty(name, enable);
}

void XInputDeviceAdapter::handlePropertyAdd(const QByteArray &name)
{
    handlePropertyChange(name);
    if (notifyOnChange_.contains(name)) {
        Q_EMIT supportedPropertiesChanged();
    }
}

void XInputDeviceAdapter::handlePropertyRemove(const QByteArray &name)
{
    if (notifyOnChange_.contains(name)) {
        Q_EMIT supportedPropertiesChanged();
    }
}

void XInputDeviceAdapter::handlePropertyChange(const QByteArray &name)
{
    Q_FOREACH (const auto &prop, notifyOnChange_.value(name)) {
        auto propIndex = metaObject()->indexOfProperty(prop.constData());
        Q_ASSERT(propIndex >= 0);

        metaObject()->property(propIndex).notifySignal().invoke(this);
    }
}

#define SIMPLE_GET_SET2(type, realType, getter, setter) \
    type XInputDeviceAdapter::getter() const \
    { \
        static const auto name = QByteArrayLiteral(#getter); \
        auto xiProp = requiredDevProperties_.value(name).first(); \
        return static_cast<type>(impl->deviceProperty(xiProp).value<realType>()); \
    } \
    void XInputDeviceAdapter::setter(type v) \
    { \
        static const auto name = QByteArrayLiteral(#getter); \
        auto xiProp = requiredDevProperties_.value(name).first(); \
        impl->setDeviceProperty(xiProp, static_cast<realType>(v)); \
    }

#define SIMPLE_GET_SET(type, getter, setter) \
    SIMPLE_GET_SET2(type, type, getter, setter)

SIMPLE_GET_SET2(InputDevice::XAccelerationProfile, int, accelProfile, setAccelProfile)
SIMPLE_GET_SET(float, accelConstantDeceleration, setAccelConstantDeceleration)
SIMPLE_GET_SET(float, accelAdaptiveDeceleration, setAccelAdaptiveDeceleration)
SIMPLE_GET_SET(float, accelVelocityScaling, setAccelVelocityScaling)
