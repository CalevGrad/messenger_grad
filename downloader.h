#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject* parent = 0);
    ~Downloader() { }

signals:
    void onReady(QString, QByteArray);
    void onError(QString, QString, QNetworkReply::NetworkError);

public slots:
    void getData(QString urlStr, QByteArray user, QMap<QString, QString> parametersUrl);
    void postData(QString urlStr, QByteArray user, QMap<QString, QString> parametersUrl, QMap<QString, QString> parametersBody);
    void onResult(QNetworkReply* reply);

private:
    QNetworkAccessManager *manager;
};

#endif // DOWNLOADER_H
