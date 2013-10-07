#include "FastbootMonitor.h"
#include "util/Utils.h"

#include <QDebug>

#define FASTBOOT_BINARY "fastboot"

FastbootMonitor* FastbootMonitor::sDefault = NULL;

//------------------------------------------
FastbootMonitor::FastbootMonitor(QObject *parent) :
    QObject(parent), mMonitorProcess(NULL)
{

}
//------------------------------------------
FastbootMonitor* FastbootMonitor::getDefault()
{
    if (!sDefault) sDefault = new FastbootMonitor();
    return sDefault;
}
//------------------------------------------
void FastbootMonitor::startMonitoring()
{
    if (mMonitorProcess != NULL) return;

    qDebug() << "Starting fastboot monitoring process";

    mMonitorProcess = new QProcess(this);
    connect(mMonitorProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMonitorStdOut()));

    mMonitorTimerId = startTimer(1000);
}
//------------------------------------------
void FastbootMonitor::stopMonitoring()
{
    killTimer(mMonitorTimerId);

    if (mMonitorProcess != NULL)
    {
        delete mMonitorProcess;
        mMonitorProcess = NULL;
    }
}
//------------------------------------------
bool FastbootMonitor::isDeviceOnline()
{
    return mDeviceSerial != "";
}
//------------------------------------------
void FastbootMonitor::onMonitorStdOut()
{
    // NOTE: we only support ONE fastboot device at a time here!!
    QString data = QString(mMonitorProcess->readAllStandardOutput()).trimmed();

    if (data == "")
    {
        if (mDeviceSerial != "")
        {
            qDebug() << "Fastboot device removed";
            mDeviceSerial = "";
            emit onFastbootOffline();
        }
    }
    else
    {
        qDebug() << "Fastboot device online";
        QStringList splitData = data.split("\t");
        QString deviceSerial = splitData[0];

        if (mDeviceSerial != deviceSerial)
        {
            mDeviceSerial = deviceSerial;
            emit onFastbootOnline();
        }
    }
}
//------------------------------------------
void FastbootMonitor::timerEvent(QTimerEvent *evt)
{
    Q_UNUSED(evt);

    if (mMonitorProcess->state() == QProcess::Running)
    {
        qDebug() << "Previous fastboot monitor still running, pruning this timer event";
        return;
    }

    QString program = Utils::getBundlePath() + FASTBOOT_BINARY;
    QStringList args;
    args << "devices";
    mMonitorProcess->start(program, args);
}
//------------------------------------------
