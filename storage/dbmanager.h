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

    Q_INVOKABLE QString add(QString p_url);
    Q_INVOKABLE void del();
    Q_INVOKABLE void update();


    

signals:

private:
    QNetworkReply *m_network_reply;
    QFile *m_file;
    QString filename;



public :
    void doDownload(const QVariant &v);
    QString imageDownloadPath;


private slots:

   void downloadFinished();
   void updateDownloadProgress(qint64, qint64  );

public slots:

private:

};

#endif // DBMANAGER_H
