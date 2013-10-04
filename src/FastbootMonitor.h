#ifndef FASTBOOTMONITOR_H
#define FASTBOOTMONITOR_H

#include <QObject>
#include <QProcess>

class FastbootMonitor : public QObject
{
    Q_OBJECT

protected:
    explicit FastbootMonitor(QObject* parent = 0);

public:
    static FastbootMonitor* getDefault();

    void startMonitoring();
    void stopMonitoring();
    void timerEvent(QTimerEvent *evt);

signals:
    void onFastbootOnline();
    void onFastbootOffline();

private slots:
    void onMonitorStdOut();

private:
    static FastbootMonitor* sDefault;

    int mMonitorTimerId;
    QProcess* mMonitorProcess;
    QString mDeviceSerial;
};

#endif // FASTBOOTMONITOR_H
