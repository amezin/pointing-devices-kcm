#include "kwindevicemanager.h"

#include <KPluginFactory>

#include "devicemanagerinterface.h"
#include "deviceinterface.h"
#include "kwindevice.h"

K_PLUGIN_FACTORY(KWinDeviceManagerFactory, registerPlugin<KWinDeviceManager>();)

KWinDeviceManager::KWinDeviceManager(QObject *parent, const QVariantList &)
    : InputDeviceManager(parent)
{
    interface_ = new OrgKdeKWinInputDeviceManagerInterface(QStringLiteral("org.kde.KWin"),
                                                           QStringLiteral("/Input"),
                                                           QDBusConnection::sessionBus(),
                                                           this);
    if (!interface_->isValid()) {
        return;
    }

    connect(interface_, &OrgKdeKWinInputDeviceManagerInterface::deviceAdded,
            this, &KWinDeviceManager::addDevice);
    connect(interface_, &OrgKdeKWinInputDeviceManagerInterface::deviceRemoved,
            this, &KWinDeviceManager::removeDevice);

    Q_FOREACH (auto path, interface_->devices().value()) {
        addDevice(path);
    }
}

KWinDeviceManager::~KWinDeviceManager()
{
}

QList<InputDevice *> KWinDeviceManager::devices() const
{
    QList<InputDevice *> out;
    out.reserve(devices_.size());
    Q_FOREACH (auto dev, devices_) {
        out.append(dev);
    }
    return out;
}

void KWinDeviceManager::addDevice(const QDBusObjectPath &path)
{
    Q_FOREACH (auto dev, devices_) {
        if (dev->path() == path) {
            return;
        }
    }

    auto dev = new OrgKdeKWinInputDeviceInterface(QStringLiteral("org.kde.KWin"),
                                                  path.path(),
                                                  QDBusConnection::sessionBus(),
                                                  this);

    if (!dev->isPointer()) {
        delete dev;
        return;
    }

    auto wrapper = new KWinDevice(dev, this);
    devices_.append(wrapper);
    Q_EMIT deviceAdded(wrapper);
}

void KWinDeviceManager::removeDevice(const QDBusObjectPath &path)
{
    Q_FOREACH (auto dev, devices_) {
        if (dev->path() == path) {
            Q_EMIT deviceRemoved(dev);
            delete dev;
            devices_.removeAll(dev);
        }
    }
}

#include "kwindevicemanager.moc"
