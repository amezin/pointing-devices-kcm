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
    Q_PROPERTY(bool canTest READ canTest NOTIFY canTestChanged)
    Q_PROPERTY(bool canRevertTest READ canRevertTest NOTIFY canRevertTestChanged)
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

    bool canTest() const;
    bool canRevertTest() const;

public Q_SLOTS:
    void test();
    void revertTest();

Q_SIGNALS:
    void canTestChanged();
    void canRevertTestChanged();

private:
    void setCanTest(bool);
    void setCanRevertTest(bool);

    void addDevice(InputDevice *);
    void removeDevice(InputDevice *);

    void updateNeedsSave();
    void updateCanTest();
    void updateCanRevertTest();

    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
    DeviceListModel *deviceList_;
    bool canTest_, canRevertTest_;
};
