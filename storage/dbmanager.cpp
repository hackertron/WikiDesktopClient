#include "dbmanager.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QCoreApplication>
#include <QDebug>
#include <QQmlcomponent>
#include <QQmlcontext>
#include <QQmlEngine>



dbmanager::dbmanager(QObject *parent) : QObject(parent)
{

}

void dbmanager::add()
{

    QQmlEngine engine;
QQmlComponent component(&engine, "Mywebview.qml");
QObject *object = component.create();

QVariant returnedValue;
QMetaObject::invokeMethod(object, "myQmlFunction",Q_RETURN_ARG(QVariant, returnedValue));

qDebug() << "QML function returned:" << returnedValue.toString();

delete object;
/*
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
    */
    
}

void dbmanager::del()
{
    qDebug() <<"DELETION CODE GOES HERE";
}

