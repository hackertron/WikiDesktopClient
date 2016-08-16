/*
 * model.h and model.cpp is totally inspired from 
 * QAbstractitem Model subclass 
 * to get better and more detailed idea 
 * refer : http://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html
 * 
 */ 

#include <QAbstractListModel>
#include <QStringList>


class list
{
public:
    list(const QString &title, const QString &id);
    
    
    QString title() const;
    QString id() const;
    
private:
    QString m_title;
    QString m_id;
    
};

class listmodel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum listroles {
        titlerole = Qt::UserRole + 1,
        idrole
    };
    
    listmodel(QObject *parent = 0);
    
    
    void addpages(const list &list);
    Q_INVOKABLE void deletepages(int row);
    Q_INVOKABLE void deletelist();
    
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<list> m_list;
    
};
