#include "AdbMonitor.h"
#include "AdbDevice.h"
#include "util/Utils.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>

#define ADB_BINARY "adb"

AdbMonitor* AdbMonitor::sDefault = NULL;

//----------------------------------------------
AdbMonitor::AdbMonitor(QObject *parent) :
    QObject(parent), mMonitorProcess(NULL)
{
    // We start our ADB instance
    startServer();
}
//----------------------------------------------
AdbMonitor::~AdbMonitor()
{
    // We kill adb server when we leave
    killServer();
    killTimer(mMonitorTimerId);

    for (QList<AdbDevice*>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        delete (*it);
    }
    mDevices.clear();
}
//----------------------------------------------
AdbMonitor* AdbMonitor::getDefault()
{
    if (sDefault == NULL)
        sDefault = new AdbMonitor();

    return sDefault;
}
//----------------------------------------------
void AdbMonitor::killDefault()
{
    if (sDefault == NULL)
    {
        delete sDefault;
    }
}
//----------------------------------------------
void AdbMonitor::startServer()
{
    QProcess* proc = new QProcess(this);
    QString program = Utils::getBundlePath() + ADB_BINARY;
    QStringList args;
    args << "start-server";
    proc->start(program, args);

    startMonitorService();
    emit onAdbReady();
}
//----------------------------------------------
void AdbMonitor::startMonitorService()
{
    // We constantly look for new ADB devices
    mMonitorTimerId = startTimer(1000);
    timerEvent(NULL);
}
//----------------------------------------------
void AdbMonitor::killServer()
{
    QProcess* proc = new QProcess(this);
    QString program = Utils::getBundlePath() + ADB_BINARY;
    QStringList args;
    args << "kill-server";
    proc->start(program, args);
}
//----------------------------------------------
void AdbMonitor::timerEvent(QTimerEvent* evt)
{
    Q_UNUSED(evt);

    // Poll ADB devices
    if (mMonitorProcess == NULL)
    {
        mMonitorProcess = new QProcess(this);
        connect(mMonitorProcess, SIGNAL(readyReadStandardOutput()), this,
                SLOT(onAdbDevicesReadyReadStdOut()));
        connect(mMonitorProcess, SIGNAL(readyReadStandardError()), this,
                SLOT(onAdbDevicesReadyReadStdErr()));

    }

    // We make sure we're not running already
    if (mMonitorProcess->state() == QProcess::Starting
            || mMonitorProcess->state() == QProcess::Running) {
        qDebug() << "Previous 'adb devices' event not finished, bailing out";
        return;
    }

    QString program = Utils::getBundlePath() + ADB_BINARY;
    QStringList args;
    args << "devices";
    args << "-l";

    mMonitorProcess->start(program, args);
}
//----------------------------------------------
void AdbMonitor::onAdbDevicesReadyReadStdOut()
{
    QString output = mMonitorProcess->readAllStandardOutput();
    QStringList lines = output.split("\n");

    // We skip the first line ("List of devices attached")
    QStringList::iterator line = lines.begin();
    ++line;

    // We keep reference of the devices to detect unplugged devices
    QStringList pings;

    for (; line != lines.end(); ++line)
    {
        QString lineStr = (*line);

        // Skip eventual other empty lines
        if (lineStr.trimmed().isEmpty()) continue;

        // I'm not sure why Google don't use a tab here...
        // XXX: Is it because of Windows?
        QStringList lineSplit = lineStr.split("       ");

        if (lineSplit.size() == 2) {
            QString deviceSerial = lineSplit.at(0);
            QString deviceStatus = lineSplit.at(1);
            pings.push_back(deviceSerial);

            processAdbDevice(deviceSerial, deviceStatus);
        }
    }

    // We prune disconnected devices
    QList<AdbDevice*> pruneList;
    for (QList<AdbDevice*>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        if (!pings.contains((*it)->getSerial()))
        {
            emit onDeviceDisconnected(*it);
            pruneList.push_back(*it);
        }
    }

    for (QList<AdbDevice*>::iterator it = pruneList.begin(); it != pruneList.end(); ++it)
    {
        mDevices.removeAll((*it));
        delete (*it);
    }

    pruneList.clear();
    pings.clear();
}
//----------------------------------------------
void AdbMonitor::onAdbDevicesReadyReadStdErr()
{
    qDebug() << mMonitorProcess->readAllStandardError();
}
//----------------------------------------------
void AdbMonitor::processAdbDevice(const QString &serial, const QString &deviceLine)
{
    AdbDevice* device = findDevice(serial);

    bool isNewDevice = (device == NULL);
    if (isNewDevice)
    {
        device = new AdbDevice(serial);
        mDevices.push_back(device);
    }

    if (deviceLine.startsWith("unauthorized") && isNewDevice)
    {
        // A device has been plugged, but is not authorized
        device->setState(ADB_DEVICE_STATE_UNAUTHORIZED);
        emit onDeviceConnected(device);
    }
    else if (deviceLine.startsWith("offline") && isNewDevice)
    {
        // A device has been plugged, but is offline. User-wise,
        // it's the same thing: they need to check their device
        device->setState(ADB_DEVICE_STATE_OFFLINE);
        emit onDeviceConnected(device);
    }
    else if (deviceLine.startsWith("recovery") && device->getState() != ADB_DEVICE_STATE_RECOVERY)
    {
        // A device arrived in recovery mode
        device->setState(ADB_DEVICE_STATE_RECOVERY);
        emit onDeviceConnected(device);
    }
    else if (deviceLine.startsWith("device") && device->getState() != ADB_DEVICE_STATE_ONLINE)
    {
        // Device is online or became online. We only notify once.
        device->setState(ADB_DEVICE_STATE_ONLINE);
        QStringList props = deviceLine.split(" ");
        for (QStringList::iterator it = props.begin(); it != props.end(); ++it)
        {
            if ((*it) == "device") continue;
            QString key = (*it).left((*it).indexOf(":")).trimmed();
            QString value = (*it).mid((*it).indexOf(":")+1).trimmed();

            if (key == "product") device->setProduct(value);
            else if (key == "model") device->setModel(value);
            else if (key == "device") device->setDevice(value);
            else qDebug() << "Unknown key: " << key;
        }

        emit onDeviceConnected(device);
    }

    device->dump();
}
//----------------------------------------------
AdbDevice* AdbMonitor::findDevice(const QString &serial)
{
    for (QList<AdbDevice*>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        if ((*it)->getSerial() == serial) return (*it);
    }

    return NULL;
}
//----------------------------------------------
bool AdbMonitor::hasDeviceConnected()
{
    return mDevices.size() > 0;
}
//----------------------------------------------
void AdbMonitor::pullFile(const QString &remotePath,
                          const QString &localPath,
                          const QString &deviceSerial)
{
    Q_UNUSED(remotePath);
    Q_UNUSED(localPath);
    Q_UNUSED(deviceSerial);
    qDebug() << "Unimplemented method pullFile";
}
//----------------------------------------------
void AdbMonitor::pushFile(const QString &srcPath,
                          const QString &destPath,
                          const QString &deviceSerial)
{
    Q_UNUSED(srcPath);
    Q_UNUSED(destPath);
    Q_UNUSED(deviceSerial);
    qDebug() << "Unimplemented method pushFile";
}
//----------------------------------------------
void AdbMonitor::shell(const QString &parameters, bool blocking)
{
    QProcess* proc = new QProcess(this);
    QString program = Utils::getBundlePath() + ADB_BINARY;
    QStringList args;
    args << "shell" << QString("'" + parameters + "'");
    proc->start(program, args);

    if (blocking && !proc->waitForFinished(10000))
    {
        qDebug() << "Timed out waiting for shell command";
    }
}
//----------------------------------------------
void AdbMonitor::reboot(const QString &destination)
{
    QProcess* proc = new QProcess(this);
    QString program = Utils::getBundlePath() + ADB_BINARY;
    QStringList args;
    args << "reboot" << destination;
    proc->start(program, args);

    if (!proc->waitForFinished(10000))
    {
        qDebug() << "Timed out waiting for adb reboot " << destination;
    }
}
//----------------------------------------------
void AdbMonitor::sideload(const QString &path)
{
    QProcess* proc = new QProcess(this);
    QString program = Utils::getBundlePath() + ADB_BINARY;
    QStringList args;
    args << "sideload" << path;
    proc->start(program, args);

    if (!proc->waitForFinished(120000))
    {
        qDebug() << "Timed out (120s) waiting for adb sideload " << path;
    }

    qDebug() << proc->readAllStandardOutput();
}
//----------------------------------------------
