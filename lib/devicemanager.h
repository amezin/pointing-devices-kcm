#pragma once

#include <QObject>

#include "pointingdevices_export.h"

class InputDevice;

class POINTINGDEVICES_EXPORT InputDeviceManager : public QObject
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
