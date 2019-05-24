#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    static QThread* getAgentThread();
    static QThread* getDownloadThread();
    static void Release();
    static QThread *getSpeechEvaluationThread();
    static QThread *getMp3RecorderThread();
    static QThread *getAfterClassSpeechEvaluationThread();
    static QThread* getStatusMonitorThread();
//    static void StatusMonitorThreadRelease();

private:
    static QThread* m_pAgentThread;
    static QThread* m_pDownloadThread;
    static QThread* m_pSpeechEvaluationThread;
    static QThread* m_pMp3RecorderThread;
    static QThread* m_pAfterClassSpeechEvaluationThread;
    static QThread* m_pStatusMonitorThread;
};

#endif // THREADMANAGER_H
