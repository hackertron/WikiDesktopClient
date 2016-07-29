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
    Q_INVOKABLE QString del(QString pageid);
    Q_INVOKABLE void update();




signals:

private:
    QNetworkReply *m_network_reply;
    QFile *m_file;
    QString filename;

    QNetworkReply *png_network_reply;
    QFile *png_file;
    QString png_filename;



public :
   void doDownload(const QVariant &v);
   void png_download(const QStringList &v , const QStringList &n);
    QString imageDownloadPath;


private slots:

   void downloadFinished();
   void updateDownloadProgress(qint64, qint64  );

   void png_finished();
   void update_png_download(qint64, qint64  );


public slots:

private:

};

#endif // DBMANAGER_H
