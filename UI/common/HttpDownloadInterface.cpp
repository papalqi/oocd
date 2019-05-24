#include "HttpDownloadInterface.h"
#include "ThreadManager.h"
HttpDownloadInterface::HttpDownloadInterface(QObject *parent)
    : QObject(parent)
      ,m_nTaskId(-1)
      ,m_bInterrupt(false)
{
    moveToThread(ThreadManager::getAgentThread());
    ThreadManager::getAgentThread()->start();
}

void HttpDownloadInterface::setTaskId(int nTaskId)
{
    m_nTaskId = nTaskId;
}

int HttpDownloadInterface::getTaskId()
{
    return m_nTaskId;
}

void HttpDownloadInterface::interrutptDownload()
{
    m_bInterrupt = true;
}

bool HttpDownloadInterface::isInterruptDownload()
{
    return m_bInterrupt;
}
