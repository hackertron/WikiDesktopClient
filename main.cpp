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
#include <model.h>



void css_download(QString url , QString filename, QString path)
{
    QString requested_url = url;

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( requested_url );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {

        QDir dir(path);
        if (!dir.exists())
            dir.mkpath(path);
        if (!dir.exists("WTL_appdata"))
            dir.mkdir("WTL_appdata");

        dir.cd("WTL_appdata");

        QString   css = (QString)reply->readAll();
        filename = dir.absoluteFilePath(filename);

        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << css << endl;
        }

    }
}

void js_download(QString url , QString filename, QString path)
{
    QString requested_url = url;

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( requested_url );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {

        QDir dir(path);
        if (!dir.exists())
            dir.mkpath(path);
        if (!dir.exists("WTL_appdata"))
            dir.mkdir("WTL_appdata");

        dir.cd("WTL_appdata");

        QString   js = (QString)reply->readAll();
        filename = dir.absoluteFilePath(filename);

        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << js << endl;
        }

    }
}





int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("WikiToLearn");
    QGuiApplication::setOrganizationName("en.wikitolearn.org");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QtWebView::initialize();

    qmlRegisterType<dbmanager>("en.wtl.org", 1, 0, "dbmanager"); // registers the C++ type in the QML system

    dbmanager dbman;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty( "dbman", &dbman );

    qmlRegisterType<listmodel>("en.wtl.model", 1, 0, "model");
    listmodel mod;
    engine.rootContext()->setContextProperty("mod", &mod);



    //standard path to store all the application data ( varies from OS to OS )
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    qDebug() << path;

    engine.rootContext()->setContextProperty("path", path);



    // create wtl application data in application data location ( generic )
    // cd to the newly created application data dir
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(path);
    if (!dir.exists("WTL_appdata"))
        dir.mkdir("WTL_appdata");

    dir.cd("WTL_appdata");




    /*
     * creates sqlite DB to keep track of all the stored offline pages
     * and also it's dependencies
     *
    */
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
                   "`page_revision` INT  NOT NULL ,"
                   "`page_title` TEXT NOT NULL);"))
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




    /*
     * Populate the model to show all the available offline pages ( IF ANY )
     * this model list will be showed to user so that he/she can
     * either read the offline page or delete or update them
     *
    */



    if(!db.open())
    {
        qDebug() <<"error in opening DB";
    }
    else
    {
        qDebug() <<"connected to DB" ;

    }


    QSqlQuery quer("SELECT page_ID , page_title FROM Pages");
    while (quer.next()) {
        QString i = quer.value(0).toString();
        QString p = quer.value(1).toString();
        mod.addpages(list(p,i));


    }


    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("myModel", &mod);




    /* *** css download **/

    if(QFile::exists(dir.absoluteFilePath("main.css"))) // checks if the css already exist
    {
        qDebug() << " already downloaded main.css";
    }

    else{
        QString url = "https://en.wikitolearn.org/load.php?debug=false&lang=en&modules=ext.echo.badgeicons%7Cext.echo.styles.badge%7Cext.math.styles%7Cext.visualEditor.desktopArticleTarget.noscript%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmediawiki.sectionAnchor%7Cskin.wikitolearn&only=styles&skin=wikitolearnskin";
        css_download(url,"main.css",path);

    }

    if(QFile::exists(dir.absoluteFilePath("bootstrap.css")))
    {
        qDebug() << "already downloaded bootstrap.css";
    }
    else{
        QString  url = "https://en.wikitolearn.org/skins/WikiToLearnSkin/bower_components/font-awesome/css/font-awesome.min.css?314";
        css_download(url,"bootstrap.css",path);

    }

    if(QFile::exists(dir.absoluteFilePath("wikitolearnskin.css")))
    {
        qDebug() << "already downloaded wikitolearnskin.css";
    }
    else{
        QString url = "http://en.wikitolearn.org/load.php?debug=false&lang=en&modules=ext.visualEditor.desktopArticleTarget.noscript%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmediawiki.sectionAnchor%7Cskin.wikitolearn&only=styles&skin=wikitolearnskin";
        css_download(url,"wikitolearnskin.css",path);
    }



    /* css download end  */

    /* js download */
    if(QFile::exists(dir.absoluteFilePath("wikitolearnskin.js"))) // checks if the js file already exist
    {
        qDebug() << " already downloaded wikitolearnskin.js";
    }

    else{
        QString url = "https://en.wikitolearn.org/load.php?debug=false&lang=en&modules=startup&only=scripts&skin=wikitolearnskin";
        js_download(url,"wikitolearnskin.js",path);

    }



    /* js download end */

    QSettings settings;
    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());


    //load main.qml and stores the settings
    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
