#include <QProcess>

#include "config.h"

class Xvfb : public QProcess
{
public:
    explicit Xvfb(const QByteArray &display)
        : display_(display)
    {
        start(QLatin1String(XVFB_COMMAND), QStringList() << display);
        waitForStarted();
        QThread::sleep(1);
    }

    ~Xvfb() Q_DECL_OVERRIDE
    {
        terminate();
        waitForFinished();
    }

    const char *display() const
    {
        return display_.constData();
    }

private:
    const QByteArray display_;
};
