#include <QCoreApplication>
#include <QDebug>

#include "xinputdevice.h"
#include "xinputdevicemanager.h"

static void addDevice(XInputDevice *dev)
{
    qDebug() << "Device added:" << dev->name();

    QObject::connect(dev, &XInputDevice::enabledChanged, [dev]() {
        qDebug() << "Device" << dev->name() << "enabled:" << dev->enabled();
    });
    QObject::connect(dev, &XInputDevice::typeChanged, [dev]() {
        qDebug() << "Device" << dev->name() << "type:" << dev->type();
    });
    QObject::connect(dev, &XInputDevice::devicePropertyChanged, [dev](const QByteArray &prop) {
        qDebug() << "Device" << dev->name() << "property" << prop << "changed";
    });
    QObject::connect(dev, &XInputDevice::devicePropertyAdded, [dev](const QByteArray &prop) {
        qDebug() << "Device" << dev->name() << "property" << prop << "added";
    });
    QObject::connect(dev, &XInputDevice::devicePropertyRemoved, [dev](const QByteArray &prop) {
        qDebug() << "Device" << dev->name() << "property" << prop << "removed";
    });
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    XInputDeviceManager manager;
    QObject::connect(&manager, &XInputDeviceManager::deviceAdded, &addDevice);
    QObject::connect(&manager, &XInputDeviceManager::deviceRemoved, [](XInputDevice *dev) {
        qDebug() << "Device removed:" << dev->name();
    });

    Q_FOREACH (auto dev, manager.devices()) {
        addDevice(dev);
    }

    return app.exec();
}
