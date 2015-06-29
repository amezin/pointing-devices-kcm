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

InputDeviceManager *InputDeviceManager::create()
{
    KPluginLoader loader(QStringLiteral("pointingdevices_x11"));
    if (!loader.factory()) {
        qCCritical(POINTINGDEVICES) << "Can't load plugin" << loader.fileName() << ":" << loader.errorString();
    }
    return loader.factory()->create<InputDeviceManager>();
}

InputDeviceManager *InputDeviceManager::instance()
{
    static QScopedPointer<InputDeviceManager> instance_(create());
    return instance_.data();
}
