#pragma once

#include "device.h"

class XInputDevice;

class XInputDeviceAdapter : public InputDevice
{
    Q_OBJECT

public:
    XInputDeviceAdapter(XInputDevice *impl, QObject *parent);
    ~XInputDeviceAdapter() Q_DECL_OVERRIDE;

    XInputDevice *device() const
    {
        return impl;
    }

    QString name() const Q_DECL_OVERRIDE;
    QString identifier() const Q_DECL_OVERRIDE;

    QVariant deviceProperty(const QString &name) const Q_DECL_OVERRIDE;
    bool setDeviceProperty(const QString &name, const QVariant &value) Q_DECL_OVERRIDE;

    QStringList supportedProperties() const Q_DECL_OVERRIDE;
    bool isPropertyWritable(const QString &name) const Q_DECL_OVERRIDE;

private:
    void handlePropertyAdded(const QByteArray &);
    void handlePropertyRemoved(const QByteArray &);
    void handlePropertyChanged(const QByteArray &);

    XInputDevice *impl;

    QStringList supported_;
};
