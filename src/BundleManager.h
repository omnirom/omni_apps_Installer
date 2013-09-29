#ifndef BUNDLEMANAGER_H
#define BUNDLEMANAGER_H

#include <QString>
#include <QStringList>
#include <QDateTime>

enum BuildType
{
    // Experimental build, generally done manually by a developer
    BUILD_TYPE_EXPERIMENTAL,

    // Nightly build, done automatically, but not verified to be working properly
    BUILD_TYPE_NIGHTLY,

    // Verified build, similar to nightly, but proven to be working "properly"
    BUILD_TYPE_VERIFIED,

    // Stable build
    BUILD_TYPE_STABLE
};

/**
 * @brief The BundleBuild class represents a build available for a specific ROM
 */
class BundleBuild
{
public:
    BundleBuild(const QString& title, const QString& description, const QString& downloadUrl,
                const QString& device, const QDateTime& date, BuildType type) :
        mTitle(title), mDescription(description), mDownloadUrl(downloadUrl), mDevice(device),
        mDate(date), mBuildType(type) { }


    QString getTitle() const { return mTitle; }
    QString getDescription() const { return mDescription; }
    QString getDownloadUrl() const { return mDownloadUrl; }
    QString getDevice() const { return mDevice; }
    QDateTime getDate() const { return mDate; }
    BuildType getBuildType() const { return mBuildType; }

protected:
    QString mTitle;
    QString mDescription;
    QString mDownloadUrl;
    QString mDevice;
    QDateTime mDate;
    BuildType mBuildType;
};


/**
 * @brief The BundleRom class represents a ROM available in a downloaded bundle, and stores
 *        the builds it handles.
 */
class BundleRom
{
public:
    BundleRom(const QString& name) : mName(name) { }

    QString getName() const { return mName; }

    void addBuild(BundleBuild* build);

protected:
    QString mName;
    QStringList mSupportedDevices;
    QList<BundleBuild*> mBuilds;

};

/**
 * @brief The Bundle class contains all the information provided by the downloaded bundle.
 */
class Bundle
{
public:
    Bundle(const QString& name, const QString& url, int version) : mName(name), mUrl(url),
        mVersion(version) { }

    /**
     * @brief getProviderName
     * @return The name of the bundle provider (e.g. 'XDA-Developers')
     */
    QString getProviderName() const { return mName; }

    /**
     * @brief getSchemeVersion
     * @return The version of the scheme (e.g. 1)
     */
    int getSchemeVersion() const { return mVersion; }

    /**
     * @brief getPublicUrl
     * @return The public URL that users can browse
     */
    QString getPublicUrl() const { return mUrl; }

    /**
     * @brief isDeviceSupported
     * @param name The device codename (e.g. 'mako' or 'find5')
     * @return True if the device is supported by at least one build provided by the bundle
     */
    bool isDeviceSupported(const QString& name);

    /**
     * @brief getSupportedRoms
     * @return A list of the supported ROMs
     */
    QStringList getSupportedRoms();

protected:
    QString mName;
    QString mUrl;
    int mVersion;

    QList<BundleRom> mRoms;
};


/**
 * @brief The BundleManager class manages the current Bundle, or downlads it from a remote URL
 */
class BundleManager
{
protected:
    explicit BundleManager();

public:
    static BundleManager* getDefault();

    /**
     * @brief fetchBundle Retrieves bundle JSON information from a remote server
     * @param url The URL to fetch and parse
     */
    void fetchBundle(const QString& url);

    /**
     * @brief getCurrentBundle
     * @return Returns current bundle (or null if none loaded)
     */
    Bundle* getCurrentBundle() const;

protected:
    void parseBundle(const QString& data);

    static BundleManager* sDefault;

    Bundle* mCurrentBundle;

};

#endif // BUNDLEMANAGER_H
