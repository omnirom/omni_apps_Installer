#ifndef BUNDLEMANAGER_H
#define BUNDLEMANAGER_H

#include <QString>

/**
 * @brief The BundleRom class represents a ROM available in a downloaded bundle.
 */
class BundleRom
{
public:
    BundleRom(const QString& name);

    /**
     * @brief addSupportedDevice adds a device to the supported list
     * @param device Code of the device (e.g. 'mako')
     */
    void addSupportedDevice(const QString& device);

protected:
    QStringList mSupportedDevices;
};

/**
 * @brief The Bundle class contains all the information provided by the downloaded bundle.
 */
class Bundle
{
public:
    Bundle(const QString& name, const QString& url, int version);

protected:
    QString mName;
    QString mUrl;
    int mVersion;

    QList<BundleRom> mRoms;
};

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
     * @brief getProviderName
     * @return The name of the bundle provider (e.g. 'XDA-Developers')
     */
    QString getProviderName() const;

    /**
     * @brief getSchemeVersion
     * @return The version of the scheme (e.g. 1)
     */
    int getSchemeVersion() const;

    /**
     * @brief getPublicUrl
     * @return The public URL that users can browse
     */
    QString getPublicUrl() const;

    /**
     * @brief isDeviceSupported
     * @param name The device codename (e.g. 'mako' or 'find5')
     * @return True if the device is supported by at least one build provided by the provider
     */
    bool isDeviceSupported(const QString& name);

    /**
     * @brief getSupportedRoms
     * @return A list of the supported ROMs
     */
    QStringList getSupportedRoms();

protected:
    void parseBundle(const QString& data);

    static BundleManager* sDefault;

    Bundle* mCurrentBundle;

};

#endif // BUNDLEMANAGER_H
