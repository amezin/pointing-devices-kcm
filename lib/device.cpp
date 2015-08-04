#include "device.h"

InputDevice::InputDevice(QObject *parent)
    : QObject(parent)
{
}

InputDevice::~InputDevice()
{
}

QStringList InputDevice::supportedProperties() const
{
    return QStringList(QStringLiteral("enabled"));
}
