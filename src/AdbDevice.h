#ifndef ADBDEVICE_H
#define ADBDEVICE_H

#include <QString>

class AdbDevice
{
public:
    explicit AdbDevice(const QString& serial);

    void setAuthorized(bool authorized);
    void setProduct(const QString& product);
    void setModel(const QString& model);
    void setDevice(const QString& device);

    /**
     * @brief isAuthorized
     * @return Computer allowed to access ADB on device
     */
    bool isAuthorized() const;

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
    bool mIsAuthorized;
};

#endif // ADBDEVICE_H
