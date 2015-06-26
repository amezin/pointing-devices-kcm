#pragma once

#include "xcbobject.h"

#include "pointingdevices_x11_export.h"

class POINTINGDEVICES_X11_EXPORT XcbConnection : public XcbObject
{
public:
    explicit XcbConnection(const char *display = Q_NULLPTR)
        : XcbObject(xcb_connect(display, &defaultScreen_))
    {
    }

    ~XcbConnection() Q_DECL_OVERRIDE
    {
        if (connection()) {
            xcb_disconnect(connection());
        }
    }

    int defaultScreen() const
    {
        return defaultScreen_;
    }

private:
    int defaultScreen_;
};
