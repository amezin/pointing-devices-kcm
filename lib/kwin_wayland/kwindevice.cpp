#include "kwindevice.h"

#include <QMetaObject>
#include <QMetaProperty>

#include "deviceinterface.h"
#include "kwinproperty.h"

KWinDevice::KWinDevice(OrgKdeKwinInputDeviceInterface *device, QObject *parent)
    : InputDevice(parent), device_(device)
{
    device_->setParent(this);

    addProp("libinput Tapping Enabled",
            new KWinBoolProperty(device_, "tapEnabled", "tapFingerCount", this));
    addProp("libinput Tapping Enabled Default",
            new KWinBoolProperty(device_, "tapEnabledDefault", "tapFingerCount", this));

    addProp("libinput Tapping Drag Lock Enabled",
            new KWinBoolProperty(device_, "tapDragLockEnabled", "tapFingerCount", this));
    addProp("libinput Tapping Drag Lock Enabled Default",
            new KWinBoolProperty(device_, "tapDragLockEnabledDefault", "tapFingerCount", this));

    addProp("libinput Accel Speed",
            new KWinProperty(device_, "accelSpeed", "accelAvailable", this));
    addProp("libinput Accel Speed Default",
            new KWinProperty(device_, "accelSpeedDefault", "accelAvailable", this));

    addProp("libinput Natural Scrolling Enabled",
            new KWinBoolProperty(device_, "scrollNaturalScrollEnabled", "scrollHasNaturalScroll", this));
    addProp("libinput Natural Scrolling Enabled Default",
            new KWinBoolProperty(device_, "scrollNaturalScrollEnabledDefault", "scrollHasNaturalScroll", this));

    addProp("libinput Left Handed Enabled",
            new KWinBoolProperty(device_, "leftHanded", "leftHandedAvailable", this));
    addProp("libinput Left Handed Enabled Default",
            new KWinBoolProperty(device_, "leftHandedDefault", "leftHandedAvailable", this));

    addProp("libinput Middle Emulation Enabled",
            new KWinBoolProperty(device_, "middleEmulationEnabled", "middleEmulationAvailable", this));
    addProp("libinput Middle Emulation Enabled Default",
            new KWinBoolProperty(device_, "middleEmulationEnabledDefault", "middleEmulationAvailable", this));

    addProp("libinput Disable While Typing Enabled",
            new KWinBoolProperty(device_, "dwtEnabled", "dwtAvailable", this));
    addProp("libinput Disable While Typing Enabled Default",
            new KWinBoolProperty(device_, "dwtEnabledDefault", "dwtAvailable", this));

    addProp("libinput Send Events Modes Available",
            new KWinBitsetProperty(device_, "sendEventsModes", "", 2, this));
    addProp("libinput Send Events Mode Enabled",
            new KWinBitsetProperty(device_, "sendEventsMode", "sendEventsModes", 2, this));
    addProp("libinput Send Events Mode Enabled Default",
            new KWinBitsetProperty(device_, "sendEventsModeDefault", "sendEventsModes", 2, this));

    addProp("libinput Scroll Methods Available",
            new KWinBitsetProperty(device_, "scrollMethods", "", 3, this));
    addProp("libinput Scroll Method Enabled",
            new KWinSingleBitProperty(device_, "scrollMethod", "scrollMethods", 3, this));
    addProp("libinput Scroll Method Enabled Default",
            new KWinSingleBitProperty(device_, "scrollMethodDefault", "scrollMethods", 3, this));

    addProp("libinput Click Methods Available",
            new KWinBitsetProperty(device_, "clickMethods", "", 2, this));
    addProp("libinput Click Method Enabled",
            new KWinSingleBitProperty(device_, "clickMethod", "clickMethods", 2, this));
    addProp("libinput Click Method Enabled Default",
            new KWinSingleBitProperty(device_, "clickMethodDefault", "clickMethods", 2, this));
}

KWinDevice::~KWinDevice()
{
}

QDBusObjectPath KWinDevice::path() const
{
    return QDBusObjectPath(device_->path());
}

QString KWinDevice::name() const
{
    return device_->name();
}

QString KWinDevice::identifier() const
{
    QStringList id;
    id.append(name());
    id.append(QString::number(device_->vendorId()));
    id.append(QString::number(device_->productId()));
    return id.join(':');
}

QVariant KWinDevice::deviceProperty(const QString &name) const
{
    auto p = props_.value(name, Q_NULLPTR);
    return p ? p->value() : QVariant();
}

QStringList KWinDevice::supportedProperties() const
{
    return supported_;
}

bool KWinDevice::isPropertyWritable(const QString &name) const
{
    auto p = props_.value(name, Q_NULLPTR);
    return p ? p->isWritable() : false;
}

QVariant KWinDevice::defaultValue(const QString &prop) const
{
    return deviceProperty(prop + QStringLiteral(" Default"));
}

bool KWinDevice::setProperties(const QVariantHash &props)
{
    for (auto i = props.constBegin(); i != props.constEnd(); i++) {
        auto p = props_.value(i.key(), Q_NULLPTR);
        if (!p) {
            return false;
        }
        if (!p->setValue(i.value())) {
            return false;
        }
    }
    return true;
}

void KWinDevice::updatePropAvailability(const QString &name, KWinProperty *p)
{
    auto index = supported_.indexOf(name);
    if ((index >= 0) == p->isAvailable()) {
        return;
    }
    if (p->isAvailable()) {
        supported_.append(name);
        Q_EMIT propertyAdded(name);
    } else {
        supported_.removeAt(index);
        Q_EMIT propertyRemoved(name);
    }
}

void KWinDevice::addProp(const QString &name, KWinProperty *p)
{
    props_.insert(name, p);
    connect(p, &KWinProperty::availabilityChanged,
            this, [=]() { updatePropAvailability(name, p); });
    connect(p, &KWinProperty::valueChanged,
            this, [=]() { Q_EMIT propertyChanged(name); });
    updatePropAvailability(name, p);
}
