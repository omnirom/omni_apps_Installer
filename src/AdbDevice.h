#ifndef ADBDEVICE_H
#define ADBDEVICE_H

#include <QString>

enum AdbDeviceState
{
    ADB_DEVICE_STATE_OFFLINE,
    ADB_DEVICE_STATE_UNAUTHORIZED,
    ADB_DEVICE_STATE_ONLINE,
    ADB_DEVICE_STATE_RECOVERY
};

class AdbDevice
{
public:
    explicit AdbDevice(const QString& serial);

    void setState(AdbDeviceState state);
    void setProduct(const QString& product);
    void setModel(const QString& model);
    void setDevice(const QString& device);

    /**
     * @brief getState
     * @return State of the device
     */
    AdbDeviceState getState() const;

    /**
     * @brief getSerial
     * @return Device serial number
     */
    QString getSerial() const;

    /**
     * @brief getProduct
     * @return Device product name (e.g. 'occam')
     */
    QString getProduct() const;

    /**
     * @brief getModel
     * @return Device model name (e.g. 'Nexus_4')
     */
    QString getModel() const;

    /**
     * @brief getDevice
     * @return Device name (e.g. 'mako')
     */
    QString getDevice() const;

    void dump();

private:
    QString mSerial;
    QString mProduct;
    QString mModel;
    QString mDevice;
    AdbDeviceState mState;
};

#endif // ADBDEVICE_H
