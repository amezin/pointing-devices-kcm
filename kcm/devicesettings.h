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
    bool differsFromActive() const;
    bool savedDiffersFromActive() const;

    void loadSaved();
    void loadDefaults();
    void loadActive();

    void apply();
    void save();
    void applySaved();

    QVariant value(const QString &) const;
    void setValue(const QString &, const QVariant &);

Q_SIGNALS:
    void needsSaveChanged();
    void differsFromActiveChanged();
    void savedDiffersFromActiveChanged();
    void changed();
    void deviceDisconnected();

private:
    void setNeedsSave(bool);
    void setDiffersFromActive(bool);
    void setSavedDiffersFromActive(bool);

    void addProperty(const QString &name);
    void removeProperty(const QString &name);
    void updateProperty(const QString &name);

    QVariant savedValue(const QString &name) const;
    QVariant defaultValue(const QString &name) const;
    QVariant deviceValue(const QString &name) const;
    static QVariant fixupType(const QVariant &, const QVariant &);

    bool setValueNoSignal(const QString &, const QVariant &);
    void updateNeedsSave();
    void updateDiffersFromActive();
    void updateSavedDiffersFromActive();

    QPointer<InputDevice> device_;
    KConfigGroup configGroup_, defaultsGroup_;
    QVariantHash values_, readonly_;
    bool needsSave_, differsFromActive_, savedDiffersFromActive_;
};
