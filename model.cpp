/*
 * model.h and model.cpp is totally inspired from
 * QAbstractitem Model subclass
 * to get better and more detailed idea
 * refer : http://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html
 *
 */

#include "model.h"
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDir>
#include <QStandardPaths>
#include <QSqlQuery>
#include "dbmanager.h"
#include <QString>

bool check_revision(QString id , QString page_revision)
{

    int revision_number  = page_revision.toInt();

    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mg;
    QObject::connect(&mg, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QString url = "http://en.wikitolearn.org/api.php?action=parse&pageid="+id+"&format=json";
    QNetworkRequest re( ( url ) );
    QNetworkReply *reply = mg.get(re);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString   html = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(html.toUtf8());



        QJsonObject jsonObj = jsonResponse.object();

        int  revid = jsonObj["parse"].toObject()["revid"].toInt();



        if(revision_number == revid)
        {
            delete reply;
            return true ;
        }
        else
        {
            delete reply;
        }
    }
    return false;
}

list::list(const QString &title, const QString &id)
    : m_title(title), m_id(id)
{
}

QString list::title() const
{
    return m_title;
}

QString list::id() const
{
    return m_id;
}

listmodel::listmodel(QObject *parent)
    : QAbstractListModel(parent)
{
}


// add pages to the list , to show user
void listmodel::addpages(const list &list)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_list << list;
    endInsertRows();
}



int listmodel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant listmodel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_list.count())
        return QVariant();

    const list &list = m_list[index.row()];
    if (role == titlerole)
        return list.title();
    else if (role == idrole)
        return list.id();
    return QVariant();
}


QHash<int, QByteArray> listmodel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[titlerole] = "title";
    roles[idrole] = "id";
    return roles;
}

// delete specified page from the list of pages shown to user
void listmodel::deletepages(int row)
{

    qDebug() << row;
    beginRemoveRows(QModelIndex(), row, row);
    m_list.removeAt(row);
    endRemoveRows();
}

// delete the whole list
void listmodel::deletelist()
{
    int total = m_list.count();
    qDebug() << total;
    for(int i = 0 ; i < total ; i++)
    {
        deletepages(i);
    }
}


void listmodel::new_page(QString current_title)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir dir(path);
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

    QSqlQuery query("SELECT page_ID FROM Pages WHERE page_title = '" +current_title+ "'");
    while(query.next())
    {
        QString id  =  query.value(0).toString();
        addpages(list(current_title,id));
    }


}

void listmodel::update(QString pageid , int row)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir dir(path);
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
    QSqlQuery query("SELECT page_title , page_revision FROM Pages WHERE page_ID = '" +pageid+ "'");
    QString current_title , page_revision;
    while(query.next())
    {
        current_title = query.value(0).toString();
        page_revision = query.value(1).toString();

    }
    bool check = check_revision(pageid,page_revision);

    if(check!=true){
        deletepages(row);
        new_page(current_title);
    }
    else{
        qDebug() <<"no update";
    }
}
