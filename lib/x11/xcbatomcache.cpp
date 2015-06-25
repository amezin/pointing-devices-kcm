#include "xcbatomcache.h"

#include <QScopedPointer>

XcbAtomCache::XcbAtomCache(xcb_connection_t *connection)
    : XcbObject(connection)
{
}

XcbAtomCache::~XcbAtomCache()
{
    Q_FOREACH (auto cookie, internCookies_) {
        xcb_discard_reply(connection(), cookie.sequence);
    }
    Q_FOREACH (auto cookie, getNameCookies_) {
        xcb_discard_reply(connection(), cookie.sequence);
    }
}

bool XcbAtomCache::prefetch(const QByteArray &name)
{
    if (atoms_.contains(name) || internCookies_.contains(name)) {
        return true;
    }

    auto cookie = xcb_intern_atom_unchecked(connection(), 1, name.size(), name.constData());
    if (cookie.sequence) {
        internCookies_[name] = cookie;
        return true;
    }
    return false;
}

bool XcbAtomCache::prefetch(xcb_atom_t atom)
{
    if (atomNames_.contains(atom) || getNameCookies_.contains(atom)) {
        return true;
    }

    auto cookie = xcb_get_atom_name_unchecked(connection(), atom);
    if (cookie.sequence) {
        getNameCookies_[atom] = cookie;
        return true;
    }
    return false;
}

xcb_atom_t XcbAtomCache::intern(const QByteArray &name)
{
    if (atoms_.contains(name)) {
        return atoms_[name];
    }
    if (!internCookies_.contains(name)) {
        if (!prefetch(name)) {
            return XCB_ATOM_NONE;
        }
    }

    auto cookie = internCookies_[name];
    internCookies_.remove(name);

    auto reply = xcb_intern_atom_reply(connection(), cookie, Q_NULLPTR);
    if (!reply) {
        return XCB_ATOM_NONE;
    }

    auto atom = reply->atom;
    std::free(reply);

    if (atom == XCB_ATOM_NONE) {
        return atom;
    }

    atoms_[name] = atom;
    atomNames_[atom] = name;

    if (getNameCookies_.contains(atom)) {
        xcb_discard_reply(connection(), getNameCookies_[atom].sequence);
        getNameCookies_.remove(atom);
    }

    return atom;
}

QByteArray XcbAtomCache::getName(xcb_atom_t atom)
{
    if (atomNames_.contains(atom)) {
        return atomNames_[atom];
    }
    if (!getNameCookies_.contains(atom)) {
        if (!prefetch(atom)) {
            return QByteArray();
        }
    }

    auto cookie = getNameCookies_[atom];
    getNameCookies_.remove(atom);

    QScopedPointer<xcb_get_atom_name_reply_t, QScopedPointerPodDeleter>
            reply(xcb_get_atom_name_reply(connection(), cookie, Q_NULLPTR));
    if (!reply) {
        return QByteArray();
    }

    QByteArray name(xcb_get_atom_name_name(reply.data()), reply->name_len);

    atomNames_[atom] = name;
    atoms_[name] = atom;

    if (internCookies_.contains(name)) {
        xcb_discard_reply(connection(), internCookies_[name].sequence);
        internCookies_.remove(name);
    }

    return name;
}
