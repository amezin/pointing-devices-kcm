#include "kwinproperty.h"

KWinProperty::KWinProperty(QObject *iface, const char *name, const char *avail,
                           QObject *parent)
    : QObject(parent), obj_(iface), available_(false), hasAvail_(avail && *avail)
{
    auto meta = iface->metaObject();

    auto propIndex = meta->indexOfProperty(name);
    if (propIndex >= 0) {
        prop_ = meta->property(propIndex);
        watchForChanges(name);
    }

    if (hasAvail_) {
        auto availIndex = meta->indexOfProperty(avail);
        if (availIndex >= 0) {
            avail_ = meta->property(availIndex);
            watchForChanges(avail);
        }
    }

    update();
}

KWinProperty::~KWinProperty()
{
}

void KWinProperty::watchForChanges(const char *prop)
{
    QByteArray signalName(prop);
    signalName.append("Changed");

    auto meta = obj_->metaObject();
    auto signalIndex = meta->indexOfSignal(signalName.constData());
    if (signalIndex >= 0) {
        connect(obj_, meta->method(signalIndex),
                this, metaObject()->method(metaObject()->indexOfSlot("update")));
    }
}

bool KWinProperty::isAvailable() const
{
    return available_;
}

QVariant KWinProperty::value() const
{
    return value_;
}

bool KWinProperty::isWritable() const
{
    return available_ && prop_.isWritable();
}

bool KWinProperty::setInternalValue(const QVariant &v)
{
    if (isWritable() && prop_.read(obj_) != v) {
        if (!prop_.write(obj_, v)) {
            return false;
        }
    }
    if (value_ != v) {
        value_ = v;
        Q_EMIT valueChanged();
    }
    return true;
}

bool KWinProperty::setValue(const QVariant &v)
{
    return setInternalValue(v);
}

void KWinProperty::setAvailable(bool v)
{
    if (available_ != v) {
        available_ = v;
        Q_EMIT availabilityChanged();
    }
}

void KWinProperty::update()
{
    if (!prop_.isValid()) {
        setAvailable(false);
        return;
    }
    if (hasAvail_) {
        if (!avail_.isValid()) {
            setAvailable(false);
            return;
        }
        if (!avail_.read(obj_).toBool()) {
            setAvailable(false);
            return;
        }
    }

    setInternalValue(prop_.read(obj_));
    setAvailable(true);
}

KWinBoolProperty::KWinBoolProperty(QObject *iface, const char *name, const char *avail,
                                   QObject *parent)
    : KWinProperty(iface, name, avail, parent)
{
}

KWinBoolProperty::~KWinBoolProperty()
{
}

QVariant KWinBoolProperty::value() const
{
    return KWinProperty::value().toBool();
}

bool KWinBoolProperty::setValue(const QVariant &v)
{
    return KWinProperty::setValue(v.toBool());
}

KWinBitsetProperty::KWinBitsetProperty(QObject *iface, const char *name,
                                       const char *avail, int numBits,
                                       QObject *parent)
    : KWinProperty(iface, name, avail, parent), numBits_(numBits)
{
}

KWinBitsetProperty::~KWinBitsetProperty()
{
}

QVariant KWinBitsetProperty::value() const
{
    QVariantList vl;
    vl.reserve(numBits_);
    auto internal = KWinProperty::value().toULongLong();
    for (int i = 0; i < numBits_; i++) {
        vl.append((internal & (1ULL << i)) != 0);
    }
    return vl;
}

bool KWinBitsetProperty::setValue(const QVariant &v)
{
    auto vl = v.toList();
    auto bits = 0ULL;
    for (int i = 0; i < numBits_ && i < vl.size(); i++) {
        if (vl[i].toBool()) {
            bits |= (1ULL << i);
        }
    }
    return KWinProperty::setValue(vl);
}

KWinSingleBitProperty::KWinSingleBitProperty(QObject *iface, const char *name,
                                             const char *avail, int numBits,
                                             QObject *parent)
    : KWinProperty(iface, name, avail, parent), numBits_(numBits)
{
}

KWinSingleBitProperty::~KWinSingleBitProperty()
{
}

QVariant KWinSingleBitProperty::value() const
{
    auto internal = KWinProperty::value().toInt();
    QVariantList vl;
    vl.reserve(numBits_);
    for (int i = 0; i < numBits_; i++) {
        vl.append(internal == i + 1);
    }
    return vl;
}

bool KWinSingleBitProperty::setValue(const QVariant &v)
{
    auto vl = v.toList();
    for (int i = 0; i < numBits_ && i < vl.size(); i++) {
        if (vl[i].toBool()) {
            return KWinProperty::setValue(i + 1);
        }
    }
    return KWinProperty::setValue(0);
}
