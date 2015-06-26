#pragma once

#include <QObject>

#include "pointing-devices-common_export.h"

class InputDevice;

#define InputDeviceManager_IID "org.kde.pointingdevices.devicemanager"

class POINTING_DEVICES_COMMON_EXPORT InputDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit InputDeviceManager(QObject *parent);
    ~InputDeviceManager() Q_DECL_OVERRIDE;

    virtual QList<InputDevice *> devices() const = 0;

    InputDevice *deviceByIdentifier(const QString &) const;

    static InputDeviceManager *instance();

Q_SIGNALS:
    void deviceAdded(InputDevice *device);
    void deviceRemoved(InputDevice *device);

private:
    static InputDeviceManager *create();
};
