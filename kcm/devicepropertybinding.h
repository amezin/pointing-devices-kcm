#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlProperty>
#include <QQmlPropertyValueSource>

#include "devicesettings.h"

class DevicePropertyBinding : public QObject, public QQmlPropertyValueSource
{
    Q_OBJECT
    Q_INTERFACES(QQmlPropertyValueSource)

    Q_PROPERTY(QObject* device READ device WRITE setDevice NOTIFY deviceChanged)
    Q_PROPERTY(QString deviceProperty READ deviceProperty WRITE setDeviceProperty NOTIFY devicePropertyChanged)
public:
    explicit DevicePropertyBinding(QObject *parent = Q_NULLPTR);
    ~DevicePropertyBinding() Q_DECL_OVERRIDE;

    DeviceSettings *device() const
    {
        return device_;
    }

    QString deviceProperty() const
    {
        return deviceProperty_;
    }

    void setDevice(QObject *);
    void setDevice(DeviceSettings *);
    void setDeviceProperty(const QString &);

    void setTarget(const QQmlProperty &) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void deviceChanged(QObject *device);
    void devicePropertyChanged(const QString &deviceProperty);

private Q_SLOTS:
    void devToTarget();
    void targetToDev();

private:
    void update();

    QPointer<DeviceSettings> device_, prevDevice_;
    QPointer<QObject> prevTarget_;
    QString deviceProperty_;
    QQmlProperty deviceProp_, target_;
    bool prevEnabled_;
};
