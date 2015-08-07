#pragma once

#include <QHash>
#include <QPointer>
#include <QTimer>
#include <KConfigGroup>

#include "device.h"

class DeviceProperty;

class DeviceSettings : public QObject
{
    Q_OBJECT
public:
    DeviceSettings(KConfig *config, KConfig *defaults, InputDevice *,
                   QObject *parent = Q_NULLPTR);
    ~DeviceSettings() Q_DECL_OVERRIDE;

    InputDevice *device() const;
    bool isPropertySupported(const QString &) const;
    bool isPropertyWritable(const QString &) const;

    bool needsSave() const;

    void loadSaved();
    void loadDefaults();
    void loadActive();

    void apply();
    void save();

    QVariant value(const QString &) const;
    void setValue(const QString &, const QVariant &);

Q_SIGNALS:
    void needsSaveChanged();
    void changed();

private:
    void setNeedsSave(bool);

    void addProperty(const QString &name);
    void removeProperty(const QString &name);
    void updateProperty(const QString &name);

    QVariant savedValue(const QString &name) const;
    QVariant defaultValue(const QString &name) const;
    QVariant deviceValue(const QString &name) const;
    QVariant fixupType(const QVariant &, const QVariant &) const;

    bool setValueNoSignal(const QString &, const QVariant &);
    void updateStatus();

    QPointer<InputDevice> device_;
    KConfigGroup configGroup_, defaultsGroup_;
    QVariantHash values_, readonly_;
    bool needsSave_;
};
