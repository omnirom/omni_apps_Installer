#include "AdbDevice.h"
#include <QDebug>

//------------------------------------------
AdbDevice::AdbDevice(const QString &serial) :
    mSerial(serial), mIsAuthorized(false)
{

}
//------------------------------------------
void AdbDevice::setAuthorized(bool authorized)
{
    mIsAuthorized = authorized;
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
bool AdbDevice::isAuthorized() const
{
    return mIsAuthorized;
}
//------------------------------------------
void AdbDevice::dump()
{
    qDebug() << "Serial: " << mSerial;
    qDebug() << "Authorized: " << mIsAuthorized;
    qDebug() << "Product: " << mProduct;
    qDebug() << "Model: " << mModel;
    qDebug() << "Device: " << mDevice;
}

//------------------------------------------
