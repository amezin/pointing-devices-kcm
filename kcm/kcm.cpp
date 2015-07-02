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
      deviceManager_(InputDeviceManager::create(this)),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc"))
{
    connect(deviceManager_, &InputDeviceManager::deviceAdded, [](QObject *o) {
        QQmlEngine::setObjectOwnership(o, QQmlEngine::CppOwnership);
    });

    connect(deviceManager_, &InputDeviceManager::deviceAdded, this, &PointingDevicesKCM::deviceAdded);
    connect(deviceManager_, &InputDeviceManager::deviceRemoved, this, &PointingDevicesKCM::deviceRemoved);

    KAboutData *about = new KAboutData("kcm_pointingdevices", i18n("Pointing Devices KCM"), "0.0.1");
    setAboutData(about);
}

PointingDevicesKCM::~PointingDevicesKCM()
{
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
