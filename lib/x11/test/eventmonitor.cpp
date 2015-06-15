#include <QCoreApplication>
#include <QDebug>

#include "xinputdevice.h"
#include "xinputdevicemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    XInputDeviceManager manager;
    QObject::connect(&manager, &XInputDeviceManager::deviceAdded, [](XInputDevice *dev) {
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
    });
    QObject::connect(&manager, &XInputDeviceManager::deviceRemoved, [](XInputDevice *dev) {
        qDebug() << "Device removed:" << dev->name();
    });

    return app.exec();
}
