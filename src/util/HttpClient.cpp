#include "HttpClient.h"

//----------------------------------------------------------------------
HttpClient::HttpClient(QObject* parent) : QObject(parent)
{
    connect(&mNetworkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(httpFinished(QNetworkReply*)));
}
//----------------------------------------------------------------------
QNetworkReply* HttpClient::downloadUrl(const QString &url)
{
    QNetworkRequest req(url);
    return mNetworkManager.get(req);
}
//----------------------------------------------------------------------
void HttpClient::httpFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    emit onDownloadFinished(reply, QString(data));
}
//----------------------------------------------------------------------
