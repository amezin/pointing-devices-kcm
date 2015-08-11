#include "devicemanager.h"

#include <QScopedPointer>
#include <KPluginLoader>
#include <KPluginFactory>

#include "device.h"
#include "log.h"

InputDeviceManager::InputDeviceManager(QObject *parent)
    : QObject(parent)
{
}

InputDeviceManager::~InputDeviceManager()
{
}

InputDevice *InputDeviceManager::deviceByIdentifier(const QString &id) const
{
    Q_FOREACH (auto device, devices()) {
        if (device->identifier() == id) {
            return device;
        }
    }
    return Q_NULLPTR;
}

InputDeviceManager *InputDeviceManager::create(QObject *parent)
{
    KPluginLoader kwinLoader(QStringLiteral("pointingdevices_kwin"));
    if (kwinLoader.factory()) {
        auto manager = kwinLoader.factory()->create<InputDeviceManager>(parent);
        if (!manager->devices().isEmpty()) {
            return manager;
        }
        delete manager;
    }

    KPluginLoader x11Loader(QStringLiteral("pointingdevices_x11"));
    if (!x11Loader.factory()) {
        qCCritical(POINTINGDEVICES) << "Can't load plugin" << x11Loader.fileName() << ":" << x11Loader.errorString();
    }
    return x11Loader.factory()->create<InputDeviceManager>(parent);
}
