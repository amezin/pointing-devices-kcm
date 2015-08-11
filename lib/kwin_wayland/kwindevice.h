#pragma once

#include <QVariantHash>
#include <QDBusObjectPath>

#include "device.h"

class OrgKdeKwinInputDeviceInterface;
class KWinProperty;

class KWinDevice : public InputDevice
{
    Q_OBJECT
public:
    explicit KWinDevice(OrgKdeKwinInputDeviceInterface *device,
                        QObject *parent = Q_NULLPTR);
    ~KWinDevice() Q_DECL_OVERRIDE;

    OrgKdeKwinInputDeviceInterface *device() const
    {
        return device_;
    }

    QDBusObjectPath path() const;

    QString name() const Q_DECL_OVERRIDE;
    QString identifier() const Q_DECL_OVERRIDE;

    QVariant deviceProperty(const QString &name) const Q_DECL_OVERRIDE;
    bool setProperties(const QVariantHash &) Q_DECL_OVERRIDE;

    QStringList supportedProperties() const Q_DECL_OVERRIDE;
    bool isPropertyWritable(const QString &name) const Q_DECL_OVERRIDE;

    QVariant defaultValue(const QString &prop) const Q_DECL_OVERRIDE;

private:
    void updatePropAvailability(const QString &, KWinProperty *);
    void updatePropValue(const QString &, KWinProperty *);
    void addProp(const QString &, KWinProperty *);

    OrgKdeKwinInputDeviceInterface *device_;
    QHash<QString, KWinProperty *> props_;
    QStringList supported_;
};
