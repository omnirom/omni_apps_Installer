#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject* parent = 0);

    /**
     * @brief downloadUrl Downloads the specified URL data
     * @param url The URL
     * @return The data of the file, in a QString
     */
    QNetworkReply* downloadUrl(const QString& url);

private slots:
    void httpFinished(QNetworkReply* reply);

signals:
    void onDownloadFinished(QNetworkReply* reply);

protected:
    QNetworkAccessManager mNetworkManager;

};


#endif // HTTPCLIENT_H
