#include "ThreadManager.h"
#include <QDebug>

QThread* ThreadManager::m_pAgentThread = NULL;
QThread* ThreadManager::m_pDownloadThread = NULL;
QThread* ThreadManager::m_pSpeechEvaluationThread = NULL;
QThread* ThreadManager::m_pMp3RecorderThread = NULL;
QThread* ThreadManager::m_pAfterClassSpeechEvaluationThread = NULL;
QThread* ThreadManager::m_pStatusMonitorThread = NULL;
QThread *ThreadManager::getAgentThread()
{
    if (m_pAgentThread == NULL)
    {
       m_pAgentThread = new QThread;
    }
    return m_pAgentThread;
}

QThread *ThreadManager::getDownloadThread()
{
    if (m_pDownloadThread == NULL)
    {
        m_pDownloadThread = new QThread;
    }
    return m_pDownloadThread;
}

void ThreadManager::Release()
{
    if(ThreadManager::m_pAgentThread)
    {
        ThreadManager::m_pAgentThread->terminate();
        ThreadManager::m_pAgentThread->wait();
        delete ThreadManager::m_pAgentThread;
        ThreadManager::m_pAgentThread = NULL;
    }
    if(ThreadManager::m_pDownloadThread)
    {
        ThreadManager::m_pDownloadThread->terminate();
        ThreadManager::m_pDownloadThread->wait();
        delete ThreadManager::m_pDownloadThread;
        ThreadManager::m_pDownloadThread = NULL;
    }
    if(ThreadManager::m_pSpeechEvaluationThread)
    {
        ThreadManager::m_pSpeechEvaluationThread->terminate();
        ThreadManager::m_pSpeechEvaluationThread->wait();
        delete ThreadManager::m_pSpeechEvaluationThread;
        ThreadManager::m_pSpeechEvaluationThread = NULL;
    }
    if(ThreadManager::m_pMp3RecorderThread)
    {
        ThreadManager::m_pMp3RecorderThread->terminate();
        ThreadManager::m_pMp3RecorderThread->wait();
        delete ThreadManager::m_pMp3RecorderThread;
        ThreadManager::m_pMp3RecorderThread = NULL;
    }
    if(m_pAfterClassSpeechEvaluationThread)
    {
        ThreadManager::m_pAfterClassSpeechEvaluationThread->terminate();
        ThreadManager::m_pAfterClassSpeechEvaluationThread->wait();
        delete ThreadManager::m_pAfterClassSpeechEvaluationThread;
        ThreadManager::m_pAfterClassSpeechEvaluationThread = NULL;
    }

    if(m_pStatusMonitorThread)
    {
        ThreadManager::m_pStatusMonitorThread->terminate();
        ThreadManager::m_pStatusMonitorThread->wait();
        delete ThreadManager::m_pStatusMonitorThread;
        ThreadManager::m_pStatusMonitorThread = NULL;
    }

    if(m_pDownloadThread)
    {
        ThreadManager::m_pDownloadThread->terminate();
        ThreadManager::m_pDownloadThread->wait();
        delete ThreadManager::m_pDownloadThread;
        ThreadManager::m_pDownloadThread = NULL;
    }
}

QThread *ThreadManager::getSpeechEvaluationThread()
{
    if(m_pSpeechEvaluationThread == NULL)
    {
        m_pSpeechEvaluationThread = new QThread;
    }
    return m_pSpeechEvaluationThread;
}

QThread* ThreadManager::getMp3RecorderThread()
{
    if(m_pMp3RecorderThread == NULL)
    {
        m_pMp3RecorderThread = new QThread;
    }

    return m_pMp3RecorderThread;
}

QThread* ThreadManager::getAfterClassSpeechEvaluationThread()
{
    if(!m_pAfterClassSpeechEvaluationThread)
    {
        m_pAfterClassSpeechEvaluationThread = new QThread;
    }

    return m_pAfterClassSpeechEvaluationThread;
}

QThread *ThreadManager::getStatusMonitorThread()
{
    if (m_pStatusMonitorThread == NULL)
    {
       m_pStatusMonitorThread = new QThread;
    }
    return m_pStatusMonitorThread;
}

//void ThreadManager::StatusMonitorThreadRelease()
//{
//    if(m_pStatusMonitorThread)
//    {
//        ThreadManager::m_pStatusMonitorThread->terminate();
//        ThreadManager::m_pStatusMonitorThread->wait();
//        delete ThreadManager::m_pStatusMonitorThread;
//        ThreadManager::m_pStatusMonitorThread = NULL;
//    }
//}
