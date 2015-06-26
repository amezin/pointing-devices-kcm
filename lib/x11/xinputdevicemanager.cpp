#include "xinputdevicemanager.h"

#include <QScopedPointer>
#include <QSocketNotifier>

#include <xcb/xcb_event.h>

#include "xcbatomcache.h"
#include "xinputdevice.h"
#include "log.h"

XInputDeviceManager::XInputDeviceManager(const char *display, QObject *parent)
    : QObject(parent),
      XcbConnection(display),
      atomCache_(new XcbAtomCache(connection())),
      notifier_(new QSocketNotifier(xcb_get_file_descriptor(connection()), QSocketNotifier::Read, this)),
      xinput_(xcb_get_extension_data(connection(), &xcb_input_id))
{
    connect(notifier_, SIGNAL(activated(int)), SLOT(processEvents()));

    struct {
        xcb_input_event_mask_t head;
        xcb_input_xi_event_mask_t mask;
    } mask;
    mask.head.deviceid = XCB_INPUT_DEVICE_ALL;
    mask.head.mask_len = sizeof(mask.mask) / sizeof(uint32_t);
    mask.mask = xcb_input_xi_event_mask_t(XCB_INPUT_XI_EVENT_MASK_PROPERTY |
                                          XCB_INPUT_XI_EVENT_MASK_HIERARCHY);
    xcb_input_xi_select_events(connection(), rootWindow(), 1, &mask.head);

    addDevices(xcb_input_xi_query_device_unchecked(connection(), XCB_INPUT_DEVICE_ALL));
}

XInputDeviceManager::~XInputDeviceManager()
{
    // Ensure XInputDevices are deleted before xcb_disconnect()
    Q_FOREACH (auto dev, devices_) {
        delete dev;
    }
}

void XInputDeviceManager::addDevices(xcb_input_xi_query_device_reply_t *reply)
{
    for (auto i = xcb_input_xi_query_device_infos_iterator(reply); i.rem;
         xcb_input_xi_device_info_next(&i))
    {
        auto dev = new XInputDevice(i.data, atomCache(), this);
        devices_.append(dev);
        Q_EMIT deviceAdded(dev);
    }
}

void XInputDeviceManager::addDevices(xcb_input_xi_query_device_cookie_t cookie)
{
    QScopedPointer<xcb_input_xi_query_device_reply_t, QScopedPointerPodDeleter>
            reply(xcb_input_xi_query_device_reply(connection(), cookie, Q_NULLPTR));
    if (!reply) {
        qCCritical(POINTINGDEVICES) << "XIQueryDevice failed";
        return;
    }
    addDevices(reply.data());
}

xcb_window_t XInputDeviceManager::rootWindow() const
{
    auto setup = xcb_get_setup(connection());
    int steps = defaultScreen();
    for (auto i = xcb_setup_roots_iterator(setup); i.rem; xcb_screen_next(&i)) {
        if (!steps--) {
            return i.data->root;
        }
    }
    return XCB_NONE;
}

void XInputDeviceManager::processEvents()
{
    Q_FOREVER {
        QScopedPointer<xcb_generic_event_t, QScopedPointerPodDeleter>
                event(xcb_poll_for_event(connection()));
        if (!event) {
            break;
        }
        processEvent(event.data());
    }
}

XInputDevice *XInputDeviceManager::device(xcb_input_device_id_t id) const
{
    Q_FOREACH (auto device, devices_) {
        if (device->id() == id) {
            return device;
        }
    }
    return Q_NULLPTR;
}

XInputDevice *XInputDeviceManager::device(const char *name) const
{
    Q_FOREACH (auto device, devices_) {
        if (device->name() == name) {
            return device;
        }
    }
    return Q_NULLPTR;
}

void XInputDeviceManager::processEvent(const xcb_generic_event_t *e)
{
    switch (XCB_EVENT_RESPONSE_TYPE(e)) {
    case 0:
        processEvent(reinterpret_cast<const xcb_generic_error_t *>(e));
        break;
    case XCB_GE_GENERIC:
        processEvent(reinterpret_cast<const xcb_ge_generic_event_t *>(e));
        break;
    }
}

void XInputDeviceManager::processEvent(const xcb_generic_error_t *e)
{
    qCCritical(POINTINGDEVICES) << "XCB error"
                                << xcb_event_get_error_label(e->error_code);
}

void XInputDeviceManager::processEvent(const xcb_ge_generic_event_t *e)
{
    if (e->extension != xinput_->major_opcode) {
        return;
    }

    switch (e->event_type) {
    case XCB_INPUT_PROPERTY:
        processEvent(reinterpret_cast<const xcb_input_property_event_t *>(e));
        break;
    case XCB_INPUT_HIERARCHY:
        processEvent(reinterpret_cast<const xcb_input_hierarchy_event_t *>(e));
        break;
    }
}

void XInputDeviceManager::processEvent(const xcb_input_property_event_t *e)
{
    auto dev = device(e->deviceid);
    if (dev) {
        dev->processEvent(e);
    }
}

void XInputDeviceManager::processEvent(const xcb_input_hierarchy_event_t *e)
{
    auto info = reinterpret_cast<const xcb_input_hierarchy_info_t *>(e + 1);

    QList<xcb_input_xi_query_device_cookie_t> cookies;
    for (unsigned i = 0; i < e->num_infos; i++) {
        if (info[i].flags & (XCB_INPUT_HIERARCHY_MASK_MASTER_ADDED |
                             XCB_INPUT_HIERARCHY_MASK_SLAVE_ADDED))
        {
            cookies.append(xcb_input_xi_query_device_unchecked(connection(),
                                                               info[i].deviceid));
        }
    }

    Q_FOREACH (auto cookie, cookies) {
        addDevices(cookie);
    }

    for (unsigned i = 0; i < e->num_infos; i++) {
        auto dev = device(info[i].deviceid);
        if (!dev) {
            qCCritical(POINTINGDEVICES) << "Event for unknown device ID" << info[i].deviceid;
            continue;
        }
        if (info[i].flags & (XCB_INPUT_HIERARCHY_MASK_MASTER_REMOVED |
                             XCB_INPUT_HIERARCHY_MASK_SLAVE_REMOVED))
        {
            devices_.removeAll(dev);
            Q_EMIT deviceRemoved(dev);
            delete dev;
        } else {
            dev->processEvent(&info[i]);
        }
    }
}
