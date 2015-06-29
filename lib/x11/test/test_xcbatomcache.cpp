#include <QtTest>

#include "xcbatomcache.h"
#include "xcbconnection.h"

class XcbAtomCacheTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testKnownAtomsIntern()
    {
        XcbConnection c;
        XcbAtomCache cache(c.connection());
        QCOMPARE(cache.intern(QByteArrayLiteral("PRIMARY")), xcb_atom_t(XCB_ATOM_PRIMARY));
        QCOMPARE(cache.intern(QByteArrayLiteral("SECONDARY")), xcb_atom_t(XCB_ATOM_SECONDARY));
    }

    void testKnownAtomsGetName()
    {
        XcbConnection c;
        XcbAtomCache cache(c.connection());
        QCOMPARE(cache.getName(XCB_ATOM_PRIMARY), QByteArrayLiteral("PRIMARY"));
        QCOMPARE(cache.getName(XCB_ATOM_SECONDARY), QByteArrayLiteral("SECONDARY"));
    }

};

QTEST_GUILESS_MAIN(XcbAtomCacheTest)

#include "test_xcbatomcache.moc"
