#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>
#include <dbmanager.h>
#include <QtWebEngine/QtWebEngine>
#include <QStandardPaths>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtWebEngine::initialize();

    qmlRegisterType<dbmanager>("en.wtl.org", 1, 0, "dbmanager");
        dbmanager dbman;
        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty( "dbman", &dbman );
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



        QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
         qDebug() << path;

         QDir dir(path);
         if (!dir.exists())
             dir.mkpath(path);
         if (!dir.exists("WTL_appdata"))
             dir.mkdir("WTL_appdata");

         dir.cd("WTL_appdata");


    QDir databasePath;



    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
    if(!db.open())
    {
        qDebug() <<"error";
    }
    else
    {
        qDebug() <<"connected" ;
    }
    QSqlQuery query;

   if( query.exec("CREATE TABLE IF NOT EXISTS `Pages`"
    "(  `page_ID` INTEGER NOT NULL PRIMARY KEY ,"
    "`page_revision` INT  NOT NULL);"))
   {
       qDebug() << "pages table created";
   }
   else
   {
    qDebug() <<query.lastError();
   }

  if(query.exec("CREATE TABLE IF NOT EXISTS `Dependencies` ("
     "`depe_ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
    " `depe_fileName` VARCHAR(45) NOT NULL,"
    "`revision_number` INTEGER NOT NULL);"))
  {
      qDebug() << "Dependencies table created";

  }
  else
  {
   qDebug() <<query.lastError();
  }

   query.clear();
   db.close();

   QString styling = "<style type=\"text/css\"> "

             " p{"

                "  display: block;"
             " -webkit-margin-before: 1em; "
             " -webkit-margin-after: 1em; "
             " -webkit-margin-start: 0px; "
             " -webkit-margin-end: 0px; "

             " } "


           " body { "
          "    font-family: 'Source Sans Pro', sans-serif; "
           "   } "

       "   pre { "
        "      display: block; "
         "     padding: 9.5px; "
         "     margin: 0 0 10px; "
          "    font-size: 13px; "
           "   line-height: 1.42857143; "
           "   word-break: break-all; "
           "   word-wrap: break-word; "
            "  color: #333; "
          "    background-color: #f5f5f5; "
           "   border: 1px solid #ccc; "
           "   border-radius: 4px; "
       "   } "



        "  </style> "
           "";





    QString filename = dir.absoluteFilePath("main.css");
    qDebug() << filename;
    QFile file(filename);
          if (file.open(QIODevice::ReadWrite)) {
              QTextStream stream(&file);
              stream << styling << endl;
          }

    app.setApplicationName("WTL_Client");
    return app.exec();
}
