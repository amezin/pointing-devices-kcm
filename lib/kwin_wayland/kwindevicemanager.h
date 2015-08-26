#pragma once

#include "devicemanager.h"

class QDBusObjectPath;
class OrgKdeKWinInputDeviceManagerInterface;
class KWinDevice;

class KWinDeviceManager : public InputDeviceManager
{
    Q_OBJECT
public:
    KWinDeviceManager(QObject *parent, const QVariantList&);
    ~KWinDeviceManager() Q_DECL_OVERRIDE;

    QList<InputDevice *> devices() const Q_DECL_OVERRIDE;

private:
    void addDevice(const QDBusObjectPath &);
    void removeDevice(const QDBusObjectPath &);

    OrgKdeKWinInputDeviceManagerInterface *interface_;
    QList<KWinDevice *> devices_;
};
