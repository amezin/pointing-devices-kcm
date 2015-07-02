#include <QtTest>

#include "devicemanager.h"

class DeviceManagerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        auto app = QCoreApplication::instance();
        app->addLibraryPath(PLUGIN_DIR);
    }

    void testPluginLoad()
    {
        QScopedPointer<InputDeviceManager> manager(InputDeviceManager::create());
        QVERIFY(manager);
    }
};

QTEST_GUILESS_MAIN(DeviceManagerTest)

#include "test_devicemanager.moc"
