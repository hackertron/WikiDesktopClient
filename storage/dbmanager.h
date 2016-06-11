#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QDebug>
#include <QPoint>
#include <QNetworkReply>
#include <QFile>

class dbmanager : public QObject
{
    Q_OBJECT
public:
    explicit dbmanager(QObject *parent = 0);

    Q_INVOKABLE void add();
    Q_INVOKABLE void del();
    

signals:

private:
    QNetworkReply *m_network_reply;
    QFile *m_file;
    QString filename;
    QString imageDownloadPath;

public :
    void doDownload(const QVariant &v);

private slots:

   void downloadFinished();
   void updateDownloadProgress(qint64, qint64  );

public slots:

private:

};

#endif // DBMANAGER_H
