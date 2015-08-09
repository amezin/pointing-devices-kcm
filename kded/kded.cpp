#include "kded.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QTimer>

#include <KConfig>
#include <KConfigGroup>

#include "devicemanager.h"
#include "device.h"
#include "kdedadaptor.h"

K_PLUGIN_FACTORY_WITH_JSON(PointingDevicesKDEDFactory, "kded_pointingdevices.json", registerPlugin<PointingDevicesKDED>();)

PointingDevicesKDED::PointingDevicesKDED(QObject *parent, const QVariantList &)
    : KDEDModule(parent),
      deviceManager_(InputDeviceManager::create(this)),
      config_(QStringLiteral("pointingdevicesrc")),
      defaults_(QStringLiteral("pointingdevicesdefaultsrc"))
{
    connect(deviceManager_, &InputDeviceManager::deviceAdded,
            this, [this](InputDevice *device)
            {
                config_.reparseConfiguration();
                PointingDevicesKDED::applyConfig(device);
            });

    Q_FOREACH (auto device, deviceManager_->devices()) {
        applyConfig(device);
    }

    new PointingDevicesAdaptor(this);
}

PointingDevicesKDED::~PointingDevicesKDED()
{
}

void PointingDevicesKDED::applyConfig(InputDevice *device)
{
    Q_ASSERT(device);

    connect(device, &InputDevice::propertyAdded,
            this, &PointingDevicesKDED::reapplyConfig, Qt::UniqueConnection);

    config_.reparseConfiguration();
    auto group = config_.group(device->identifier());
    auto defaultsGroup = defaults_.group(device->identifier());

    applyProperties(device, device->supportedProperties(), group, defaultsGroup);

    defaultsGroup.sync();
}

QVariant PointingDevicesKDED::fixupType(const QVariant &value, const QVariant &pattern)
{
    if (!value.isValid()) {
        return value;
    }

    QVariant converted(value);
    converted.convert(pattern.userType());

    if (converted.userType() != QMetaType::QVariantList &&
            converted.userType() != QMetaType::QStringList)
    {
        return converted;
    }

    QVariantList list(converted.toList());
    QVariantList listPattern(pattern.toList());

    QVariantList listConverted;
    listConverted.reserve(listPattern.size());
    for (int i = 0; i < listPattern.size(); i++) {
        listConverted.append(fixupType(list.value(i), listPattern.at(i)));
    }

    return listConverted;
}

bool PointingDevicesKDED::applyProperties(InputDevice *device, const QStringList &props,
                                          KConfigGroup &group, KConfigGroup &defaultsGroup)
{
    QVariantHash changes;
    Q_FOREACH (const auto &prop, props) {
        if (!device->isPropertyWritable(prop)) {
            continue;
        }

        auto currentValue = device->deviceProperty(prop);
        if (!currentValue.isValid()) {
            continue;
        }

        auto defaultValue = device->defaultValue(prop);
        if (!defaultValue.isValid()) {
            if (!defaultsGroup.hasKey(prop)) {
                defaultsGroup.writeEntry(prop, currentValue);
            }
            defaultValue = fixupType(defaultsGroup.readEntry(prop, currentValue), currentValue);
        }
        auto newValue = fixupType(group.readEntry(prop, defaultValue), currentValue);

        if (newValue != currentValue) {
            changes.insert(prop, newValue);
        }
    }
    return device->setProperties(changes);
}

void PointingDevicesKDED::reapplyConfig(const QString &prop)
{
    auto device = qobject_cast<InputDevice *>(sender());
    Q_ASSERT(device);

    config_.reparseConfiguration();
    auto group = config_.group(device->identifier());
    auto defaultsGroup = defaults_.group(device->identifier());
    applyProperties(device, QStringList(prop), group, defaultsGroup);
    defaultsGroup.sync();
}

void PointingDevicesKDED::reloadConfig()
{
    config_.reparseConfiguration();
    Q_FOREACH (auto device, deviceManager_->devices()) {
        applyConfig(device);
    }
}

void PointingDevicesKDED::reloadConfig(const QString &identifier)
{
    config_.reparseConfiguration();
    auto device = deviceManager_->deviceByIdentifier(identifier);
    applyConfig(device);
}

#include "kded.moc"
