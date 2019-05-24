#include "ELog.h"
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QIODevice>
#include <QDirIterator>
#include <QSettings>
#include <QDebug>
#include <QStandardPaths>
#include <QTextCodec>
#include "CommonFun.h"
#include "EIni.h"
#include "FilePathManage.h"
ELog* ELog::m_pInstance = nullptr;

ELog::ELog()
{
    mLogThread = new QThread;
    mLogThread->start();
    moveToThread(mLogThread);
    m_sPath = FilePathManage::instance()->getLogPath();

    // 写够20个文件后删除 大概是使用7天会清除一次日志
    QDir dir(m_sPath);
    QStringList files = dir.entryList(QStringList("*.txt"), QDir::Files, QDir::Name);
    if(files.size() >= 20)
    {
        CommonFun::deleteDir(m_sPath);
    }
    m_sCurDate = QDateTime::currentDateTime().toString("yyyyMMdd");

    m_sVersion = 0.1;
    m_pErrorMutex = new QMutex;
    m_pDebugMutex = new QMutex;
    m_pTestMutex = new QMutex;

    qRegisterMetaType<MessageType>("MessageType");
    connect(this,&ELog::sigWriteMessage,this,&ELog::slotWriteMessage);
}

ELog::~ELog()
{
    mLogThread->terminate();
    mLogThread->wait(2000);
    mLogThread->deleteLater();

    if (m_pErrorMutex != nullptr)
    {
        delete m_pErrorMutex;
        m_pErrorMutex = nullptr;
    }
    if (m_pDebugMutex != nullptr)
    {
        delete m_pDebugMutex;
        m_pDebugMutex = nullptr;
    }
    if (m_pTestMutex != nullptr)
    {
        delete m_pTestMutex;
        m_pTestMutex = nullptr;
    }
}

ELog *ELog::instance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new ELog;
    }

    return m_pInstance;
}

void ELog::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void ELog::writeDebugLog(const QString &sDebugLog)
{
    sigWriteMessage(DEBUG_MESSAGE,sDebugLog);
}

void ELog::writeErrorLog(const QString &sErrorLog)
{
    sigWriteMessage(ERROR_MESSAGE,sErrorLog);
}

void ELog::writeTestLog(const QString &sTestLog)
{
    sigWriteMessage(TEST_MESSAGE,sTestLog);
}

QString ELog::getCurrentErrorLog()
{
    m_sCurDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString fileName = m_sPath + QDir::separator() + m_sCurDate + "_" + QString::number(EIni::getUseId()) +  "_error.txt";

    QString newFileName = m_sPath + QDir::separator() + m_sCurDate + "_" + QString::number(EIni::getUseId()) + "_WinUpload" +   "_error.txt";
    m_pErrorMutex->lock();
    QFile::remove(newFileName);
    bool ret =  QFile::copy(fileName,newFileName);
    m_pErrorMutex->unlock();
    if(ret)
    {
        return newFileName;
    }
    else
    {
        return "";
    }

}

QString ELog::getCurrentDebugLog()
{
    m_sCurDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString fileName = m_sPath + QDir::separator() + m_sCurDate + "_" + QString::number(EIni::getUseId()) +  "_debug.txt";

    QString newFileName = m_sPath + QDir::separator() + m_sCurDate + "_" + QString::number(EIni::getUseId()) + "_WinUpload" +   "_debug.txt";

    m_pDebugMutex->lock();
    QFile::remove(newFileName);
    bool ret =  QFile::copy(fileName,newFileName);
    m_pDebugMutex->unlock();
    if(ret)
    {
        return newFileName;
    }
    else
    {
        return "";
    }
}

void ELog::writeMessage(const QString& sContent, const QString& sFileName)
{
    QString logPath = m_sPath;
    //查看是否包含该目录
    QDir logDir;
    if (!logDir.exists(logPath))
    {
        logDir.mkpath(logPath);
    }
    //拼字符串
    QFile file(logPath + QDir::separator() + sFileName);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString message = QString("%1 %2 %3").arg(current_date_time).arg(m_sVersion).arg(sContent);

    //打开文件写入
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream text_stream(&file);
        text_stream.setCodec(QTextCodec::codecForName("utf8"));
        text_stream << message << "\r\n\r\n";
        file.flush();
    }
    file.close();
}

void ELog::slotWriteMessage(ELog::MessageType type, QString content)
{
    switch (type)
    {
    case DEBUG_MESSAGE:
    {
        m_pDebugMutex->lock();
        qDebug()<<content;
        m_sCurDate = QDateTime::currentDateTime().toString("yyyyMMdd");
        QString fileName = m_sCurDate + "_" + QString::number(EIni::getUseId()) +  "_debug.txt";
        writeMessage(content,fileName);
        m_pDebugMutex->unlock();
        break;
    }

    case ERROR_MESSAGE:
    {
        m_pErrorMutex->lock();
        qDebug()<<"error###"<<content;
        m_sCurDate = QDateTime::currentDateTime().toString("yyyyMMdd");
        QString fileName = m_sCurDate + "_" + QString::number(EIni::getUseId()) +  "_error.txt";
        writeMessage(content, fileName);
        m_pErrorMutex->unlock();
        break;
    }

    case TEST_MESSAGE:
    {
        m_pTestMutex->lock();
        qDebug()<<"test###"<<content;
        m_sCurDate = QDateTime::currentDateTime().toString("yyyyMMdd");
        QString fileName = m_sCurDate + "_" + QString::number(EIni::getUseId()) +  "_test.txt";
        writeMessage(content, fileName);
        m_pTestMutex->unlock();
        break;
    }

    }
}


