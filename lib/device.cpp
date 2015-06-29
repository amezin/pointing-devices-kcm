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

QByteArrayList InputDevice::supportedProperties() const
{
    QByteArrayList interfaceProperties;
    for (int i = staticMetaObject.propertyOffset(); i < staticMetaObject.propertyCount(); i++) {
        interfaceProperties.append(staticMetaObject.property(i).name());
    }
    return interfaceProperties;
}
