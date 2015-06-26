#pragma once

#include <QtGlobal>

#include <xcb/xcb.h>

#include "pointingdevices_x11_export.h"

class POINTINGDEVICES_X11_EXPORT XcbObject
{
public:
    explicit XcbObject(xcb_connection_t *connection)
        : connection_(connection)
    {
    }

    virtual ~XcbObject()
    {
    }

    xcb_connection_t *connection() const
    {
        return connection_;
    }

private:
    Q_DISABLE_COPY(XcbObject)

    xcb_connection_t *connection_;
};
