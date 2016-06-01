#include "dbmanager.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QString>
#include <QDir>
#include <QQmlEngine>
#include <QQmlComponent>


dbmanager::dbmanager(QObject *parent) : QObject(parent)
{

}

void dbmanager::add()
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

    if( query.exec(""))
    {
        qDebug() << "PAGE ADDED successfully";
    }
    else
    {
     qDebug() <<query.lastError();
    }
    db.close();
}

void dbmanager::del()
{
    qDebug() <<"DELETION CODE GOES HERE";
}


