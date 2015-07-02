#include <QtTest>

#include "xinputdevicemanager.h"
#include "xinputdeviceadapter.h"
#include "xinputdevicemanageradapter.h"
#include "xvfb.h"

class AdaptersTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testBasicProperties()
    {
        Xvfb xvfb(":123");
        QCOMPARE(xvfb.state(), QProcess::Running);

        XInputDeviceManager manager(xvfb.display());
        XInputDeviceManagerAdapter managerAdapter(&manager);
    }

};

QTEST_GUILESS_MAIN(AdaptersTest)

#include "test_adapters.moc"

