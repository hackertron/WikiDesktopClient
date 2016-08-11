/*
 * model.h and model.cpp is totally inspired from 
 * QAbstractitem Model subclass 
 * to get better and more detailed idea 
 * refer : http://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html
 * 
 */ 

#include "model.h"
#include <QDebug>

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
