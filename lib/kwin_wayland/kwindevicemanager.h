#pragma once

#include "devicemanager.h"

class QDBusObjectPath;
class OrgKdeKwinInputDeviceManagerInterface;
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

    OrgKdeKwinInputDeviceManagerInterface *interface_;
    QList<KWinDevice *> devices_;
};
