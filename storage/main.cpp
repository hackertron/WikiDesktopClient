#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>
#include <dbmanager.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QScopedPointer <dbmanager> dbm(new dbmanager);
    engine.rootContext()->setContextProperty("dbm",dbm.data());


    QDir databasePath;


    QString path = databasePath.currentPath()+"WTL.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
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
    "(  `page_ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
    "`page_revision` VARCHAR(45) NOT NULL);"))
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
    "`depe_lastUpdated` DATETIME NOT NULL);"))
  {
      qDebug() << "Dependencies table created";
  }
  else
  {
   qDebug() <<query.lastError();
  }

   if( query.exec("CREATE TABLE IF NOT EXISTS `Pages_has_Dependencies`"
               "(`page_ID`	INTEGER NOT NULL,"
              " `depe_ID`	INTEGER NOT NULL,"
               "PRIMARY KEY(page_ID,depe_ID),"
               "INDEX `fk_Pages_has_Dependencies_Dependencies1_idx` (`depe_ID` ASC),"
               "INDEX `fk_Pages_has_Dependencies_Pages_idx` (`page_ID` ASC),"
               "CONSTRAINT `fk_Pages_has_Dependencies_Pages`"
               "FOREIGN KEY (`page_ID`)"
               "REFERENCES `Pages` (`page_ID`)"
               "ON DELETE NO ACTION"
               "ON UPDATE NO ACTION,"
               "CONSTRAINT `fk_Pages_has_Dependencies_Dependencies1`"
               "FOREIGN KEY (`depe_ID`)"
               "REFERENCES `Dependencies` (`depe_ID`)"
               "ON DELETE NO ACTION"
               "ON UPDATE NO ACTION);"))
   {
       qDebug() << "Pages_has_Dependencies table created";
   }
   else
   {
    qDebug() <<query.lastError();
   }

   db.close();
    return app.exec();
}
