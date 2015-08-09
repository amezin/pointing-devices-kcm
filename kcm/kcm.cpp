#include "kcm.h"

#include <QQmlEngine>
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>

#include "devicemanager.h"
#include "device.h"
#include "devicesettings.h"
#include "deviceproperty.h"
#include "devicelistmodel.h"

K_PLUGIN_FACTORY_WITH_JSON(PointingDevicesKCMFactory, "kcm_pointingdevices.json", registerPlugin<PointingDevicesKCM>();)

PointingDevicesKCM::PointingDevicesKCM(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(parent, args),
      deviceManager_(InputDeviceManager::create(this)),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc")),
      deviceList_(new DeviceListModel(this))
{
    KAboutData *about = new KAboutData("kcm_pointingdevices", i18n("Pointing Devices KCM"), "0.0.1");
    setAboutData(about);

    qmlRegisterType<DeviceListModel>();
    qmlRegisterType<DeviceSettings>();
    qmlRegisterType<DeviceProperty>("org.kde.PointingDevicesKCM", 1, 0, "DeviceProperty");

    if (!deviceManager_) {
        return; // TODO: error message
    }

    connect(deviceManager_, &InputDeviceManager::deviceAdded,
            this, &PointingDevicesKCM::addDevice, Qt::QueuedConnection);
    connect(deviceManager_, &InputDeviceManager::deviceRemoved,
            this, &PointingDevicesKCM::removeDevice);
    Q_FOREACH (InputDevice *dev, deviceManager_->devices()) {
        addDevice(dev);
    }
}

PointingDevicesKCM::~PointingDevicesKCM()
{
}

void PointingDevicesKCM::addDevice(InputDevice *dev)
{
    auto settings = new DeviceSettings(&config_, &defaults_, dev, this);
    QQmlEngine::setObjectOwnership(settings, QQmlEngine::CppOwnership);
    deviceList_->add(settings);
    connect(settings, &DeviceSettings::needsSaveChanged,
            this, &PointingDevicesKCM::updateNeedsSave);

    updateNeedsSave();
}

void PointingDevicesKCM::removeDevice(InputDevice *dev)
{
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        if (settings->device() == dev) {
            deviceList_->remove(settings);
            disconnect(settings, &DeviceSettings::needsSaveChanged,
                       this, &PointingDevicesKCM::updateNeedsSave);
        }
    }
}

void PointingDevicesKCM::updateNeedsSave()
{
    bool needsSave = false;
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        if (settings->needsSave()) {
            needsSave = true;
        }
    }
    if (this->needsSave() != needsSave) {
        setNeedsSave(needsSave);
    }
}

void PointingDevicesKCM::load()
{
    config_.reparseConfiguration();
    defaults_.reparseConfiguration();
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        settings->loadSaved();
    }
}

void PointingDevicesKCM::defaults()
{
    config_.reparseConfiguration();
    defaults_.reparseConfiguration();
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        settings->loadDefaults();
    }
}

void PointingDevicesKCM::save()
{
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        settings->apply();
        settings->save();
    }
}

#include "kcm.moc"
