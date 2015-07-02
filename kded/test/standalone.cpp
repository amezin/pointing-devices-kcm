#include <QCoreApplication>

#include "kded.h"

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath(PLUGIN_DIR);
    QCoreApplication app(argc, argv);

    PointingDevicesKDED kded(Q_NULLPTR, QVariantList());

    return app.exec();
}
