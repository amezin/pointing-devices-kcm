#pragma once

#include <KDEDModule>

class PointingDevicesKDED : public KDEDModule
{
    Q_OBJECT
public:
    PointingDevicesKDED(QObject *parent, const QVariantList &);
    ~PointingDevicesKDED() Q_DECL_OVERRIDE;
};
