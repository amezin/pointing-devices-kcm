#pragma once

#include <KConfig>
#include <KQuickAddons/ConfigModule>

class DeviceListModel;
class InputDeviceManager;
class InputDevice;

class PointingDevicesKCM : public KQuickAddons::ConfigModule
{
    Q_OBJECT

    Q_PROPERTY(DeviceListModel* deviceList READ deviceList CONSTANT)
public:
    explicit PointingDevicesKCM(QObject *parent, const QVariantList &args);
    ~PointingDevicesKCM() Q_DECL_OVERRIDE;

    DeviceListModel *deviceList() const
    {
        return deviceList_;
    }

    void load() Q_DECL_OVERRIDE;
    void defaults() Q_DECL_OVERRIDE;
    void save() Q_DECL_OVERRIDE;

private:
    void addDevice(InputDevice *);
    void removeDevice(InputDevice *);

    void updateNeedsSave();

    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
    DeviceListModel *deviceList_;
};
