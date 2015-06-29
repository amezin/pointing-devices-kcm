#pragma once

#include <QHash>
#include <QByteArray>

#include "device.h"

class XInputDevice;

class XInputDeviceAdapter : public InputDevice
{
    Q_OBJECT

    Q_PROPERTY(XAccelerationProfile accelProfile READ accelProfile WRITE setAccelProfile NOTIFY accelProfileChanged)
    Q_PROPERTY(float accelConstantDeceleration READ accelConstantDeceleration WRITE setAccelConstantDeceleration NOTIFY accelConstantDecelerationChanged)
    Q_PROPERTY(float accelAdaptiveDeceleration READ accelAdaptiveDeceleration WRITE setAccelAdaptiveDeceleration NOTIFY accelAdaptiveDecelerationChanged)
    Q_PROPERTY(float accelVelocityScaling READ accelVelocityScaling WRITE setAccelVelocityScaling NOTIFY accelVelocityScalingChanged)

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

    XAccelerationProfile accelProfile() const;
    void setAccelProfile(XAccelerationProfile);

    float accelConstantDeceleration() const;
    void setAccelConstantDeceleration(float);

    float accelAdaptiveDeceleration() const;
    void setAccelAdaptiveDeceleration(float);

    float accelVelocityScaling() const;
    void setAccelVelocityScaling(float);

Q_SIGNALS:
    void accelProfileChanged();
    void accelConstantDecelerationChanged();
    void accelAdaptiveDecelerationChanged();
    void accelVelocityScalingChanged();

private:
    void handlePropertyAdd(const QByteArray &);
    void handlePropertyRemove(const QByteArray &);
    void handlePropertyChange(const QByteArray &);

    XInputDevice *impl;
    QHash<QByteArray, QByteArrayList> requiredDevProperties_;
    QHash<QByteArray, QByteArrayList> notifyOnChange_;
};
