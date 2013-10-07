#include "FlashScenario.h"
#include "QJson/json_parser.hh"

#include "flasher/AbstractStep.h"
#include "flasher/FastbootStep.h"
#include "flasher/NullStep.h"
#include "BundleManager.h"
#include "AdbMonitor.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>

//-------------------------------------------------
FlashScenario::FlashScenario(QObject *parent) :
    QObject(parent), mScenario(NULL), mBuild(NULL)
{

}
//-------------------------------------------------
FlashScenario::~FlashScenario()
{
    if (mScenario) delete mScenario;
}
//-------------------------------------------------
bool FlashScenario::existsForDevice(const QString &device)
{
    QFile scenarios(":/global/assets/scenarios.json");
    if (!scenarios.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(NULL, tr("Critical error"), tr("Could not open the scenarios file!"));
        return false;
    }

    QString data = scenarios.readAll();

    // Parse the device section to find if we have something for our device
    QJson::Parser parser;
    bool ok;

    QVariantList j_root = parser.parse(data.toUtf8(), &ok).toList();

    if (!ok) qDebug() << "Unable to parse root element of the JSON data";

    for (QVariantList::iterator it = j_root.begin(); it != j_root.end(); ++it)
    {
        QVariantMap j_scenario = (*it).toMap();

        if (j_scenario["device"] == device) return true;
    }

    return false;
}
//-------------------------------------------------
bool FlashScenario::loadForDevice(const QString &device)
{
    QFile scenarios(":/global/assets/scenarios.json");
    if (!scenarios.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(NULL, tr("Critical error"), tr("Could not open the scenarios file!"));
        return false;
    }

    QString data = scenarios.readAll();

    // Parse the device section to find if we have something for our device
    QJson::Parser parser;
    bool ok;

    QVariantList j_root = parser.parse(data.toUtf8(), &ok).toList();

    if (!ok) qDebug() << "Unable to parse root element of the JSON data";

    for (QVariantList::iterator it = j_root.begin(); it != j_root.end(); ++it)
    {
        QVariantMap j_scenario = (*it).toMap();

        if (j_scenario["device"] == device)
        {
            loadDeviceScenario(j_scenario);
            return true;
        }
    }

    return false;
}
//-------------------------------------------------
void FlashScenario::loadDeviceScenario(QVariantMap data)
{
    if (mScenario) delete mScenario;
    mScenario = new ScenarioData;

    QVariantMap j_unlock = data["unlock"].toMap();
    QVariantMap j_recovery = data["recovery"].toMap();

    mScenario->device = data["device"].toString();
    mScenario->recoveryUrl = data["recoveryImage"].toString();
    mScenario->unlockStep.method = getStepFromType(j_unlock["method"].toString());
    mScenario->unlockStep.commands = j_unlock["commands"].toStringList();
    mScenario->recoveryStep.method = getStepFromType(j_recovery["method"].toString());
    mScenario->recoveryStep.commands = j_recovery["commands"].toStringList();

    qDebug() << "Loaded scenario for " << mScenario->device;
}
//-------------------------------------------------
AbstractStep* FlashScenario::getStepFromType(const QString &type)
{
    if (type == "fastboot")
    {
        return new FastbootStep();
    }
    else if (type == "null")
    {
        return new NullStep();
    }
    else
    {
        qDebug() << "Unknown step method name: " << type;
        return NULL;
    }
}
//-------------------------------------------------
bool FlashScenario::flash(BundleBuild *build, const QStringList &files)
{
    if (!build)
    {
        qDebug() << "Oops, build is NULL!";
        return false;
    }

    // Load the scenario
    if ((mScenario == NULL || mScenario->device != build->getDevice())
            && !loadForDevice(build->getDevice()))
    {
        qDebug() << "Unable to load the scenario for device " << build->getDevice();
        return false;
    }

    mBuild = build;
    mFiles = files;

    // Start flashing! First, we unlock the device.
    connect(mScenario->unlockStep.method, SIGNAL(stepPrepared()),
            this, SLOT(onFlashStep_UnlockReady()));
    connect(mScenario->unlockStep.method, SIGNAL(stepEnded(QString)),
            this, SLOT(onFlashStep_UnlockComplete()));

    mScenario->unlockStep.method->runStep(mScenario->unlockStep.commands);

    return true;
}
//-------------------------------------------------
void FlashScenario::onFlashStep_UnlockReady()
{
    disconnect(mScenario->unlockStep.method, SIGNAL(stepPrepared()),
              this, SLOT(onFlashStep_UnlockReady()));
    qDebug() << "The unlock method is ready / has been prepared.";
}
//-------------------------------------------------
void FlashScenario::onFlashStep_UnlockComplete()
{
    disconnect(mScenario->unlockStep.method, SIGNAL(stepEnded(QString)),
              this, SLOT(onFlashStep_UnlockComplete()));

    qDebug() << "The unlock is complete. Proceeding to recovery flashing";

    connect(mScenario->recoveryStep.method, SIGNAL(stepPrepared()),
            this, SLOT(onFlashStep_RecoveryReady()));
    connect(mScenario->recoveryStep.method, SIGNAL(stepEnded(QString)),
            this, SLOT(onFlashStep_RecoveryComplete()));

    // Put args in for recovery
    // %1 : recovery.img path
    QStringList filledCommands = mScenario->recoveryStep.commands;
    filledCommands.replaceInStrings("%1", mScenario->recoveryLocalPath);

    mScenario->recoveryStep.method->runStep(filledCommands);
}
//-------------------------------------------------
void FlashScenario::onFlashStep_RecoveryReady()
{
    disconnect(mScenario->unlockStep.method, SIGNAL(stepPrepared()), this,
               SLOT(onFlashStep_RecoveryReady()));
    qDebug() << "The recovery is ready to be flashed.";
}
//-------------------------------------------------
void FlashScenario::onFlashStep_RecoveryComplete()
{
    disconnect(mScenario->unlockStep.method, SIGNAL(stepEnded(QString)), this,
               SLOT(onFlashStep_RecoveryComplete()));

    qDebug() << "Recovery flashing is complete, and the device should be booting the recovery.";

    // We now wait for our new recovery ADB to be up, unless it is already
    AdbMonitor* am = AdbMonitor::getDefault();
    if (am->hasDeviceConnected())
    {
        onFlashStep_InitialAdbReady();
    }
    else
    {
        connect(am, SIGNAL(onDeviceConnected(AdbDevice*)),
                this, SLOT(onFlashStep_InitialAdbReady()));
    }
}
//-------------------------------------------------
void FlashScenario::onFlashStep_InitialAdbReady()
{
    // Once ADB is up, all we need to do is install the ROM (and additional packages, if needed).
    // To do that easily, we're going to use 'adb sideload', but the recovery by default cannot
    // run in sideload mode all the time, as it takes over the normal ADB server/protocol. What
    // we do is that we write the command NOW to /cache, and kill the recovery to make it go into
    // sideload mode.
    AdbMonitor* am = AdbMonitor::getDefault();
    disconnect(am, SIGNAL(onDeviceConnected(AdbDevice*)), this, SLOT(onFlashStep_InitialAdbReady()));

    // We write the command to /cache
    qDebug() << "Doing sideload cache command";
    am->shell("echo \"sideload\" > /cache/recovery/openrecoveryscript", true);

    // We kill the recovery to reload it, and make it read our command file
    qDebug() << "Killing recovery to start in sideload mode";
    am->shell("killall recovery", true);

    onFlashStep_SideloadAdbReady();
}
//-------------------------------------------------
void FlashScenario::onFlashStep_SideloadAdbReady()
{
    // We're in sideload mode now, we can flash the files
    AdbMonitor* am = AdbMonitor::getDefault();

    for (QStringList::iterator it = mFiles.begin(); it != mFiles.end(); ++it)
    {
        qDebug() << "Starting sideload of " << *it;
        am->sideload(*it);
    }

    qDebug() << "Sideloaded all files";

    // Try to reboot the device once sideload is done
    // We wait for device to exit sideload mode
    connect(am, SIGNAL(onDeviceConnected(AdbDevice*)), this, SLOT(onFlashStep_SideloadComplete()));
}
//-------------------------------------------------
void FlashScenario::onFlashStep_SideloadComplete()
{
    AdbMonitor* am = AdbMonitor::getDefault();
    disconnect(am, SIGNAL(onDeviceConnected(AdbDevice*)), this, SLOT(onFlashStep_SideloadComplete()));

    qDebug() << "Rebooting to system";
    am->reboot();
}

//-------------------------------------------------
