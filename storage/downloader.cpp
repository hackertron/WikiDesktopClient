#include "downloader.h"

void Downloader::downloadFileFromURL(const QString &url, const QString &filePath) {
    if (!m_isReady)
        return;
    m_isReady = false;

    const QString fileName = filePath + url.right(url.size() - url.lastIndexOf("/")); // your filePath should end with a forward slash "/"
    m_file = new QFile();
    m_file->setFileName(fileName);
    m_file->open(QIODevice::WriteOnly);
    if (!m_file->isOpen()) {
        m_isReady = true;
        return; // TODO: permission check?
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager;

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onDownloadFileComplete(QNetworkReply *)));

    manager->get(request);
}

void Downloader::onDownloadFileComplete(QNetworkReply *reply) {
    if (!m_file->isWritable()) {
        m_isReady = true;
        return; // TODO: error check
    }

    m_file->write(reply->readAll());
    m_file->close(); // TODO: delete the file from the system later on
    m_isReady = true;
}
