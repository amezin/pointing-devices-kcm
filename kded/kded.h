#pragma once

#include <KConfig>
#include <KDEDModule>

class KConfig;

class InputDevice;
class InputDeviceManager;

class PointingDevicesKDED : public KDEDModule
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.PointingDevices")
public:
    PointingDevicesKDED(QObject *parent, const QVariantList &);
    ~PointingDevicesKDED() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void reloadConfig();
    void reloadConfig(const QString &device);

private:
    void applyConfig(InputDevice *);
    void reapplyConfig();

    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
};
