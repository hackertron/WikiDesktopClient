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

    // to download the page locally and add it's entry to database
    Q_INVOKABLE QString add(QString p_url);

    // to delete the page locally and remove it's entries from database
    Q_INVOKABLE QString del(QString pageid);

    // to delete all the pages and remove the entries from database
    Q_INVOKABLE void deleteAll();

    // to update the page specified by user
    Q_INVOKABLE void update();

    // forcefully update all pages
    Q_INVOKABLE void update_page(QString pageid);

    Q_INVOKABLE void clear_list();

    Q_INVOKABLE void quit();


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
    QString imageDownloadPath; // path to save the images


private slots:

   void downloadFinished();
   void updateDownloadProgress(qint64, qint64  );

   void png_finished();
   void update_png_download(qint64, qint64  );


public slots:

private:

};

#endif // DBMANAGER_H
