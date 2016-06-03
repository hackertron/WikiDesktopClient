#include "dbmanager.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>



dbmanager::dbmanager(QObject *parent) : QObject(parent)
{

}

void dbmanager::add()
{



    // create custom temporary event loop on stack
       QEventLoop eventLoop;

       // "quit()" the event-loop, when the network request "finished()"
       QNetworkAccessManager mgr;
       QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

       // the HTTP request
       QNetworkRequest req( QUrl( QString("http://en.wikitolearn.org/api.php?action=parse&page=Linear%20Algebra/Sets&format=json") ) );
       QNetworkReply *reply = mgr.get(req);
       eventLoop.exec();

       if (reply->error() == QNetworkReply::NoError) {
           //success
           //qDebug() << "Success" <<reply->readAll();
          QString   html = (QString)reply->readAll();
          QJsonDocument jsonResponse = QJsonDocument::fromJson(html.toUtf8());

          QJsonObject jsonObj = jsonResponse.object();


         qDebug() << jsonResponse.object()["parse"].toObject()["text"].toObject()["*"].toString();
         delete reply;
       }
       else {
           //failure
           qDebug() << "Failure" <<reply->errorString();
           delete reply;
       }

    QDir databasePath;
    QString path = databasePath.currentPath()+"WTL.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    if(!db.open())
    {
        qDebug() <<"error in opening DB";
    }
    else
    {
        qDebug() <<"connected to DB" ;
    }
    QSqlQuery query;

    if( query.exec(""))
    {
        qDebug() << "PAGE ADDED successfully";
    }
    else
    {
     qDebug() <<query.lastError();
    }

}

void dbmanager::del()
{
    qDebug() <<"DELETION CODE GOES HERE";
}

