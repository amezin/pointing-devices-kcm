#pragma once

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QVariant>

#include <xcb/xcb.h>
#include <xcb/xinput.h>

#include "xcbobject.h"
#include "pointingdevices_x11_export.h"

class XcbAtomCache;

class POINTINGDEVICES_X11_EXPORT XInputDevice : public QObject, public XcbObject
{
    Q_OBJECT

public:
    XInputDevice(const xcb_input_xi_device_info_t *, XcbAtomCache *, QObject *parent = Q_NULLPTR);
    ~XInputDevice() Q_DECL_OVERRIDE;

    xcb_input_device_id_t id() const
    {
        return id_;
    }

    xcb_input_device_type_t type() const
    {
        return type_;
    }

    QByteArray name() const
    {
        return name_;
    }

    bool enabled() const
    {
        return enabled_;
    }

    XcbAtomCache *atomCache() const
    {
        return atomCache_;
    }

    QVariant deviceProperty(xcb_atom_t);
    QVariant deviceProperty(const QByteArray &name);

    bool setDeviceProperty(xcb_atom_t, const QVariant &);
    bool setDeviceProperty(const QByteArray &, const QVariant &);

    QByteArrayList devicePropertyNames() const;
    QList<xcb_atom_t> devicePropertyAtoms() const;

    bool devicePropertyExists(xcb_atom_t) const;
    bool devicePropertyExists(const QByteArray &) const;

    void processEvent(const xcb_input_property_event_t *);
    void processEvent(const xcb_input_hierarchy_info_t *);

Q_SIGNALS:
    void devicePropertyAdded(const QByteArray &);
    void devicePropertyRemoved(const QByteArray &);
    void devicePropertyChanged(const QByteArray &);
    void enabledChanged();
    void typeChanged();

private:
    bool fetchDeviceProperty(xcb_atom_t);
    QVariant getDevicePropertyValue(const xcb_input_xi_get_property_reply_t *);

    template<typename T>
    QVariant unpackProperty(const xcb_input_xi_get_property_reply_t *, const T *);
    template<typename T>
    QVariant unpackPropertyItem(const xcb_input_xi_get_property_reply_t *, const T *);

    template<typename T>
    bool packPropertyItem(T *, const QVariant &, xcb_atom_t);
    template<typename T>
    QByteArray packProperty(const QVariantList &, xcb_atom_t);

    XcbAtomCache *atomCache_;

    xcb_input_device_id_t id_;
    xcb_input_device_type_t type_;
    bool enabled_;
    QByteArray name_;

    struct PropertyInfo
    {
        QVariant value;
        xcb_atom_t type;
        xcb_input_property_format_t format;
    };

    QHash<xcb_atom_t, PropertyInfo> properties_;
    QHash<xcb_atom_t, xcb_input_xi_get_property_cookie_t> cookies_;
};
