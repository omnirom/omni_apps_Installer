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
    // Remote path for recovery image
    QString recoveryUrl;
    // Local path
    QString recoveryLocalPath;
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
     * @param files The path to the local files to flash
     * @return true if success, false otherwise
     */
    bool flash(BundleBuild* build, const QStringList& files);

    /**
     * @brief getScenarioData returns the data of the currently loaded scenario
     * @return  A pointer to the data, or NULL if no scenario loaded (@see loadForDevice)
     */
    ScenarioData* getScenarioData() const { return mScenario; }

private slots:
    // Slot to call when unlock step is ready to be performed
    void onFlashStep_UnlockReady();
    // Slot to call when unlock is complete
    void onFlashStep_UnlockComplete();
    // Slot to call when recovery step is ready to be performed
    void onFlashStep_RecoveryReady();
    // Slot to call when recovery flash/boot is complete
    void onFlashStep_RecoveryComplete();
    // Slot to call when initial ADB step is up
    void onFlashStep_InitialAdbReady();
    // Slot to call when sideload ADB step is up
    void onFlashStep_SideloadAdbReady();
    // Slot to call when sideload is complete and ADB is back up
    void onFlashStep_SideloadComplete();

protected:
    void loadDeviceScenario(QVariantMap data);
    AbstractStep* getStepFromType(const QString& type);

    ScenarioData* mScenario;
    BundleBuild* mBuild;
    QStringList mFiles;
};

#endif // FLASHSCENARIO_H
