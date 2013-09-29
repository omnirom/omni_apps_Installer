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
