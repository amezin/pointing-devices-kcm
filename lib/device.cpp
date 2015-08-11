#include "device.h"

InputDevice::InputDevice(QObject *parent)
    : QObject(parent)
{
    connect(this, &InputDevice::propertyAdded,
            this, [this](const QString &) { Q_EMIT supportedPropertiesChanged(); });
    connect(this, &InputDevice::propertyRemoved,
            this, [this](const QString &) { Q_EMIT supportedPropertiesChanged(); });
}

InputDevice::~InputDevice()
{
}
