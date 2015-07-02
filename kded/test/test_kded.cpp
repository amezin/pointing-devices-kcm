#include <QtTest>

#include "kded.h"

class KdedTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        QCoreApplication::instance()->addLibraryPath(PLUGIN_DIR);
    }

    void testLoadUnload() // To be run under Valgrind to detect memory errors
    {
        QEventLoop loop;
        PointingDevicesKDED kded(Q_NULLPTR, QVariantList());
        loop.processEvents();
    }
};

QTEST_GUILESS_MAIN(KdedTest)

#include "test_kded.moc"
