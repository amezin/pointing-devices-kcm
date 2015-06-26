#pragma once

#include "device.h"

class XInputDevice;

class XInputDeviceAdapter : public InputDevice
{
    Q_OBJECT
public:
    XInputDeviceAdapter(XInputDevice *impl, QObject *parent);
    ~XInputDeviceAdapter() Q_DECL_OVERRIDE;

    QByteArrayList supportedProperties() const Q_DECL_OVERRIDE;

    QString name() const Q_DECL_OVERRIDE;
    QString identifier() const Q_DECL_OVERRIDE;

    bool isEnabled() const Q_DECL_OVERRIDE;
    void setEnabled(bool) Q_DECL_OVERRIDE;

    XInputDevice *device() const
    {
        return impl;
    }

private:
    void handlePropertyAdd(const QByteArray &);
    void handlePropertyRemove(const QByteArray &);
    void handlePropertyChange(const QByteArray &);

    XInputDevice *impl;
};
