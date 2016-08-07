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

    qmlRegisterType<listmodel>("en.wtl.model", 1, 0, "model");
    listmodel mod;
     engine.rootContext()->setContextProperty("mod", &mod);



   // engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    qDebug() << path;

    engine.rootContext()->setContextProperty("path", path);



    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(path);
    if (!dir.exists("WTL_appdata"))
        dir.mkdir("WTL_appdata");

    dir.cd("WTL_appdata");






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

 //   QString neverland  = dir.absoluteFilePath("menu.png");
 //   QFile::copy(":/images/menu.png",neverland);


/* model view shit goes here */




                          if(!db.open())
                          {
                              qDebug() <<"error in opening DB";
                          }
                          else
                          {
                              qDebug() <<"connected to DB" ;

                          }

                          QVector<QString> page_id;
                          QVector<QString> page_title;

                          QSqlQuery quer("SELECT page_ID , page_title FROM Pages");
                          while (quer.next()) {
                              QString i = quer.value(0).toString();
                              page_id.push_back(i);
                              QString p = quer.value(1).toString();
                              page_title.push_back(p);


                          }
                          for (int i = 0; i < page_id.size(); ++i)
                              {
                               qDebug() << page_id.at(i).toLocal8Bit().constData() << endl;
                               qDebug() << page_title.at(i).toLocal8Bit().constData() << endl;
                                mod.addpages(list(page_title.at(i) , page_id.at(i)));




                              }





                      QQmlContext *ctxt = engine.rootContext();
                      ctxt->setContextProperty("myModel", &mod);


/* model view shit ends here */



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
