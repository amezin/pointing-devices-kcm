#pragma once

#include <QObjectList>
#include <KConfig>
#include <KQuickAddons/ConfigModule>

#include "devicesettings.h"

class InputDeviceManager;

class PointingDevicesKCM : public KQuickAddons::ConfigModule
{
    Q_OBJECT

public:
    explicit PointingDevicesKCM(QObject *parent, const QVariantList &args);
    ~PointingDevicesKCM() Q_DECL_OVERRIDE;

    void load() Q_DECL_OVERRIDE;
    void defaults() Q_DECL_OVERRIDE;
    void save() Q_DECL_OVERRIDE;

    InputDeviceManager *deviceManager() const;

    Q_INVOKABLE DeviceSettings *settingsFor(InputDevice *,
                                            QObject *parent = Q_NULLPTR);
    Q_INVOKABLE QList<QObject*> allDevices() const;

Q_SIGNALS:
    void loadRequested();
    void saveRequested();
    void defaultsRequested();

    void deviceAdded(InputDevice *device);
    void deviceRemoved(InputDevice *device);

private:
    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
};
