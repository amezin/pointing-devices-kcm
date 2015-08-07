#include "kded.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QTimer>

#include <KConfig>
#include <KConfigGroup>

#include "devicemanager.h"
#include "device.h"
#include "kdedadaptor.h"

K_PLUGIN_FACTORY_WITH_JSON(PointingDevicesKDEDFactory, "kded_pointingdevices.json", registerPlugin<PointingDevicesKDED>();)

PointingDevicesKDED::PointingDevicesKDED(QObject *parent, const QVariantList &)
    : KDEDModule(parent),
      deviceManager_(InputDeviceManager::create(this)),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc"))
{
    connect(deviceManager_, &InputDeviceManager::deviceAdded,
            this, [this](InputDevice *device)
            {
                config_.reparseConfiguration();
                PointingDevicesKDED::applyConfig(device);
            });

    Q_FOREACH (auto device, deviceManager_->devices()) {
        applyConfig(device);
    }

    new PointingDevicesAdaptor(this);
}

PointingDevicesKDED::~PointingDevicesKDED()
{
}

void PointingDevicesKDED::applyConfig(InputDevice *device)
{
    Q_ASSERT(device);

    connect(device, &InputDevice::propertyAdded,
            this, &PointingDevicesKDED::reapplyConfig, Qt::UniqueConnection);

    config_.reparseConfiguration();
    auto group = config_.group(device->identifier());
    auto defaultsGroup = defaults_.group(device->identifier());

    Q_FOREACH (const auto &prop, device->supportedProperties()) {
        applyProperty(device, prop, group, defaultsGroup);
    }

    defaultsGroup.sync();
}

void PointingDevicesKDED::applyProperty(InputDevice *device, const QString &prop,
                                        KConfigGroup &group, KConfigGroup &defaultsGroup)
{
    if (!device->isPropertyWritable(prop)) {
        return;
    }

    auto currentValue = device->deviceProperty(prop);
    if (!defaultsGroup.hasKey(prop)) {
        defaultsGroup.writeEntry(prop, currentValue);
    }

    auto defaultValue = defaultsGroup.readEntry(prop, currentValue);
    auto newValue = group.readEntry(prop, defaultValue);

    if (newValue != currentValue) {
        device->setDeviceProperty(prop, newValue);
    }
}

void PointingDevicesKDED::reapplyConfig(const QString &prop)
{
    auto device = qobject_cast<InputDevice *>(sender());
    Q_ASSERT(device);

    config_.reparseConfiguration();
    auto group = config_.group(device->identifier());
    auto defaultsGroup = defaults_.group(device->identifier());
    applyProperty(device, prop, group, defaultsGroup);
    defaultsGroup.sync();
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
