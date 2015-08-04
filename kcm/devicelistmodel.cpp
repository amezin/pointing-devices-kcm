#include "devicelistmodel.h"

#include "devicesettings.h"

DeviceListModel::DeviceListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

DeviceListModel::~DeviceListModel()
{
}

void DeviceListModel::add(DeviceSettings *item)
{
    if (items_.contains(item)) {
        return;
    }

    beginInsertRows(QModelIndex(), items_.count(), items_.count());
    items_.append(item);
    endInsertRows();

    if (item->device()) {
        connect(item->device(), &InputDevice::nameChanged,
                this, &DeviceListModel::deviceNameChanged);
    }
}

void DeviceListModel::remove(DeviceSettings *item)
{
    int index = items_.indexOf(item);
    if (index < 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    items_.removeAt(index);
    endRemoveRows();

    if (item->device()) {
        disconnect(item->device(), &InputDevice::nameChanged,
                   this, &DeviceListModel::deviceNameChanged);
    }
}

int DeviceListModel::rowCount(const QModelIndex &) const
{
    return items_.count();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > items_.count()) {
        return QVariant();
    }
    if (role == SettingsRole) {
        return QVariant::fromValue(static_cast<QObject *>(items_[index.row()]));
    }
    if (role == NameRole) {
        auto dev = items_[index.row()]->device();
        return dev ? QVariant(dev->name()) : QVariant();
    }
    return QVariant();
}

QHash<int, QByteArray> DeviceListModel::roleNames() const
{
    return { { SettingsRole, QByteArrayLiteral("settings") },
             { NameRole, QByteArrayLiteral("name") } };
}

void DeviceListModel::deviceNameChanged()
{
    InputDevice *dev = qobject_cast<InputDevice *>(sender());
    if (!dev) {
        return;
    }

    for (int i = 0; i < items_.count(); i++) {
        if (items_[i]->device() == dev) {
            QModelIndex index = this->index(i);
            Q_EMIT dataChanged(index, index, { NameRole });
        }
    }
}

QObject *DeviceListModel::get(int index) const
{
    if (index < 0 || index > items_.count()) {
        return Q_NULLPTR;
    }
    return items_[index];
}
