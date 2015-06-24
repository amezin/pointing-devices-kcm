#include "xinputdevice.h"

#include <type_traits>

#include "xcbatomcache.h"
#include "log.h"

XInputDevice::XInputDevice(const xcb_input_xi_device_info_t *info, XcbAtomCache *atomCache, QObject *parent)
    : QObject(parent),
      XcbObject(atomCache->connection()),
      atomCache_(atomCache),
      id_(info->deviceid),
      type_(xcb_input_device_type_t(info->type)),
      enabled_(info->enabled),
      name_(xcb_input_xi_device_info_name(info), info->name_len)
{
    auto cookie = xcb_input_xi_list_properties_unchecked(connection(), id_);
    QScopedPointer<xcb_input_xi_list_properties_reply_t, QScopedPointerPodDeleter>
            reply(xcb_input_xi_list_properties_reply(connection(), cookie, Q_NULLPTR));
    if (!reply) {
        return;
    }

    auto properties = xcb_input_xi_list_properties_properties(reply.data());
    for (int i = 0; i < reply->num_properties; i++) {
        fetchDeviceProperty(properties[i]);
        atomCache->prefetch(properties[i]);
    }
}

XInputDevice::~XInputDevice()
{
    for (auto cookie : cookies_) {
        xcb_discard_reply(connection(), cookie.sequence);
    }
}

bool XInputDevice::devicePropertyExists(xcb_atom_t atom) const
{
    return properties_.contains(atom) || cookies_.contains(atom);
}

bool XInputDevice::devicePropertyExists(const QByteArray &name) const
{
    return devicePropertyExists(atomCache_->intern(name));
}

bool XInputDevice::fetchDeviceProperty(xcb_atom_t atom)
{
    if (devicePropertyExists(atom)) {
        return true;
    }

    auto cookie = xcb_input_xi_get_property(connection(), id_, 0, atom, XCB_GET_PROPERTY_TYPE_ANY, 0, 1000);
    if (cookie.sequence) {
        cookies_[atom] = cookie;
        return true;
    }
    return false;
}

template<typename T>
QVariant XInputDevice::unpackPropertyItem(const xcb_input_xi_get_property_reply_t *reply, const T *data)
{
    switch (reply->type) {
    case XCB_ATOM_INTEGER:
        return QVariant(*reinterpret_cast<const typename std::make_signed<T>::type *>(data));
    case XCB_ATOM_CARDINAL:
        return QVariant(*data);
    case XCB_ATOM_ATOM: {
        auto name = atomCache_->getName(*reinterpret_cast<const xcb_atom_t *>(data));
        return (name.isEmpty()) ? QVariant() : name;
        }
    }
    if (reply->type == atomCache_->intern(QByteArrayLiteral("FLOAT"))) {
        return QVariant(*reinterpret_cast<const float *>(data));
    }
    qCCritical(POINTINGDEVICES) << "Unknown property type" << atomCache_->getName(reply->type);
    return QVariant();
}

template<typename T>
QVariant XInputDevice::unpackProperty(const xcb_input_xi_get_property_reply_t *reply, const T *data)
{
    QVariantList items;
    for (unsigned i = 0; i < reply->num_items; i++) {
        auto item = unpackPropertyItem(reply, &data[i]);
        if (!item.isValid()) {
            return QVariant();
        }
        items.append(item);
    }
    return items.size() == 1 ? items[0] : items;
}

template<typename T>
bool XInputDevice::packPropertyItem(T *data, const QVariant &value, xcb_atom_t type)
{
    switch (type) {
    case XCB_ATOM_INTEGER:
    case XCB_ATOM_CARDINAL:
        *data = value.value<T>();
        return true;
    case XCB_ATOM_ATOM: {
        auto atom = atomCache_->intern(value.toByteArray());
        *data = static_cast<T>(atom);
        return true;
    }
    }
    if (type == atomCache_->intern(QByteArrayLiteral("FLOAT"))) {
        *reinterpret_cast<float *>(data) = value.toFloat();
        return true;
    }
    qCCritical(POINTINGDEVICES) << "Unknown property type" << atomCache_->getName(type);
    return false;
}

template<typename T>
QByteArray XInputDevice::packProperty(const QVariantList &value, xcb_atom_t type)
{
    QByteArray buffer(value.size() * sizeof(T), '\0');
    auto data = reinterpret_cast<T *>(buffer.data());
    for (int i = 0; i < value.size(); i++) {
        if (!packPropertyItem(&data[i], value[i], type)) {
            return QByteArray();
        }
    }
    return buffer;
}

QVariant XInputDevice::getDevicePropertyValue(const xcb_input_xi_get_property_reply_t *reply)
{
    xcb_input_xi_get_property_items_t items;
    xcb_input_xi_get_property_items_unpack(xcb_input_xi_get_property_items(reply),
                                           reply->num_items,
                                           reply->format,
                                           &items);

    if (reply->type == XCB_ATOM_STRING) {
        if (reply->format != XCB_INPUT_PROPERTY_FORMAT_8_BITS) {
            qCCritical(POINTINGDEVICES) << "Only 8-bit strings are supported";
            return QVariant();
        }
        return QByteArray(reinterpret_cast<char *>(xcb_input_xi_get_property_items_data_8(&items)),
                          reply->num_items);
    }

    switch (reply->format) {
    case XCB_INPUT_PROPERTY_FORMAT_8_BITS:
        return unpackProperty(reply, xcb_input_xi_get_property_items_data_8(&items));
    case XCB_INPUT_PROPERTY_FORMAT_16_BITS:
        return unpackProperty(reply, xcb_input_xi_get_property_items_data_16(&items));
    case XCB_INPUT_PROPERTY_FORMAT_32_BITS:
        return unpackProperty(reply, xcb_input_xi_get_property_items_data_32(&items));
    }
    return QVariant();
}

QVariant XInputDevice::deviceProperty(xcb_atom_t atom)
{
    if (properties_.contains(atom)) {
        return properties_[atom].value;
    }

    if (!cookies_.contains(atom)) {
        qCCritical(POINTINGDEVICES) << "Device doesn't have property" << atomCache_->getName(atom);
        return QVariant();
    }

    auto cookie = cookies_[atom];
    cookies_.remove(atom);

    QScopedPointer<xcb_input_xi_get_property_reply_t, QScopedPointerPodDeleter>
            reply(xcb_input_xi_get_property_reply(connection(), cookie, Q_NULLPTR));
    if (!reply) {
        qCCritical(POINTINGDEVICES) << "XIGetProperty request failed for" << atomCache_->getName(atom);
        return QVariant();
    }

    auto value = getDevicePropertyValue(reply.data());
    if (value.isValid()) {
        properties_[atom] = PropertyInfo{ value, reply->type, xcb_input_property_format_t(reply->format) };
    } else {
        qCCritical(POINTINGDEVICES) << "Unknown format" << reply->format
                                    << "of property" << atomCache_->getName(atom);
    }
    return value;
}

QVariant XInputDevice::deviceProperty(const QByteArray &name)
{
    auto atom = atomCache_->intern(name);
    if (atom == XCB_ATOM_NONE) {
        qCCritical(POINTINGDEVICES) << "No atom with name" << name;
        return QVariant();
    }

    return deviceProperty(atom);
}

bool XInputDevice::setDeviceProperty(xcb_atom_t atom, const QVariant &value)
{
    auto currentValue = deviceProperty(atom);
    if (!currentValue.isValid()) {
        return false;
    }
    if (currentValue == value) {
        return true;
    }

    xcb_input_xi_change_property_items_t items;
    auto info = properties_[atom];
    if (info.type == XCB_ATOM_STRING) {
        if (info.format != XCB_INPUT_PROPERTY_FORMAT_8_BITS) {
            qCCritical(POINTINGDEVICES) << "Only 8-bit strings are supported";
            return false;
        }
        if (!value.canConvert<QByteArray>()) {
            qCCritical(POINTINGDEVICES) << "Can't convert" << value << "to byte array";
            return false;
        }

        auto v = value.value<QByteArray>();
        items.data8 = reinterpret_cast<uint8_t *>(v.data());
        xcb_input_xi_change_property_aux(connection(), id_, XCB_PROP_MODE_REPLACE,
                                         info.format, atom, info.type, v.size(), &items);
        xcb_flush(connection());
        return true;
    }

    auto asList = value.canConvert<QVariantList>() ? value.toList()
                                                   : (QVariantList() << value);
    QByteArray data;
    switch (info.format) {
    case XCB_INPUT_PROPERTY_FORMAT_8_BITS:
        data = packProperty<uint8_t>(asList, info.type);
        items.data8 = reinterpret_cast<uint8_t *>(data.data());
        break;
    case XCB_INPUT_PROPERTY_FORMAT_16_BITS:
        data = packProperty<uint16_t>(asList, info.type);
        items.data16 = reinterpret_cast<uint16_t *>(data.data());
        break;
    case XCB_INPUT_PROPERTY_FORMAT_32_BITS:
        data = packProperty<uint32_t>(asList, info.type);
        items.data32 = reinterpret_cast<uint32_t *>(data.data());
        break;
    default:
        qCCritical(POINTINGDEVICES) << "Unknown property format" << info.format;
        return false;
    }

    if (data.isNull()) {
        return false;
    }

    xcb_input_xi_change_property_aux(connection(), id_, XCB_PROP_MODE_REPLACE,
                                     info.format, atom, info.type, asList.size(), &items);
    xcb_flush(connection());
    return true;
}

bool XInputDevice::setDeviceProperty(const QByteArray &name, const QVariant &value)
{
    auto atom = atomCache_->intern(name);
    if (atom == XCB_ATOM_NONE) {
        qCCritical(POINTINGDEVICES) << "No atom with name" << name;
        return false;
    }
    return setDeviceProperty(atom, value);
}

QByteArrayList XInputDevice::devicePropertyNames() const
{
    auto atoms = devicePropertyAtoms();
    QByteArrayList names;
    for (auto atom : atoms) {
        names.push_back(atomCache_->getName(atom));
    }
    return names;
}

QList<xcb_atom_t> XInputDevice::devicePropertyAtoms() const
{
    auto atoms = cookies_.keys();
    atoms.append(properties_.keys());
    return atoms;
}

void XInputDevice::processEvent(const xcb_input_property_event_t *e)
{
    Q_ASSERT(e->deviceid == id_);

    bool existBefore = devicePropertyExists(e->property);

    bool isModified = bool(e->what & XCB_INPUT_PROPERTY_FLAG_MODIFIED);
    bool isDeleted = bool(e->what & XCB_INPUT_PROPERTY_FLAG_DELETED);

    if (!existBefore || isModified) {
        properties_.remove(e->property);
        if (cookies_.contains(e->property)) {
            xcb_discard_reply(connection(), cookies_[e->property].sequence);
            cookies_.remove(e->property);
        }
    }

    if (!isDeleted) {
        fetchDeviceProperty(e->property);
    }

    bool existAfter = devicePropertyExists(e->property);
    Q_ASSERT(existBefore || existAfter);

    auto name = atomCache_->getName(e->property);
    if (!existBefore) {
        Q_EMIT devicePropertyAdded(name);
    } else if (!existAfter) {
        Q_EMIT devicePropertyRemoved(name);
    } else if (isModified) {
        Q_EMIT devicePropertyChanged(name);
    }
}

void XInputDevice::processEvent(const xcb_input_hierarchy_info_t *e)
{
    Q_ASSERT(e->deviceid == id_);

    if (type_ != e->type) {
        type_ = xcb_input_device_type_t(e->type);
        Q_EMIT typeChanged();
    }

    if (enabled_ != e->enabled) {
        enabled_ = e->enabled;
        Q_EMIT enabledChanged();
    }
}
