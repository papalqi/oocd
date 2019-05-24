#ifndef HTTPDOWNLOADINTERFACE_H
#define HTTPDOWNLOADINTERFACE_H

#include <QObject>

class HttpDownloadInterface : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownloadInterface(QObject *parent = 0);
    void setTaskId(int nTaskId);
    int getTaskId();
    void interrutptDownload();
    bool isInterruptDownload();

signals :
    void sigDownloadProgress(int nProgressPercent, int nTaskId); // 进度百分比的分子值
    void sigDownFinished(bool bSuccess, QString sFilePath, int nTaskId);

private:
    int m_nTaskId;
    bool m_bInterrupt;
};

#endif // HTTPDOWNLOADINTERFACE_H
