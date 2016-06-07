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
#include <QRegularExpression>
#include <QString>
#include <QTextStream>





dbmanager::dbmanager(QObject *parent) : QObject(parent)
{

}

bool add_in_db(int pageid , int revid)
{
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

    query.prepare("INSERT INTO pages (page_ID,page_revision) "
                     "VALUES (? , ?)");
       query.bindValue(0,pageid);
       query.bindValue(1, revid);

       if(query.exec())
       {
           qDebug() << "done";
           return(true);
       }
       else
       {
           qDebug() << query.lastError();

       }
       return (false);
}


void dbmanager::add()
{

    QString text ;
    int pageid , revid;


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


          text = jsonResponse.object()["parse"].toObject()["text"].toObject()["*"].toString();
          pageid = jsonResponse.object()["parse"].toObject()["pageid"].toInt();
          revid = jsonResponse.object()["parse"].toObject()["revid"].toInt();
         text = text.replace("\n","");
         text = text.replace("&#39;/index.php", "http://en.wikitolearn.org/index.php");
         text = text.replace("&amp;","&");
         text = text.replace("MathShowImage&amp;", "MathShowImage&");
         text = text.replace("mode=mathml&#39;", "mode=mathml""");
         text = text.replace("<meta class=\"mwe-math-fallback-image-inline\" aria-hidden=\"true\" style=\"background-image: url(" ,"<img style=\"background-repeat: no-repeat; background-size: 100% 100%; vertical-align: -0.838ex;height: 2.843ex;\""   "src=");
         text = text.replace("<meta class=\"mwe-math-fallback-image-display\" aria-hidden=\"true\" style=\"background-image: url(" ,"<img style=\"background-repeat: no-repeat; background-size: 100% 100%; vertical-align: -0.838ex;height: 2.843ex;\""  "src=");
         text = text.replace("&amp;mode=mathml\"" , "&mode=mathml>\"");
       //  qDebug() << text;
         qDebug() <<pageid;


         delete reply;
       }
       else {
           //failure
           qDebug() << "Failure" <<reply->errorString();
           delete reply;
       }
       QDir dir;
       QString Folder_name = QString::number(pageid);
       if(QDir(Folder_name).exists())
       {
        qDebug() << " already exist ";

       }
       else{
           dir.mkdir(Folder_name);

           QDir dr(Folder_name);
           QString filename = Folder_name+".html";
           QFile file(filename);
             file.open(QIODevice::WriteOnly | QIODevice::Text);
             QTextStream out(&file);
             out << text;

             // optional, as QFile destructor will already do it:
             file.close();
             bool success = add_in_db(pageid,revid);
             if(success == true)
             {
                 qDebug() <<"entry added to DB successfully ";
             }
             else
             {
                 qDebug() <<" failed to add in DB ";
             }
       }





    /* * ***************************************************** */

/************************** DB CODE was here now it has it's own function add_in_db ********************/

}

void dbmanager::del()
{
    qDebug() <<"DELETION CODE GOES HERE";
}
