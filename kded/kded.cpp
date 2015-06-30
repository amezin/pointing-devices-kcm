#include "kded.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QTimer>

#include <KConfig>
#include <KConfigGroup>
#include <KPluginFactory>

#include "devicemanager.h"
#include "device.h"
#include "kdedadaptor.h"

K_PLUGIN_FACTORY(PointingDevicesKDEDFactory, registerPlugin<PointingDevicesKDED>();)

PointingDevicesKDED::PointingDevicesKDED(QObject *parent, const QVariantList &)
    : KDEDModule(parent),
      deviceManager_(InputDeviceManager::instance()),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc"))
{
    connect(deviceManager_, &InputDeviceManager::deviceAdded, this, [this](InputDevice *dev) {
        /*
         * When XInput devices are plugged in, they are disabled at first.
         * After short amount of time X enables them.
         * Wait for 1 sec to prevent "enabled=false" from being saved for all devices as default.
         */
        QPointer<InputDevice> devPtr(dev);
        QTimer::singleShot(1000, this, [this, devPtr]() {
            if (devPtr) { // the device can possibly be deleted at this point
                applyConfig(devPtr, true);
            }
        });
    });

    Q_FOREACH (auto device, deviceManager_->devices()) {
        applyConfig(device);
    }

    new PointingDevicesAdaptor(this);
}

PointingDevicesKDED::~PointingDevicesKDED()
{
}

void PointingDevicesKDED::applyConfig(InputDevice *device, bool overwriteDefaults)
{
    Q_ASSERT(device);

    connect(device, &InputDevice::supportedPropertiesChanged,
            this, &PointingDevicesKDED::reapplyConfig, Qt::UniqueConnection);

    auto group = config_.group(device->identifier());
    auto defaultsGroup = defaults_.group(device->identifier());

    auto metaObject = device->metaObject();
    auto supported = device->supportedProperties();

    for (auto i = InputDevice::staticMetaObject.propertyOffset(); i < metaObject->propertyCount(); i++) {
        auto prop = metaObject->property(i);
        if (!prop.isReadable() || !prop.isWritable() || !supported.contains(prop.name())) {
            continue;
        }

        auto currentValue = prop.read(device);
        if (!defaultsGroup.hasKey(prop.name()) || overwriteDefaults) {
            defaultsGroup.writeEntry(prop.name(), currentValue);
        }

        auto defaultValue = defaultsGroup.readEntry(prop.name(), currentValue);
        auto newValue = group.readEntry(prop.name(), defaultValue);
        prop.write(device, newValue);
    }

    if (overwriteDefaults) {
        Q_FOREACH (const auto &key, defaultsGroup.keyList()) {
            if (!supported.contains(key.toLatin1())) {
                defaultsGroup.deleteEntry(key);
            }
        }
    }

    defaultsGroup.sync();
}

void PointingDevicesKDED::reapplyConfig()
{
    auto device = qobject_cast<InputDevice *>(sender());
    Q_ASSERT(device);
    applyConfig(device, false);
}

void PointingDevicesKDED::reloadConfig()
{
    config_.reparseConfiguration();
    Q_FOREACH (auto device, deviceManager_->devices()) {
        applyConfig(device);
    }
}

void PointingDevicesKDED::reloadConfig(const QString &identifier)
{
    config_.reparseConfiguration();
    auto device = deviceManager_->deviceByIdentifier(identifier);
    applyConfig(device);
}

#include "kded.moc"
