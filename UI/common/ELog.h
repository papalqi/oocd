#ifndef ELOG_H
#define ELOG_H

#include <QMutex>
#include <QString>
#include <QObject>
#include <QThread>
class ELog : public QObject
{
    Q_OBJECT
public:
    enum MessageType{
      DEBUG_MESSAGE = 0,
      ERROR_MESSAGE,
      TEST_MESSAGE
    };

    ~ELog();
    static ELog* instance();
    static void Release();
    void writeDebugLog(const QString& sDebugLog);
    void writeErrorLog(const QString& sErrorLog);
    void writeTestLog(const QString& sTestLog);
    QString getCurrentErrorLog();
    QString getCurrentDebugLog();

signals:
    void sigWriteMessage(MessageType type,QString content);
private:
    ELog();
    void writeMessage(const QString& sContent, const QString& sFileName);
private slots:
    void slotWriteMessage(MessageType type,QString content);
private:
    static ELog* m_pInstance;
    QMutex* m_pDebugMutex;
    QMutex* m_pErrorMutex;
    QMutex* m_pTestMutex;
    QString m_sCurDate;
    QString m_sVersion;

    QString m_sPath;
    QThread *mLogThread;
};

#endif // ELOG_H
