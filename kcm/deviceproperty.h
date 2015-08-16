#pragma once

#include <QObject>

#include "devicesettings.h"

class DeviceProperty : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool available READ isAvailable NOTIFY availableChanged)
    Q_PROPERTY(bool writable READ isWritable NOTIFY writableChanged)
    Q_PROPERTY(DeviceSettings* device READ device WRITE setDevice NOTIFY deviceChanged)
public:
    explicit DeviceProperty(QObject *parent = Q_NULLPTR);
    ~DeviceProperty() Q_DECL_OVERRIDE;

    QString name() const;
    void setName(const QString &);

    QVariant value() const;
    void setValue(const QVariant &);

    bool isAvailable() const;
    bool isWritable() const;

    DeviceSettings *device() const;
    void setDevice(DeviceSettings *);

Q_SIGNALS:
    void nameChanged(const QString &name);
    void valueChanged(const QVariant &value);
    void availableChanged(bool available);
    void writableChanged(bool writable);
    void deviceChanged(DeviceSettings *device);

private:
    void update();
    void handleDeviceDestroyed(QObject *);

    DeviceSettings *device_;
    QString name_;

    QVariant prevValue_;
    bool prevAvailable_, prevWritable_;
};
