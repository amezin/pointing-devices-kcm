#include "xinputdevicemanageradapter.h"

#include <KPluginFactory>

#include "xinputdevicemanager.h"
#include "xinputdevice.h"
#include "xinputdeviceadapter.h"

K_PLUGIN_FACTORY(XInputDeviceManagerFactory, registerPlugin<XInputDeviceManagerAdapter>();)

XInputDeviceManagerAdapter::XInputDeviceManagerAdapter(XInputDeviceManager *impl,
                                                       QObject *parent)
    : InputDeviceManager(parent),
      impl(impl)
{
    init();
}

XInputDeviceManagerAdapter::XInputDeviceManagerAdapter(QObject *parent,
                                                       const QVariantList &)
    : InputDeviceManager(parent),
      impl(new XInputDeviceManager(Q_NULLPTR, this))
{
    init();
}

XInputDeviceManagerAdapter::~XInputDeviceManagerAdapter()
{
}

void XInputDeviceManagerAdapter::init()
{
    qRegisterMetaType<XInputDeviceAdapter*>();

    connect(impl, &XInputDeviceManager::deviceAdded,
            this, &XInputDeviceManagerAdapter::addDevice);
    connect(impl, &XInputDeviceManager::deviceRemoved,
            this, &XInputDeviceManagerAdapter::removeDevice);

    Q_FOREACH (auto device, impl->devices()) {
        addDeviceNoSignal(device);
    }
}

QList<InputDevice *> XInputDeviceManagerAdapter::devices() const
{
    return devices_;
}

XInputDeviceAdapter *XInputDeviceManagerAdapter::wrapperFor(XInputDevice *device) const
{
    Q_FOREACH (auto wrapper, devices_) {
        if (qobject_cast<XInputDeviceAdapter *>(wrapper)->device() == device) {
            return qobject_cast<XInputDeviceAdapter *>(wrapper);
        }
    }
    return Q_NULLPTR;
}

XInputDeviceAdapter *XInputDeviceManagerAdapter::addDeviceNoSignal(XInputDevice *device)
{
    if (wrapperFor(device)) {
        return Q_NULLPTR;
    }

    connect(device, &XInputDevice::typeChanged,
            this, &XInputDeviceManagerAdapter::handleDeviceTypeChange,
            Qt::UniqueConnection);
    connect(device, &XInputDevice::devicePropertyAdded,
            this, &XInputDeviceManagerAdapter::handleDeviceProperyAdd,
            Qt::UniqueConnection);
    connect(device, &XInputDevice::devicePropertyRemoved,
            this, &XInputDeviceManagerAdapter::handleDeviceProperyRemove,
            Qt::UniqueConnection);

    if (device->type() != XCB_INPUT_DEVICE_TYPE_SLAVE_POINTER) {
        return Q_NULLPTR;
    }

    static const auto DeviceProductId = QByteArrayLiteral("Device Product ID");
    if (!device->devicePropertyExists(DeviceProductId)) {
        return Q_NULLPTR;
    }

    auto wrapper = new XInputDeviceAdapter(device, this);
    devices_.append(wrapper);
    return wrapper;
}

void XInputDeviceManagerAdapter::addDevice(XInputDevice *device)
{
    auto wrapper = addDeviceNoSignal(device);
    if (wrapper) {
        QMetaObject::invokeMethod(this, "delayedEmitDeviceAdded", Qt::QueuedConnection,
                                  Q_ARG(XInputDeviceAdapter*, wrapper));
    }
}

void XInputDeviceManagerAdapter::delayedEmitDeviceAdded(XInputDeviceAdapter *device)
{
    if (devices_.contains(device)) {
        Q_EMIT deviceAdded(device);
    }
}

void XInputDeviceManagerAdapter::removeDevice(XInputDevice *device)
{
    auto wrapper = wrapperFor(device);
    if (!wrapper) {
        return;
    }

    Q_EMIT deviceRemoved(wrapper);

    devices_.removeAll(wrapper);
    delete wrapper;
}

void XInputDeviceManagerAdapter::handleDeviceTypeChange()
{
    auto device = qobject_cast<XInputDevice *>(sender());
    Q_ASSERT(device);

    if (device->type() == XCB_INPUT_DEVICE_TYPE_SLAVE_POINTER) {
        addDevice(device);
    } else {
        removeDevice(device);
    }
}

void XInputDeviceManagerAdapter::handleDeviceProperyAdd(const QByteArray &name)
{
    if (name == "Device Product ID") {
        auto device = qobject_cast<XInputDevice *>(sender());
        Q_ASSERT(device);

        addDevice(device);
    }
}

void XInputDeviceManagerAdapter::handleDeviceProperyRemove(const QByteArray &name)
{
    if (name == "Device Product ID") {
        auto device = qobject_cast<XInputDevice *>(sender());
        Q_ASSERT(device);

        removeDevice(device);
    }
}

#include "xinputdevicemanageradapter.moc"
