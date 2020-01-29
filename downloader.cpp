#include "downloader.h"
#include <QUrlQuery>

Downloader::Downloader(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
}

void Downloader::getData(QString urlStr, QByteArray user,
                         QMap<QString, QString> parametersUrl)
{
    //заносим атрибуты URL
    QList<QString> parametersKeys = parametersUrl.keys();
    for(int i = 0; i < parametersKeys.size(); i++)
    {
        if(urlStr[urlStr.length() - 1] == '/')
            urlStr += '?' + parametersKeys[i] + '=' + parametersUrl[parametersKeys[i]];
        else urlStr += '&' + parametersKeys[i] + '=' + parametersUrl[parametersKeys[i]];
    }

    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);

    //авторизация
    if(user != "")
    {
        QByteArray headerData = "Basic " + user.toBase64();
        request.setRawHeader("Authorization", headerData);
    }

    manager->get(request);
}

void Downloader::postData(QString urlStr, QByteArray user,
                          QMap<QString, QString> parametersUrl,
                          QMap<QString, QString> parametersBody)
{
    //заносим атрибуты URL
    QList<QString> parametersKeys = parametersUrl.keys();
    for(int i = 0; i < parametersKeys.size(); i++)
    {
        if(urlStr[urlStr.length() - 1] == '/')
            urlStr += '?' + parametersKeys[i] + '=' + parametersUrl[parametersKeys[i]];
        else urlStr += '&' + parametersKeys[i] + '=' + parametersUrl[parametersKeys[i]];
    }

    QUrl url(urlStr);
    QNetworkRequest request;
    request.setUrl(url);

    //авторизация
    if(user != "")
    {
        QByteArray headerData = "Basic " + user.toBase64();
        request.setRawHeader("Authorization", headerData);
    }

    //добавление параметров в тело POST запроса
    QUrlQuery params;

    parametersKeys = parametersBody.keys();

    for(int i = 0; i < parametersKeys.size(); i++)
    {
        if(parametersBody[parametersKeys[i]] != "")
            params.addQueryItem(parametersKeys[i], parametersBody[parametersKeys[i]]);
    }

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    manager->post(request, params.toString().toLocal8Bit());
}

void Downloader::onResult(QNetworkReply* reply)
{
    if (reply->error()) {
//        qDebug() << "ERROR";
//        qDebug() << reply->errorString() << endl << reply->error();
        emit onError(reply->url().toString(), reply->errorString(), reply->error());
    }
    else {
//        qDebug() << "OK";
        emit onReady(reply->url().toString(), reply->readAll());
    }
}
