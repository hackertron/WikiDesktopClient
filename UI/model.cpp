#include "model.h"
#include "dbmanager.h"

model::model()
{
    
}

QString model::page_name()
{
    p_name = page_name.at(title);
    
}

QString model::page_id()
{
    p_id = page_id.at(id);
    
}

void myModel::addpages(QString page)
{
    
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_entries << page;
    endInsertRows();
}

QString model::read()
{
    // this will call addpages to add it in  m_entries 
    QDir dir(data_path);
    dir.cd("WTL_appdata");
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
    if(!db.open())
    {
        qDebug() <<"error in opening DB";
    }
    else
    {
        qDebug() <<"connected to DB" ;

    }
    
    QSqlQuery query;

    query.prepare("SELECT page_ID  FROM Pages");
    
    query.exec();

    if (query.next()) {
        page_name = query.value(0);

    }
    db.close();
    else
    {
        qDebug() << query.lastError();

    }
    
    
    qDebug() << page_name;
    for (int i = 0; i < page_name.size(); ++i)
    {
                    
            QString p = page_name.at(i);
            addpages(p);
            
    }
    
    
    
}



int myModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_entries.count();
}

QVariant myModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_entries.count())
        return QVariant();

    const model &model = m_entries[index.row()];
    if (role == title)
        return model.page_name();
    else if (role == id)
        return model.page_id();
    return QVariant();
}

QHash<int, QByteArray> myModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[title] = "title";
    roles[id]    = "id";
    return roles;
}
