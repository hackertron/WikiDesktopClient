#include "downloader.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
}

static QString fname ;

void Downloader::doDownload(QString &url)
{
    fname = url ;
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(url
)));
}

void Downloader::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {
         qDebug() << reply->readAll();

        QString filename =  fname;
        QFile *file = new QFile(filename);
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;
    }

    reply->deleteLater();
}
