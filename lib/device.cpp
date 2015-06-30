#include "device.h"

#include <QMetaObject>
#include <QMetaProperty>

InputDevice::InputDevice(QObject *parent)
    : QObject(parent)
{
}

InputDevice::~InputDevice()
{
}

QStringList InputDevice::supportedProperties() const
{
    QStringList interfaceProperties;
    for (int i = staticMetaObject.propertyOffset(); i < staticMetaObject.propertyCount(); i++) {
        interfaceProperties.append(QLatin1String(staticMetaObject.property(i).name()));
    }
    return interfaceProperties;
}
