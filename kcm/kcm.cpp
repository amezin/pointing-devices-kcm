#include "kcm.h"

#include <QQmlEngine>
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>

#include "devicemanager.h"
#include "device.h"

K_PLUGIN_FACTORY_WITH_JSON(PointingDevicesKCMFactory, "kcm_pointingdevices.json", registerPlugin<PointingDevicesKCM>();)

PointingDevicesKCM::PointingDevicesKCM(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(parent, args),
      deviceManager_(InputDeviceManager::instance())
{
    connect(deviceManager_, &InputDeviceManager::deviceAdded, this, &PointingDevicesKCM::setCppOwnership);
    connect(deviceManager_, &InputDeviceManager::deviceAdded, this, &PointingDevicesKCM::deviceAdded);
    connect(deviceManager_, &InputDeviceManager::deviceRemoved, this, &PointingDevicesKCM::deviceRemoved);

    qmlRegisterType<InputDeviceManager>();
    qmlRegisterType<InputDevice>();

    KAboutData *about = new KAboutData("kcm_pointingdevices", i18n("Pointing Devices KCM"), "0.0.1");
    setAboutData(about);
}

PointingDevicesKCM::~PointingDevicesKCM()
{
}

void PointingDevicesKCM::setCppOwnership(QObject *o)
{
    QQmlEngine::setObjectOwnership(o, QQmlEngine::CppOwnership);
}

QList<QObject *> PointingDevicesKCM::devices() const
{
    QList<QObject *> list;
    Q_FOREACH (auto dev, deviceManager_->devices()) {
        list.append(dev);
    }
    return list;
}

#include "kcm.moc"
