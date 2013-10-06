#ifndef ADBMONITOR_H
#define ADBMONITOR_H

#include <QObject>
#include <QProcess>

class AdbDevice;
class AdbMonitor : public QObject
{
    Q_OBJECT
protected:
    explicit AdbMonitor(QObject *parent = 0);
    virtual ~AdbMonitor();

public:
    static AdbMonitor* getDefault();
    void killDefault();

    /**
     * @brief hasDeviceConnected
     * @return true if a device is in an online state
     */
    bool hasDeviceConnected();

    /**
     * @brief killServer Kills ADB server
     */
    void killServer();

    /**
     * @brief startServer Starts ADB server
     */
    void startServer();

    /**
     * @brief pushFile Pushes the specified file to the device
     *
     * @param srcPath The source path on the computer
     * @param destPath The destination path on the device
     * @param deviceSerial The serial number of the device, in case
     *                     multiple devices are connected (or empty
     *                     if not needed).
     */
    void pushFile(const QString& srcPath,
                  const QString& destPath,
                  const QString& deviceSerial = "");

    /**
     * @brief pullFile Pulls a file from the device
     *
     * @param remotePath The path on the device
     * @param localPath The destination path on the computer
     * @param deviceSerial The serial number of the device, in case
     *                     multiple devices are connected (or empty
     *                     if not needed).
     */
    void pullFile(const QString& remotePath,
                  const QString& localPath,
                  const QString& deviceSerial = "");

    /**
     * @brief reboot Reboots the device to the specified destination mode
     * @param destination The destination to reboot to (e.g. "recovery"), or empty for a normal
     *                    reboot
     */
    void reboot(const QString& destination = "");

    /**
     * @brief shell Runs an "adb shell" command followed by the provided parameters.
     * @param parameters The command/parameters to run adb shell, knowing that it is escaped
     * with simple quotes.
     * @param blocking True to wait for the process to finish
     */
    void shell(const QString& parameters, bool blocking);

    /**
     * @brief sideload Sideloads the provided local ZIP path to the connected device
     * @param path The path to the ZIP file, on the host computer
     */
    void sideload(const QString& path);

    void timerEvent(QTimerEvent* evt);
signals:
    /**
     * Signal emitted when a device is plugged in
     * ADB mode.
     */
    void onDeviceConnected(AdbDevice*);

    /**
     * Signal emitted when a device is disconnected.
     */
    void onDeviceDisconnected(AdbDevice*);

    /**
     * Signal emitted when adb is running
     */
    void onAdbReady();

public slots:
    void onAdbDevicesReadyReadStdOut();
    void onAdbDevicesReadyReadStdErr();

private:
    void startMonitorService();
    void processAdbDevice(const QString& serial, const QString& deviceLine);
    AdbDevice* findDevice(const QString& serial);

    static AdbMonitor* sDefault;
    int mMonitorTimerId;
    QProcess* mMonitorProcess;
    QList<AdbDevice*> mDevices;

};

#endif // ADBMONITOR_H
