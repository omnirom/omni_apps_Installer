#include "BundleManager.h"

#include "QJson/Parser"
#include <QMessageBox>
#include <QVariant>
#include <QDebug>

BundleManager* BundleManager::sDefault = NULL;

//-----------------------------------------
BundleRom::BundleRom(const QString &name, const QString &androidVersion, const QString &iconUrl) :
    mName(name), mAndroidVersion(androidVersion), mIconUrl(iconUrl)
{

}
//-----------------------------------------
BundleRom::~BundleRom()
{
    for (QList<BundleBuild*>::iterator it = mBuilds.begin(); it != mBuilds.end(); ++it)
    {
        delete (*it);
    }
    mBuilds.clear();
}
//-----------------------------------------
void BundleRom::addBuild(BundleBuild *build)
{
    mBuilds.push_back(build);

    if (!mSupportedDevices.contains(build->getDevice()))
    {
        mSupportedDevices.push_back(build->getDevice());
    }
}
//-----------------------------------------
bool BundleRom::isDeviceSupported(const QString &device)
{
    return mSupportedDevices.contains(device);
}
//-----------------------------------------
//-----------------------------------------
Bundle::~Bundle()
{
    for (QList<BundleRom*>::iterator it = mRoms.begin(); it != mRoms.end(); ++it)
    {
        delete (*it);
    }
    mRoms.clear();
}
//-----------------------------------------
bool Bundle::isDeviceSupported(const QString &name)
{
    for (QList<BundleRom*>::iterator it = mRoms.begin(); it != mRoms.end(); ++it)
    {
        BundleRom* rom = (*it);
        if (rom->isDeviceSupported(name))
        {
            // We don't need to go any further
            return true;
        }
    }

    return false;
}
//-----------------------------------------
QList<BundleRom*> Bundle::getSupportedRoms(const QString& device)
{
    QList<BundleRom*> supported;
    for (QList<BundleRom*>::iterator it = mRoms.begin(); it != mRoms.end(); ++it)
    {
        BundleRom* rom = (*it);
        if (rom->isDeviceSupported(device))
        {
            supported.push_back(rom);
        }
    }

    return supported;
}

//-----------------------------------------
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
                    "\"name\": \"Omni\",\"androidVersion\": \"4.3\",\"icon\": \"http://placekitten.com/g/200/300\","
                    "\"builds\":["
                        "{\"device\":\"mako\",\"timestamp\":1273732894,\"type\":\"nightly\",\"name\":\"Nightly for Galaxy S II (i9100)\",\"description\":\"some build\",\"url\":\"http://download.omnirom.org/nightly/i9300-20130929.zip\"}"
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

    // Parse initial bundle info
    mCurrentBundle = new Bundle(j_root["name"].toString(), j_root["url"].toString(),
            j_root["schemeVersion"].toInt());


    // Parse ROMs
    QVariantList j_roms = j_root["roms"].toList();
    for (QVariantList::iterator it = j_roms.begin(); it != j_roms.end(); ++it)
    {
        QVariantMap j_rom = (*it).toMap();

        BundleRom* rom = new BundleRom(j_rom["name"].toString(), j_rom["androidVersion"].toString(),
                j_rom["icon"].toString());

        // Parse builds for that ROM
        QVariantList j_builds = j_rom["builds"].toList();
        for (QVariantList::iterator buildIt = j_builds.begin(); buildIt != j_builds.end(); ++buildIt)
        {
            QVariantMap j_build = (*buildIt).toMap();

            BundleBuild* build = new BundleBuild(j_build["name"].toString(),
                    j_build["description"].toString(), j_build["url"].toString(),
                    j_build["device"].toString(),
                    QDateTime::fromMSecsSinceEpoch(j_build["timestamp"].toULongLong()*1000L),
                    buildTypeNameToEnum(j_build["type"].toString()));

            rom->addBuild(build);
        }

        mCurrentBundle->addRom(rom);
    }
}
//-----------------------------------------
Bundle* BundleManager::getCurrentBundle() const
{
    return mCurrentBundle;
}
//-----------------------------------------
BuildType BundleManager::buildTypeNameToEnum(const QString& name)
{
    if (name == BUILD_TYPE_EXPERIMENTAL_STR)
        return BUILD_TYPE_EXPERIMENTAL;
    else if (name == BUILD_TYPE_NIGHTLY_STR)
        return BUILD_TYPE_NIGHTLY;
    else if (name == BUILD_TYPE_VERIFIED_STR)
        return BUILD_TYPE_VERIFIED;
    else if (name == BUILD_TYPE_RELEASE_CANDIDATE_STR)
        return BUILD_TYPE_RELEASE_CANDIDATE;
    else if (name == BUILD_TYPE_STABLE_STR)
        return BUILD_TYPE_STABLE;
    else
        return BUILD_TYPE_EXPERIMENTAL; // unknown tags are reported as experimental
}
//-----------------------------------------
