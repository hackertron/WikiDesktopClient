#ifndef MODEL_H
#define MODEL_H

#include <QStringList>
#include <QAbstractListModel>
#include <QList>
// from here 
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
// to here


QString data_path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

class model : public QObject
{
public:
    
    model();
    QStringList page_name , page_id;     
    QString page_name();
    QString page_id();
    
    
    Q_INVOKABLE QString read();
    
    
    
private:
    QString p_name;
    QString p_id;
    
};


class myModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum model_roles {
        title = Qt::UserRole + 1,
        id
    };
    
    
    void addpages(QString page);
    
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    
protected:
    QHash<int, QByteArray> roleNames() const;
    
private:
    QList<model> m_entries;
    
};

#endif // MODEL_H
