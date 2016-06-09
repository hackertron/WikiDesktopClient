#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

class Downloader : public QObject {
    Q_OBJECT
    QFile *m_file;
    bool m_isReady = true;

public:
    explicit Downloader(QObject *parent = 0) : QObject(parent) {}
    virtual ~Downloader() {}

    void downloadFileFromURL(const QString &url, const QString &filePath);

private slots:
    void onDownloadFileComplete(QNetworkReply *reply);
};

#endif // DOWNLOADER_H
