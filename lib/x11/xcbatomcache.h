#pragma once

#include <xcb/xcb.h>

#include <QByteArray>
#include <QHash>

#include "pointingdevices_x11_export.h"
#include "xcbobject.h"

class POINTINGDEVICES_X11_EXPORT XcbAtomCache : public XcbObject
{
public:
    explicit XcbAtomCache(xcb_connection_t *);
    ~XcbAtomCache() Q_DECL_OVERRIDE;

    xcb_atom_t intern(const QByteArray &);
    bool prefetch(const QByteArray &);

    QByteArray getName(xcb_atom_t);
    bool prefetch(xcb_atom_t);

private:
    QHash<QByteArray, xcb_intern_atom_cookie_t> internCookies_;
    QHash<QByteArray, xcb_atom_t> atoms_;

    QHash<xcb_atom_t, xcb_get_atom_name_cookie_t> getNameCookies_;
    QHash<xcb_atom_t, QByteArray> atomNames_;
};
