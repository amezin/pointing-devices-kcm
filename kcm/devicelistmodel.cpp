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
    return { { SettingsRole, QByteArrayLiteral("device") },
             { NameRole, QByteArrayLiteral("name") } };
}

QObject *DeviceListModel::get(int index) const
{
    if (index < 0 || index > items_.count()) {
        return Q_NULLPTR;
    }
    return items_[index];
}
