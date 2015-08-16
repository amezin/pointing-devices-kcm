#include "kcm.h"

#include <QQmlEngine>
#include <KPluginFactory>
#include <KPluginMetaData>

#include "devicemanager.h"
#include "device.h"
#include "devicesettings.h"
#include "deviceproperty.h"
#include "devicelistmodel.h"

K_PLUGIN_FACTORY_WITH_JSON(PointingDevicesKCMFactory, "kcm_pointingdevices.json", registerPlugin<PointingDevicesKCM>();)

PointingDevicesKCM::PointingDevicesKCM(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(KPluginMetaData(QStringLiteral("kcms/kcm_pointingdevices")), parent, args),
      deviceManager_(InputDeviceManager::create(this)),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc")),
      deviceList_(new DeviceListModel(this)),
      canTest_(false), canRevertTest_(false)
{
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
    revertTest();
}

void PointingDevicesKCM::addDevice(InputDevice *dev)
{
    auto settings = new DeviceSettings(&config_, &defaults_, dev, this);
    QQmlEngine::setObjectOwnership(settings, QQmlEngine::CppOwnership);
    deviceList_->add(settings);
    connect(settings, &DeviceSettings::needsSaveChanged,
            this, &PointingDevicesKCM::updateNeedsSave);
    connect(settings, &DeviceSettings::differsFromActiveChanged,
            this, &PointingDevicesKCM::updateCanTest);
    connect(settings, &DeviceSettings::savedDiffersFromActiveChanged,
            this, &PointingDevicesKCM::updateCanRevertTest);

    updateNeedsSave();
    updateCanTest();
    updateCanRevertTest();
}

void PointingDevicesKCM::removeDevice(InputDevice *dev)
{
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        if (settings->device() == dev) {
            deviceList_->remove(settings);
            disconnect(settings, &DeviceSettings::needsSaveChanged,
                       this, &PointingDevicesKCM::updateNeedsSave);
            disconnect(settings, &DeviceSettings::differsFromActiveChanged,
                       this, &PointingDevicesKCM::updateCanTest);
            disconnect(settings, &DeviceSettings::savedDiffersFromActiveChanged,
                       this, &PointingDevicesKCM::updateCanRevertTest);
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

void PointingDevicesKCM::updateCanTest()
{
    bool canTest = false;
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        if (settings->differsFromActive()) {
            canTest = true;
        }
    }
    setCanTest(canTest);
}

void PointingDevicesKCM::updateCanRevertTest()
{
    bool canRevertTest = false;
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        if (settings->savedDiffersFromActive()) {
            canRevertTest = true;
        }
    }
    setCanRevertTest(canRevertTest);
}

void PointingDevicesKCM::load()
{
    config_.reparseConfiguration();
    defaults_.reparseConfiguration();
    revertTest();
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        settings->loadSaved();
    }
}

void PointingDevicesKCM::defaults()
{
    config_.reparseConfiguration();
    defaults_.reparseConfiguration();
    revertTest();
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

bool PointingDevicesKCM::canTest() const
{
    return canTest_;
}

void PointingDevicesKCM::setCanTest(bool canTest)
{
    if (canTest_ != canTest) {
        canTest_ = canTest;
        Q_EMIT canTestChanged();
    }
}

bool PointingDevicesKCM::canRevertTest() const
{
    return canRevertTest_;
}

void PointingDevicesKCM::setCanRevertTest(bool canRevertTest)
{
    if (canRevertTest_ != canRevertTest) {
        canRevertTest_ = canRevertTest;
        Q_EMIT canRevertTestChanged();
    }
}

void PointingDevicesKCM::test()
{
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        settings->apply();
    }
}

void PointingDevicesKCM::revertTest()
{
    Q_FOREACH (DeviceSettings *settings, deviceList_->all()) {
        settings->applySaved();
    }
}

#include "kcm.moc"
