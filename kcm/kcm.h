#pragma once

#include <KConfig>
#include <KQuickAddons/ConfigModule>

class InputDeviceManager;
class InputDevice;

class PointingDevicesKCM : public KQuickAddons::ConfigModule
{
    Q_OBJECT
public:
    explicit PointingDevicesKCM(QObject *parent, const QVariantList &args);
    ~PointingDevicesKCM() Q_DECL_OVERRIDE;

    Q_INVOKABLE QList<QObject *> devices() const;

Q_SIGNALS:
    void deviceAdded(InputDevice *device);
    void deviceRemoved(InputDevice *device);

private:
    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
};

