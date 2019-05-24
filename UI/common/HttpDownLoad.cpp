#include "HttpDownload.h"
#include "ELog.h"
#include <QFileInfo>
#include <QDir>
#include <QEventLoop>
#include <QSettings>
#include <QDebug>

HttpDownLoad::HttpDownLoad(QNetworkAccessManager *pNetworkAccessManage)
    : m_pReply(NULL)
    , m_pNetworkAccessManage(pNetworkAccessManage)
    , m_pHttpDwnLoadInterface(NULL)
    , m_pFile(NULL)
    , m_nStartDownloadSize(0)
{

}

HttpDownLoad::~HttpDownLoad()
{
    if (m_pReply != NULL)
    {
        m_pReply->deleteLater();
        m_pReply = 0;
    }

    if (m_pFile != NULL){
        m_pFile->flush();
        m_pFile->close();
        delete m_pFile;
        m_pFile = NULL;
    }
}

void HttpDownLoad::downloadFile()
{
    m_pReply = m_pNetworkAccessManage->head(QNetworkRequest(m_url));
    QEventLoop loop;
    QObject::connect(m_pReply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    if (m_pReply->error() != QNetworkReply::NoError)
    {
        if (m_pHttpDwnLoadInterface != NULL)
        {
            m_pHttpDwnLoadInterface->sigDownFinished(false, QStringLiteral("访问网址错误: %1，访问路径：%2").arg(m_pReply->error()).arg(m_url.path())
                                                     , m_pHttpDwnLoadInterface->getTaskId());
        }
        return;
    }
    qint64 curFileSize = m_pReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    qint64 curFileTime = m_pReply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toTime_t();
    m_pReply->deleteLater();
    m_pReply = NULL;
    QFileInfo fileInfo(m_url.path());
    fileName = fileInfo.fileName();
    fileName  = m_sDir + fileName;	//设置目录

    if (QFile::exists(fileName))
    {
        if (m_pHttpDwnLoadInterface != NULL)
        {
            m_pHttpDwnLoadInterface->sigDownFinished(true, fileName, m_pHttpDwnLoadInterface->getTaskId());
        }
        return;
    }

    m_sTmpFile = fileName + ".tmp";
    m_sCfgFile = fileName + ".cfg";

    if (!QFile::exists(m_sCfgFile) || !compareServerFile(curFileSize, curFileTime))
    {
        QSettings settings(m_sCfgFile, QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup("cfg");
        settings.setValue("size", curFileSize);
        settings.setValue("stamp", curFileTime);
        settings.endGroup();
        QFile::remove(m_sTmpFile);
    }

    m_pFile = new QFile(m_sTmpFile);

    if (m_pFile->open(QIODevice::ReadWrite)) {
        int nStartSize = m_pFile->size();
        m_pFile->seek(nStartSize);
        startRequest(nStartSize);
    }
    else // 打开文件失败
    {        
        if (m_pHttpDwnLoadInterface != NULL)
        {
            m_pHttpDwnLoadInterface->sigDownFinished(false, fileName, m_pHttpDwnLoadInterface->getTaskId());
        }
    }
}

void HttpDownLoad::setUrl(QUrl url)
{
    m_url = url;
}

QUrl HttpDownLoad::getUrl()
{
    return m_url;
}

void HttpDownLoad::setDownDir(QString sDir)
{
    QDir dir;
    if (!dir.exists(sDir))
    {
        dir.mkpath(sDir);
    }
    m_sDir = dir.toNativeSeparators(sDir);
//    qDebug()<<QString("localDir: %1").arg(m_sDir);
}

void HttpDownLoad::setHttpDwnLoadInterface(HttpDownloadInterface *pHttpdownloadinterface)
{
    m_pHttpDwnLoadInterface = pHttpdownloadinterface;
}

void HttpDownLoad::slot_downloadReadyRead()
{
    if (m_pHttpDwnLoadInterface != NULL && m_pHttpDwnLoadInterface->isInterruptDownload())
    {
        this->deleteLater();
    }
    else
    {
        if (m_pFile != NULL)
        {
            m_pFile->write(m_pReply->readAll());
        }
    }
}

void HttpDownLoad::slot_downloadProgress(qint64 nProgressValue, qint64 nTotalValue)
{
    if (m_pReply->error())
    {
        ELog::instance()->writeErrorLog(QStringLiteral("slot_downloadProgress下载异常:%1,%2").arg(m_url.toString()).arg(m_pReply->errorString()));
        return;
    }

    if (m_pHttpDwnLoadInterface != NULL)
    {
        int nProgressPercent = (nProgressValue + m_nStartDownloadSize) * 100 / (nTotalValue + m_nStartDownloadSize);
        m_pHttpDwnLoadInterface->sigDownloadProgress(nProgressPercent, m_pHttpDwnLoadInterface->getTaskId());
    }
    //qDebug() << QString("xia: %1, %2").arg(nProgressValue + m_nStartDownloadSize).arg(nTotalValue + m_nStartDownloadSize);
}

void HttpDownLoad::slot_downloadFinished()
{
    if (m_pReply->error())
    {
        ELog::instance()->writeErrorLog(QStringLiteral("slot_downloadFinished下载异常:%1,%2").arg(m_url.toString()).arg(m_pReply->errorString()));
        if (m_pHttpDwnLoadInterface != NULL)
        {
            m_pHttpDwnLoadInterface->sigDownFinished(false, fileName, m_pHttpDwnLoadInterface->getTaskId());
        }
    }
    else
    {
        m_pFile->rename(fileName);
        QFile::remove(m_sCfgFile);
        if (m_pHttpDwnLoadInterface != NULL)
        {
            m_pHttpDwnLoadInterface->sigDownFinished(true, fileName, m_pHttpDwnLoadInterface->getTaskId());
        }
    }

    m_pReply->deleteLater();
    m_pReply = 0;

    if (m_pFile != NULL){
        m_pFile->flush();
        m_pFile->close();
        delete m_pFile;
        m_pFile = NULL;
    }
    this->deleteLater();
}

void HttpDownLoad::slot_downloadError(QNetworkReply::NetworkError error)
{
    Q_UNUSED( error )
}

bool HttpDownLoad::compareServerFile(qint64 fileSize, qint64 fileTime)
{
    QSettings setting(m_sCfgFile, QSettings::IniFormat);
    qint64 size = setting.value("cfg/size").toLongLong();
    qint64 stamp = setting.value("cfg/stamp").toLongLong();

    if ((fileSize == size) && (stamp == fileTime))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void HttpDownLoad::startRequest(qint64 from)
{
    m_nStartDownloadSize = from;

    QNetworkRequest request;
    request.setUrl(m_url);
    if (from != 0)
    {
        request.setRawHeader("Range", tr("bytes=%1-").arg(from).toUtf8());
    }

    m_pReply = m_pNetworkAccessManage->get(request);
    connect(m_pReply, SIGNAL(readyRead()), this, SLOT(slot_downloadReadyRead()));
    connect(m_pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slot_downloadProgress(qint64,qint64)));
    connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_downloadFinished()));
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slot_downloadError(QNetworkReply::NetworkError)));
}


