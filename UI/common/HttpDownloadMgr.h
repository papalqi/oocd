#ifndef HTTPDOWNLOADMGR_H
#define HTTPDOWNLOADMGR_H

#include <QObject>
#include <QNetworkAccessManager>
#include "HttpDownload.h"
#include "HttpDownloadInterface.h"

class HttpDownloadMgr : public QObject
{
    Q_OBJECT

public:
    ~HttpDownloadMgr();
    static HttpDownloadMgr* instance();
    void addHttpDownTask(QString sUrl, QString sDir, HttpDownloadInterface* pDownloadInterface = NULL);

private slots:
    void slot_addHttpDownTask(QString sUrl, QString sDir, HttpDownloadInterface* pDownloadInterface = NULL);

private:
    HttpDownloadMgr();

private:
    QNetworkAccessManager* m_pNetworkAccessManage;
    static HttpDownloadMgr* m_pIntance;
};

#endif // HTTPDOWNLOADMGR_H
