#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //
    QDir databasePath;
    //

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
    query.exec("CREATE TABLE IF NOT EXISTS `Pages`"
    "(  `page_ID` INT NOT NULL AUTO_INCREMENT,"
    "`page_revision` VARCHAR(45) NOT NULL,"
    "  PRIMARY KEY (`page_ID`))");
    query.exec("CREATE TABLE IF NOT EXISTS `Dependencies` ("
  "`depe_ID` INT NOT NULL AUTO_INCREMENT,"
 " `depe_fileName` VARCHAR(45) NOT NULL,"
  "`depe_lastUpdated` DATETIME NOT NULL,"
  "PRIMARY KEY (`depe_ID`))");

    query.exec("CREATE TABLE `Pages_has_Dependencies`"
               "(`page_ID`	INTEGER NOT NULL,"
              " `depe_ID`	INTEGER NOT NULL,"
               "PRIMARY KEY(page_ID,depe_ID))");


    return app.exec();
}
