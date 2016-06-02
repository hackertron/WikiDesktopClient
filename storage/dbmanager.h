#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QDebug>
#include <QPoint>

class dbmanager : public QObject
{
    Q_OBJECT
public:
    explicit dbmanager(QObject *parent = 0);

    Q_INVOKABLE void add();
    Q_INVOKABLE void del();
    

signals:

public slots:
};

#endif // DBMANAGER_H
