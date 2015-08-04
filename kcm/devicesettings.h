#pragma once

#include <QPointer>
#include <QQmlPropertyMap>
#include <KConfigGroup>

#include "device.h"

class DeviceSettings : public QQmlPropertyMap
{
    Q_OBJECT

    Q_PROPERTY(QObject *device READ device NOTIFY deviceDestroyed)
    Q_PROPERTY(QStringList supportedProperties READ supportedProperties NOTIFY supportedPropertiesChanged)

    Q_PROPERTY(bool needsSave READ needsSave NOTIFY needsSaveChanged)
    Q_PROPERTY(bool differsFromDefaults READ differsFromDefaults NOTIFY differsFromDefaultsChanged)
    Q_PROPERTY(bool differsFromActive READ differsFromActive NOTIFY differsFromActiveChanged)
public:
    DeviceSettings(KConfig *config, KConfig *defaults, InputDevice *,
                   QObject *parent = Q_NULLPTR);
    ~DeviceSettings() Q_DECL_OVERRIDE;

    InputDevice *device() const;

    bool needsSave() const;
    bool differsFromDefaults() const;
    bool differsFromActive() const;

    void loadSaved();
    void loadDefaults();
    void loadActive();

    void apply(bool updateDiff = true);
    void save(bool updateDiff = true);

    QStringList supportedProperties() const;

Q_SIGNALS:
    void deviceDestroyed();
    void supportedPropertiesChanged();

    void needsSaveChanged();
    void differsFromActiveChanged();
    void differsFromDefaultsChanged();

protected:
    QVariant updateValue(const QString &key, const QVariant &input) Q_DECL_OVERRIDE;

private:
    QVariant activeValue(const QString &) const;
    QVariant savedValue(const QString &) const;
    QVariant defaultValue(const QString &) const;

    void setNeedsSave(const QStringList &);
    void setDiffersFromDefaults(const QStringList &);
    void setDiffersFromActive(const QStringList &);

    typedef QVariant (DeviceSettings::*Getter)(const QString &) const;
    QStringList differsFrom(Getter, const QStringList &) const;
    void updateDiffs(const QStringList &, bool discardOld = false);
    void handlePropertyChange(const QString &, const QVariant &);

    void load(Getter, bool overwrite = true);
    void load(Getter, const QStringList &, bool overwrite = true);
    void updatePropertySet();
    void clearUnsupportedProperties();

    QPointer<InputDevice> device_;
    KConfigGroup configGroup_, defaultsGroup_;
    QStringList needsSave_, differsFromActive_, differsFromDefaults_;
};
