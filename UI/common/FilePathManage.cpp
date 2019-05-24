#include "FilePathManage.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QApplication>


FilePathManage* FilePathManage::m_pInstance = NULL;

FilePathManage::FilePathManage()
{
    init();
}

FilePathManage *FilePathManage::instance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new FilePathManage;
    }
    return m_pInstance;
}

void FilePathManage::release()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

QString FilePathManage::getPyDateBasePath(){
    QString sDatabaseDirPath = QDir::currentPath() + "/";
    checkAndMkDir(sDatabaseDirPath);
    return sDatabaseDirPath;
}

QString FilePathManage::getLogPath()
{
    QString sLogPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "log" + QDir::separator();
    checkAndMkDir(sLogPath);
    return sLogPath;
}

QString FilePathManage::getLogBackupPath()
{
    QString sLogPath = QDir::currentPath() + "/log/logBackUp/";
    checkAndMkDir(sLogPath);
    return sLogPath;
}

QString FilePathManage::getPDUpdaterExePath()
{
    return QCoreApplication::applicationDirPath()+"/Updater.exe";
}

QString FilePathManage::getHeadImagePath()
{
    static QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator() + "PD_head.png";
    return path;
}

QString FilePathManage::getInstallPackagePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::TempLocation) +"/Update.exe";
}

QString FilePathManage::getCompanionXmlFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/Companion.xml";
}

const QString& FilePathManage::getSkegnFilePath()
{
    QString fileName;
    fileName = "skegn_native.provision";
    static QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator() + fileName;
    return path;
}

const QString& FilePathManage::getIniFilePath()
{
    QString fileName;
    fileName = "PDTeacher.ini";
    static QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator() + fileName;
    return path;
}

const QString &FilePathManage::getWebCacheFilePath()
{
    static QString path =  QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QDir::separator() + "webCache/";
    checkAndMkDir(path);
    return path;
}

const QString &FilePathManage::getClassBriefImageFilePath()
{
    static QString path =  QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "classBrief" +QDir::separator();
    checkAndMkDir(path);
    return path;
}


QString FilePathManage::getQSSPath()
{
    QString qssPath = QCoreApplication::applicationDirPath() + "/qss/pd.qss";
    return qssPath;
}

QString FilePathManage::getPDInstallPath()
{
    QString sDir = m_sDocumentsPath + "/Update/Install/";
    checkAndMkDir(sDir);
    return sDir;
}

QString FilePathManage::getPDInstallUpdateFilesPath()
{
    QString sDir = m_sDocumentsPath + "/Update/Install/files";
    checkAndMkDir(sDir);
    return sDir;
}


QString FilePathManage::getDumpPath()
{
    QString sDumpPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dump/";
    checkAndMkDir(sDumpPath);
    return sDumpPath;
}


void FilePathManage::init()
{
    QString sDefalutPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (sDefalutPath.isEmpty())
    {
        sDefalutPath = "C:/Users/Administrator/Documents";
    }

    m_sDocumentsPath = sDefalutPath + "/PD";
}

void FilePathManage::checkAndMkDir(QString &dirPath)
{
    QDir dir;
    if (dir.exists(dirPath))
    {
        return;
    }
    dir.mkpath(dirPath);
    dirPath = dir.toNativeSeparators(dirPath);
}

QString FilePathManage::getUpdataDumpFilePath()
{
    QString qssPath = QCoreApplication::applicationDirPath() + "/ACUploadDumpFile.exe";
    return qssPath;
}

QString FilePathManage::getRecordFilePath()
{
    QString sDefalutPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    sDefalutPath = sDefalutPath + "/ACRecord/";
    checkAndMkDir(sDefalutPath);
    return sDefalutPath ;
}

QString FilePathManage::getRecordTaskInfoFile()
{
    QString recordPath =   getRecordFilePath();
    return  recordPath + "recordTaskInfo.json";
}

