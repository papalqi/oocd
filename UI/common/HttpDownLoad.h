#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include "HttpDownloadInterface.h"

enum DownloadResult
{
    e_downloadSuccess,
    e_downloadFailed
};

class HttpDownLoad : public QObject
{
    Q_OBJECT

public:
    HttpDownLoad(QNetworkAccessManager* pNetworkAccessManage);
    ~HttpDownLoad();

    void downloadFile();
    void setUrl(QUrl url);
    QUrl getUrl();
    void setDownDir(QString sDir);
    void setHttpDwnLoadInterface(HttpDownloadInterface* pHttpdownloadinterface);

private slots:
    void slot_downloadReadyRead();
    void slot_downloadProgress(qint64 nProgressValue, qint64 nTotalValue);
    void slot_downloadFinished();
    void slot_downloadError(QNetworkReply::NetworkError error);

private:
    bool compareServerFile(qint64 fileSize, qint64 fileTime);
    void startRequest(qint64 from = 0);

private:
    QNetworkReply* m_pReply;
    QNetworkAccessManager* m_pNetworkAccessManage;
    QUrl m_url;
    QString m_sDir;
    QString m_sTmpFile; // 临时文件
    QString m_sCfgFile; // 配置文件
    QString fileName; //
    HttpDownloadInterface* m_pHttpDwnLoadInterface;
    QFile *m_pFile;
    int m_nStartDownloadSize; // 文件的起始下载位置
};

#endif // HTTPDOWNLOAD_H
