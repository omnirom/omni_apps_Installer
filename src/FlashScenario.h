#ifndef FLASHSCENARIO_H
#define FLASHSCENARIO_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QString>

class AbstractStep;
class BundleBuild;

struct ScenarioStep
{
    AbstractStep* method;
    QStringList commands;
};

/**
 * @brief The ScenarioData struct contains all the data for the flashing scenario of a device.
 *
 * Please note that we only care about unlocking, and flashing a recovery. Once we're there,
 * we can simply use the same method for everyone, through adb, so we don't need a device-specific
 * scenario from there (I think).
 */
struct ScenarioData
{
    // Device code (e.g. 'mako')
    QString device;
    // Data for unlock step
    ScenarioStep unlockStep;
    // Data for recovery flashing step
    ScenarioStep recoveryStep;
};

class FlashScenario : public QObject
{
    Q_OBJECT

public:
    explicit FlashScenario(QObject *parent = 0);
    ~FlashScenario();

    /**
     * @brief existsForDevice tells whether or not the provided device code has a flash scenario
     * @param device The code of the device
     * @return true if a scenario exists, false otherwise
     */
    static bool existsForDevice(const QString& device);

    /**
     * @brief loadForDevice loads the flashing scenario for the specified device code name
     * @param device The code of the device that will be flashed (e.g. 'mako')
     * @return true if the scenario has been loaded, false in case of errors
     */
    bool loadForDevice(const QString& device);

    /**
     * @brief flash flashes the provided BundleBuild to the currently connected device
     * @param build The build to flash
     * @return true if success, false otherwise
     */
    bool flash(BundleBuild* build);

private slots:
    // Slot to call when the initial reboot has been initiated
    void onFlashStep_InitialReboot();
    // Slot to call when unlock step is ready to be performed
    void onFlashStep_UnlockReady();
    // Slot to call when unlock is complete
    void onFlashStep_UnlockComplete(QString);
    // Slot to call when post-unlock reboot has been initiated
    void onFlashStep_PostUnlockReboot();
    // Slot to call when recovery step is ready to be performed
    void onFlashStep_RecoveryReady();
    // Slot to call when recovery flash/boot is complete
    void onFlashStep_RecoveryComplete();
    // Slot to call when initial ADB step is up
    void onFlashStep_InitialAdbReady();
    // Slot to call when sideload ADB step is up
    void onFlashStep_SideloadAdbReady();

protected:
    void loadDeviceScenario(QVariantMap data);
    AbstractStep* getStepFromType(const QString& type);

    ScenarioData* mScenario;
};

#endif // FLASHSCENARIO_H
