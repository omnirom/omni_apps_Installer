#include "BundleManager.h"

#include "QJson/Parser"
#include <QMessageBox>
#include <QVariant>
#include <QDebug>

BundleManager* BundleManager::sDefault = NULL;

//-----------------------------------------
BundleManager::BundleManager() : mCurrentBundle(NULL)
{

}
//-----------------------------------------
BundleManager* BundleManager::getDefault()
{
    if (sDefault == NULL) sDefault = new BundleManager();
    return sDefault;
}
//-----------------------------------------
void BundleManager::fetchBundle(const QString &url)
{
    // TODO: Fetch from web
    QString bundleData = "{"
            "\"name\": \"XDA-Developers\",\"schemeVersion\": 1,\"url\": \"http://www.xda-developers.com\","
            "\"roms\":["
                "{"
                    "\"name\": \"Omni\",\"androidVersion\": \"4.3\",\"supportedDevices\": [\"i9100\", \"i9300\", \"n7000\", \"mako\", \"flo\", \"grouper\"],"
                    "\"builds\":["
                        "{\"device\":\"i9100\",\"timestamp\":1273732894,\"type\":\"nightly\",\"name\":\"Nightly for Galaxy S II (i9100)\",\"url\":\"http://download.omnirom.org/nightly/i9300-20130929.zip\"}"
                    "]"
                "}"
            "]}";

    // Parse JSON data
    parseBundle(bundleData);
}
//-----------------------------------------
void BundleManager::parseBundle(const QString &data)
{
    // We parse the bundle, which is JSON data. For more information about the JSON layout,
    // please read the "API.txt" file, or go to http://api.xda-developers.com/bundle to get
    // a sample of the one used by default.
    QJson::Parser parser;
    bool ok;

    QVariantMap j_root = parser.parse(data.toUtf8(), &ok).toMap();

    if (!ok) qDebug() << "Unable to parse root element of the JSON data";

    if (mCurrentBundle) delete mCurrentBundle;

    mCurrentBundle = new Bundle(j_root["name"].toString(), j_root["url"].toString(),
            j_root["schemeVersion"].toInt());


}
//-----------------------------------------
Bundle* BundleManager::getCurrentBundle() const
{
    return mCurrentBundle;
}
//-----------------------------------------
