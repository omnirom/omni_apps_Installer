#include "FlashScenario.h"
#include "QJson/json_parser.hh"

#include "flasher/AbstractStep.h"
#include "flasher/FastbootStep.h"
#include "BundleManager.h"
#include "AdbMonitor.h"

#include <QFile>
#include <QMessageBox>

//-------------------------------------------------
FlashScenario::FlashScenario(QObject *parent) :
    QObject(parent), mScenario(NULL)
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
    else
    {
        qDebug() << "Unknown step method name: " << type;
        return NULL;
    }
}
//-------------------------------------------------
bool FlashScenario::flash(BundleBuild *build)
{
    if (!build)
    {
        qDebug() << "Oops, build is NULL!";
        return false;
    }

    // Load the scenario
    if (!loadForDevice(build->getDevice()))
    {
        qDebug() << "Unable to load the scenario for device " << build->getDevice();
        return false;
    }

    // Start flashing! First reboot, to bootloader my dear.
    AdbMonitor::getDefault()->reboot("bootloader");
}
//-------------------------------------------------
