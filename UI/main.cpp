#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QtWebView/QtWebView>
#include <QtQml>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QQmlEngine>
#include <dbmanager.h>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("WikiToLearn");
    QGuiApplication::setOrganizationName("en.wikitolearn.org");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QtWebView::initialize();

    qmlRegisterType<dbmanager>("en.wtl.org", 1, 0, "dbmanager");
    dbmanager dbman;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty( "dbman", &dbman );
   // engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



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

    QSettings settings;
    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());


    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
