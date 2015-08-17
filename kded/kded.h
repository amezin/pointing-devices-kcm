#pragma once

#include <KConfig>
#include <KDEDModule>
#include <KPluginFactory>

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
    static QVariant fixupType(const QVariant &value, const QVariant &pattern);

    void applyConfig(InputDevice *);
    bool applyProperties(InputDevice *, const QStringList &props,
                         KConfigGroup &group, KConfigGroup &defaultsGroup);
    void reapplyConfig(const QString &prop);

    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
};
