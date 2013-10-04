#include "FastbootStep.h"
#include "../util/Utils.h"

#include <QDebug>

#define FASTBOOT_BINARY "fastboot"

//------------------------------------------------
FastbootStep::FastbootStep(QObject *parent) :
    AbstractStep(parent)
{

}
//------------------------------------------------
void FastbootStep::runStep(const QStringList &commands)
{
    qDebug() << "Fastboot step: Starting fastboot " << commands;

    QString program = Utils::getBundlePath() + FASTBOOT_BINARY;
    mProcess->start(program, commands);

    if (!mProcess->waitForStarted(5000))
    {
        // The process couldn't start successfully
        emit stepError("", "Internal error: waitForStarted timeout");
        return;
    }

    mProcess->waitForFinished(-1);
}
//------------------------------------------------
