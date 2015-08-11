#pragma once

#include <QObject>
#include <QMetaProperty>

class KWinProperty : public QObject
{
    Q_OBJECT
public:
    KWinProperty(QObject *iface, const char *name, const char *avail,
                 QObject *parent);
    ~KWinProperty() Q_DECL_OVERRIDE;

    bool isAvailable() const;
    bool isWritable() const;

    virtual QVariant value() const;
    virtual bool setValue(const QVariant &);

public Q_SLOTS:
    void update();

Q_SIGNALS:
    void valueChanged();
    void availabilityChanged();

private:
    void setAvailable(bool);
    bool setInternalValue(const QVariant &);
    void watchForChanges(const char *prop);

    QObject *obj_;
    QMetaProperty prop_, avail_;
    bool available_, hasAvail_;
    QVariant value_;
};

class KWinBoolProperty : public KWinProperty
{
    Q_OBJECT
public:
    KWinBoolProperty(QObject *iface, const char *name, const char *avail,
                     QObject *parent);
    ~KWinBoolProperty() Q_DECL_OVERRIDE;

    QVariant value() const Q_DECL_OVERRIDE;
    bool setValue(const QVariant &v) Q_DECL_OVERRIDE;
};

class KWinBitsetProperty : public KWinProperty
{
    Q_OBJECT
public:
    KWinBitsetProperty(QObject *iface, const char *name, const char *avail,
                       int numBits, QObject *parent);
    ~KWinBitsetProperty() Q_DECL_OVERRIDE;

    QVariant value() const Q_DECL_OVERRIDE;
    bool setValue(const QVariant &v) Q_DECL_OVERRIDE;
private:
    int numBits_;
};

class KWinSingleBitProperty : public KWinProperty
{
    Q_OBJECT
public:
    KWinSingleBitProperty(QObject *iface, const char *name, const char *avail,
                          int numBits, QObject *parent);
    ~KWinSingleBitProperty() Q_DECL_OVERRIDE;

    QVariant value() const Q_DECL_OVERRIDE;
    bool setValue(const QVariant &v) Q_DECL_OVERRIDE;
private:
    int numBits_;
};
