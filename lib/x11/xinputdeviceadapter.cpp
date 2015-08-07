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

    static const auto xi = QStringLiteral("xinput");
    id.push_back(xi);

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
    Q_EMIT propertyAdded(name);
    Q_EMIT supportedPropertiesChanged();
}

void XInputDeviceAdapter::handlePropertyRemoved(const QByteArray &prop)
{
    auto name = QString::fromLatin1(prop);
    if (!supported_.removeAll(name)) {
        return;
    }
    Q_EMIT propertyRemoved(name);
    Q_EMIT supportedPropertiesChanged();
}

void XInputDeviceAdapter::handlePropertyChanged(const QByteArray &prop)
{
    Q_EMIT propertyChanged(QString::fromLatin1(prop));
}

QVariant XInputDeviceAdapter::deviceProperty(const QString &name) const
{
    return impl->deviceProperty(name.toLatin1());
}

bool XInputDeviceAdapter::setDeviceProperty(const QString &name, const QVariant &value)
{
    return impl->setDeviceProperty(name.toLatin1(), value);
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
    if (!name.startsWith(QLatin1String("libinput "))) {
        return true;
    }
    return !name.endsWith(QLatin1String(" Default")) &&
            !name.endsWith(QLatin1String(" Available"));
}
