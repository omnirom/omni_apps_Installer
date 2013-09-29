#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>

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
    QString downloadUrl(const QString& url);

protected:

};


#endif // HTTPCLIENT_H
