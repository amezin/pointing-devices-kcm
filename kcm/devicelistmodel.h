#pragma once

#include <QAbstractListModel>

class DeviceSettings;

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        SettingsRole = Qt::UserRole
    };

    DeviceListModel(QObject *parent = Q_NULLPTR);
    ~DeviceListModel() Q_DECL_OVERRIDE;

    void add(DeviceSettings *);
    void remove(DeviceSettings *);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    QList<DeviceSettings *> all() const
    {
        return items_;
    }

    Q_INVOKABLE QObject *get(int index) const;

private:
    void deviceNameChanged();

    QList<DeviceSettings *> items_;
};
