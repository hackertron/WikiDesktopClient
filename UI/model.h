#include <QAbstractListModel>
#include <QStringList>

//![0]
class list
{
public:
    list(const QString &title, const QString &id);
//![0]
//type title size id animal list
    QString title() const;
    QString id() const;

private:
    QString m_title;
    QString m_id;
//![1]
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

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<list> m_list;

};
