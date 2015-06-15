#pragma once

#include <QObject>
#include <QScopedPointer>

#include <xcb/xcb.h>
#include <xcb/xinput.h>

#include "pointing-devices-x11_export.h"
#include "xcbconnection.h"

class QSocketNotifier;
class XcbAtomCache;
class XInputDevice;

class POINTING_DEVICES_X11_EXPORT XInputDeviceManager : public QObject,
                                                        public XcbConnection
{
    Q_OBJECT

public:
    explicit XInputDeviceManager(const char *display = Q_NULLPTR, QObject *parent = Q_NULLPTR);
    ~XInputDeviceManager() Q_DECL_OVERRIDE;

    XcbAtomCache *atomCache() const
    {
        return atomCache_.data();
    }

    QList<XInputDevice *> devices() const
    {
        return devices_;
    }

    XInputDevice *device(xcb_input_device_id_t) const;
    XInputDevice *device(const char *) const;

Q_SIGNALS:
    void deviceAdded(XInputDevice *);
    void deviceRemoved(XInputDevice *);

private Q_SLOTS:
    void processEvents();

private:
    xcb_window_t rootWindow() const;

    void addDevices(xcb_input_xi_query_device_cookie_t);
    void addDevices(xcb_input_xi_query_device_reply_t *);

    void processEvent(const xcb_generic_event_t *);
    void processEvent(const xcb_generic_error_t *);
    void processEvent(const xcb_ge_generic_event_t *);
    void processEvent(const xcb_input_property_event_t *);
    void processEvent(const xcb_input_hierarchy_event_t *);

    QScopedPointer<XcbAtomCache> atomCache_;
    QList<XInputDevice *> devices_;
    QSocketNotifier *notifier_;
    const xcb_query_extension_reply_t *xinput_;
};
