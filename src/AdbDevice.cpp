#include "AdbDevice.h"
#include <QDebug>

//------------------------------------------
AdbDevice::AdbDevice(const QString &serial) :
    mSerial(serial), mState(ADB_DEVICE_STATE_OFFLINE)
{

}
//------------------------------------------
void AdbDevice::setState(AdbDeviceState state)
{
    mState = state;
}
//------------------------------------------
void AdbDevice::setDevice(const QString& device)
{
    mDevice = device;
}
//------------------------------------------
void AdbDevice::setModel(const QString& model)
{
    mModel = model;
}
//------------------------------------------
void AdbDevice::setProduct(const QString& product)
{
    mProduct = product;
}
//------------------------------------------
QString AdbDevice::getSerial() const
{
    return mSerial;
}
//------------------------------------------
QString AdbDevice::getDevice() const
{
    return mDevice;
}
//------------------------------------------
QString AdbDevice::getModel() const
{
    return mModel;
}
//------------------------------------------
QString AdbDevice::getProduct() const
{
    return mProduct;
}
//------------------------------------------
AdbDeviceState AdbDevice::getState() const
{
    return mState;
}
//------------------------------------------
void AdbDevice::dump()
{
    qDebug() << "Serial: " << mSerial;
    qDebug() << "State: " << mState;
    qDebug() << "Product: " << mProduct;
    qDebug() << "Model: " << mModel;
    qDebug() << "Device: " << mDevice;
}

//------------------------------------------
