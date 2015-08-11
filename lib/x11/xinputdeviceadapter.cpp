#include "xinputdeviceadapter.h"

#include <algorithm>

#include <QMetaObject>
#include <QMetaProperty>

#include "xinputdevice.h"
#include "log.h"

XInputDeviceAdapter::XInputDeviceAdapter(XInputDevice *impl, QObject *parent)
    : InputDevice(parent), impl(impl)
{
    connect(impl, &XInputDevice::devicePropertyAdded,
            this, &XInputDeviceAdapter::handlePropertyAdded);
    connect(impl, &XInputDevice::devicePropertyRemoved,
            this, &XInputDeviceAdapter::handlePropertyRemoved);
    connect(impl, &XInputDevice::devicePropertyChanged,
            this, &XInputDeviceAdapter::handlePropertyChanged);

    Q_FOREACH (const auto &prop, impl->devicePropertyNames()) {
        if (impl->devicePropertyExists(prop)) {
            supported_.append(QString::fromLatin1(prop));
        }
    }
}

XInputDeviceAdapter::~XInputDeviceAdapter()
{
}

QString XInputDeviceAdapter::name() const
{
    return impl->name();
}

QString XInputDeviceAdapter::identifier() const
{
    static const auto name = QByteArrayLiteral("Device Product ID");
    auto id = impl->deviceProperty(name).toStringList();
    id.push_front(this->name());
    return id.join(':');
}

QStringList XInputDeviceAdapter::supportedProperties() const
{
    return supported_;
}

void XInputDeviceAdapter::handlePropertyAdded(const QByteArray &prop)
{
    auto name = QString::fromLatin1(prop);
    if (supported_.contains(name)) {
        return;
    }
    supported_.append(name);
    QMetaObject::invokeMethod(this, "delayedEmitPropertyAdded", Qt::QueuedConnection,
                              Q_ARG(QString, name));
}

void XInputDeviceAdapter::delayedEmitPropertyAdded(const QString &prop)
{
    if (!supported_.contains(prop)) {
        return;
    }
    Q_EMIT propertyAdded(prop);
}

void XInputDeviceAdapter::handlePropertyRemoved(const QByteArray &prop)
{
    auto name = QString::fromLatin1(prop);
    if (!supported_.removeAll(name)) {
        return;
    }
    Q_EMIT propertyRemoved(name);
}

void XInputDeviceAdapter::handlePropertyChanged(const QByteArray &prop)
{
    Q_EMIT propertyChanged(QString::fromLatin1(prop));
}

QVariant XInputDeviceAdapter::deviceProperty(const QString &name) const
{
    return impl->deviceProperty(name.toLatin1());
}

bool XInputDeviceAdapter::setProperties(const QVariantHash &props)
{
    static const QString enabledStr(QStringLiteral("Device Enabled"));
    static const QByteArray enabledBa(QByteArrayLiteral("Device Enabled"));
    if (props.value(enabledStr, false) == true) {
        if (!impl->setDeviceProperty(enabledBa, true)) {
            return false;
        }
    }
    for (auto i = props.constBegin(); i != props.constEnd(); ++i) {
        if (i.key() != enabledStr) {
            if (!impl->setDeviceProperty(i.key().toLatin1(), i.value())) {
                return false;
            }
        }
    }
    if (props.value(enabledStr, true) == false) {
        if (!impl->setDeviceProperty(enabledBa, false)) {
            return false;
        }
    }
    return impl->flush();
}

bool XInputDeviceAdapter::isPropertyWritable(const QString &name) const
{
    static const QStringList readOnly = { QStringLiteral("Device Node"),
                                          QStringLiteral("Device Product ID"),
                                          QStringLiteral("Axis Labels"),
                                          QStringLiteral("Button Labels") };
    if (readOnly.contains(name)) {
        return false;
    }
    if (name.startsWith(QLatin1String("libinput "))) {
        return !name.endsWith(QLatin1String(" Default")) &&
               !name.endsWith(QLatin1String(" Available"));
    }
    return true;
}

QVariant XInputDeviceAdapter::defaultValue(const QString &prop) const
{
#if 0
    /*
     * We can take default values from libinput. But Xorg.conf will be mostly
     * ignored then.
     */
    if (prop.startsWith(QLatin1String("libinput "))) {
        auto defaultName = prop + QStringLiteral(" Default");
        if (supported_.contains(defaultName)) {
            return deviceProperty(defaultName);
        }
    }
#endif
    return QVariant();
}
