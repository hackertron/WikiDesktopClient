#include "model.h"

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

//![0]
QHash<int, QByteArray> listmodel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[titlerole] = "title";
    roles[idrole] = "id";
    return roles;
}
//![0]

