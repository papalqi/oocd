#include "HttpDownloadMgr.h"
#include "ThreadManager.h"

HttpDownloadMgr* HttpDownloadMgr::m_pIntance = NULL;

HttpDownloadMgr::HttpDownloadMgr()
    : m_pNetworkAccessManage(NULL)
{
    m_pNetworkAccessManage = new QNetworkAccessManager(this);
    moveToThread(ThreadManager::getAgentThread());
    ThreadManager::getAgentThread()->start();
    qRegisterMetaType<HttpDownloadInterface*>("HttpDownloadInterface*");
    //connect(GlobalSignal::instance(), SIGNAL(sigAddHttpDownTask(QString,QString,HttpDownloadInterface*)),
    //        this, SLOT(slot_addHttpDownTask(QString,QString,HttpDownloadInterface*)));
}

HttpDownloadMgr::~HttpDownloadMgr()
{

}

HttpDownloadMgr *HttpDownloadMgr::instance()
{
    if (m_pIntance == NULL)
    {
        m_pIntance = new HttpDownloadMgr;
    }
    return m_pIntance;
}

void HttpDownloadMgr::addHttpDownTask(QString sUrl, QString sDir, HttpDownloadInterface *pDownloadInterface)
{
    slot_addHttpDownTask(sUrl, sDir, pDownloadInterface);
}

void HttpDownloadMgr::slot_addHttpDownTask(QString sUrl, QString sDir, HttpDownloadInterface *pDownloadInterface)
{
    HttpDownLoad* pHttpDownTask = new HttpDownLoad(m_pNetworkAccessManage);
    pHttpDownTask->setUrl(QUrl::fromEncoded(sUrl.toLocal8Bit()));	//设置url
    pHttpDownTask->setDownDir(sDir);
    pHttpDownTask->setHttpDwnLoadInterface(pDownloadInterface);
    pHttpDownTask->downloadFile();	//开始下载
}


