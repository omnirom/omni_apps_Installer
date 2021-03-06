#include "FastbootStep.h"
#include "../util/Utils.h"
#include "../FastbootMonitor.h"
#include "../AdbMonitor.h"

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
    AbstractStep::runStep(commands);

    FastbootMonitor* fbm = FastbootMonitor::getDefault();

    if (fbm->isDeviceOnline())
    {
        onStepPrepared();
    }
    else
    {
        // We monitor fastboot here to be notified when we're ready to unlock. Just waiting on the
        // device to reboot.
        connect(fbm, SIGNAL(onFastbootOnline()), SLOT(onStepPrepared()));
        fbm->startMonitoring();

        // We reboot to fastboot
        AdbMonitor::getDefault()->reboot("bootloader");
    }
}
//------------------------------------------------
void FastbootStep::onStepPrepared()
{
    FastbootMonitor* fbm = FastbootMonitor::getDefault();
    disconnect(fbm, SIGNAL(onFastbootOnline()), this, SLOT(onStepPrepared()));

    qDebug() << "Fastboot is online";
    qDebug() << "Fastboot step: Starting fastboot " << mCommands;

    runNextCommand();
}
//------------------------------------------------
void FastbootStep::runNextCommand()
{
    if (mCurrentCommand >= mCommands.size())
    {
        emit stepEnded("");
        return;
    }

    QString program = Utils::getBundlePath() + FASTBOOT_BINARY;
    QStringList commands = mCommands[mCurrentCommand].split(" ");
    mCurrentCommand++;
    qDebug() << "Running subcommand " << commands;
    mProcess->start(program, commands);

    if (!mProcess->waitForStarted(5000))
    {
        qDebug() << "Timeout starting fastboot process!";

        // The process couldn't start successfully
        emit stepError(-1, "", "Internal error: waitForStarted timeout");
        return;
    }
}
//------------------------------------------------
void FastbootStep::onProcessFinished(int exitCode)
{
    Q_UNUSED(exitCode);

    if (mCurrentCommand < mCommands.size())
    {
        runNextCommand();
        return;
    }

    // If OEM unlock is already done, fastboot returns 1 as it's treated as an error. We'll
    // just ignore it for now, maybe it's worth checking stdout to make sure it's not a real
    // problem.
    emit stepEnded(mProcessStdOut);
}
//------------------------------------------------
