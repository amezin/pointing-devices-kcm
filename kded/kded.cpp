#include "kded.h"

#include <KPluginFactory>

K_PLUGIN_FACTORY(PointingDevicesKDEDFactory, registerPlugin<PointingDevicesKDED>();)

PointingDevicesKDED::PointingDevicesKDED(QObject *parent, const QVariantList &)
    : KDEDModule(parent)
{
}

PointingDevicesKDED::~PointingDevicesKDED()
{
}

#include "kded.moc"
