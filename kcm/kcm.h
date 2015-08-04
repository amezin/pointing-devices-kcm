#pragma once

#include <KConfig>
#include <KQuickAddons/ConfigModule>

#include "devicelistmodel.h"

class InputDeviceManager;
class InputDevice;

class PointingDevicesKCM : public KQuickAddons::ConfigModule
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* deviceList READ deviceList CONSTANT)
    Q_PROPERTY(bool outOfSync READ isOutOfSync NOTIFY outOfSyncChanged)
public:
    explicit PointingDevicesKCM(QObject *parent, const QVariantList &args);
    ~PointingDevicesKCM() Q_DECL_OVERRIDE;

    DeviceListModel *deviceList() const
    {
        return deviceList_;
    }

    bool isOutOfSync() const
    {
        return outOfSync_;
    }

    void load() Q_DECL_OVERRIDE;
    void defaults() Q_DECL_OVERRIDE;
    void save() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void outOfSyncChanged(bool outOfSync);

private:
    void addDevice(InputDevice *);
    void removeDevice(InputDevice *);

    void updateNeedsSave();
    void updateOutOfSyncStatus();

    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
    DeviceListModel *deviceList_;
    bool outOfSync_;
};
