#pragma once

#include <QObject>
#include <QStringList>

#include "pointingdevices_export.h"

class POINTINGDEVICES_EXPORT InputDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList supportedProperties READ supportedProperties NOTIFY supportedPropertiesChanged)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString identifier READ identifier CONSTANT)

public:
    explicit InputDevice(QObject *parent);
    ~InputDevice() Q_DECL_OVERRIDE;

    virtual QString name() const = 0;
    virtual QString identifier() const = 0;

    virtual QVariant deviceProperty(const QString &name) const = 0;
    virtual bool setProperties(const QVariantHash &) = 0;

    virtual QStringList supportedProperties() const = 0;
    virtual bool isPropertyWritable(const QString &name) const = 0;

    virtual QVariant defaultValue(const QString &prop) const = 0;

Q_SIGNALS:
    void supportedPropertiesChanged();

    void propertyAdded(const QString &);
    void propertyRemoved(const QString &);
    void propertyChanged(const QString &);
};
