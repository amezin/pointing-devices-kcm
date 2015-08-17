#include "kcm.h"

#include <QQmlEngine>
#include <KPluginFactory>
#include <KPluginMetaData>

#include "device.h"
#include "devicemanager.h"
#include "deviceproperty.h"

K_PLUGIN_FACTORY_WITH_JSON(PointingDevicesKCMFactory, "kcm_pointingdevices.json", registerPlugin<PointingDevicesKCM>();)

PointingDevicesKCM::PointingDevicesKCM(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(KPluginMetaData(QStringLiteral("kcms/kcm_pointingdevices")), parent, args),
      deviceManager_(InputDeviceManager::create(this)),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc"))
{
    qmlRegisterType<InputDevice>();
    qmlRegisterType<DeviceSettings>();
    qmlRegisterType<DeviceProperty>("org.kde.PointingDevicesKCM", 1, 0, "DeviceProperty");

    if (deviceManager_) {
        connect(deviceManager_, &InputDeviceManager::deviceAdded,
                this, &PointingDevicesKCM::deviceAdded);
        connect(deviceManager_, &InputDeviceManager::deviceRemoved,
                this, &PointingDevicesKCM::deviceRemoved);
    }
}

PointingDevicesKCM::~PointingDevicesKCM()
{
}

void PointingDevicesKCM::load()
{
    config_.reparseConfiguration();
    defaults_.reparseConfiguration();
    Q_EMIT loadRequested();
}

void PointingDevicesKCM::defaults()
{
    config_.reparseConfiguration();
    defaults_.reparseConfiguration();
    Q_EMIT defaultsRequested();
}

void PointingDevicesKCM::save()
{
    Q_EMIT saveRequested();
}

InputDeviceManager *PointingDevicesKCM::deviceManager() const
{
    return deviceManager_;
}

DeviceSettings *PointingDevicesKCM::settingsFor(InputDevice *device,
                                                QObject *parent)
{
    if (!device) {
        return Q_NULLPTR;
    }
    return new DeviceSettings(&config_, &defaults_, device, parent);
}

QList<QObject *> PointingDevicesKCM::allDevices() const
{
    QList<QObject *> devices;
    if (deviceManager_) {
        Q_FOREACH (auto device, deviceManager_->devices()) {
            devices.append(device);
        }
    }
    return devices;
}

#include "kcm.moc"
