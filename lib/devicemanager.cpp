#include "devicemanager.h"

#include <QPluginLoader>
#include <QScopedPointer>

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
    QPluginLoader loader(QStringLiteral("pointingdevices_x11"));
    if (!loader.instance()) {
        qCCritical(POINTINGDEVICES) << "Can't load plugin" << loader.fileName() << ":" << loader.errorString();
    }
    return qobject_cast<InputDeviceManager *>(loader.instance());
}

InputDeviceManager *InputDeviceManager::instance()
{
    static QScopedPointer<InputDeviceManager> instance_(create());
    return instance_.data();
}
