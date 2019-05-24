#ifndef FILEPATHMANAGE_H
#define FILEPATHMANAGE_H

/// \brief 文件路径管理类 用于维护程序使用的所有文件路径
#include <QString>

class FilePathManage
{
public:
    static FilePathManage* instance();
    void release();
    QString getQSSPath();
    QString getPDInstallPath(); // PD更新目录
    QString getPDInstallUpdateFilesPath(); // PD更新目录
    QString getDumpPath();
    QString getPyDateBasePath();
    QString getLogPath(); // 日志路径
    QString getLogBackupPath(); // 日志的备份路径
    QString getPDUpdaterExePath();
    QString getHeadImagePath();

    QString getInstallPackagePath();
    QString getCompanionXmlFilePath();
    const QString &getIniFilePath();
    const QString &getWebCacheFilePath();
    const QString &getClassBriefImageFilePath();
    const QString &getSkegnFilePath();

    QString getUpdataDumpFilePath();
    QString getRecordFilePath();
    QString getRecordTaskInfoFile();
private:
    FilePathManage();
    void init();
    void checkAndMkDir(QString& dirPath);

private:
    QString m_sDocumentsPath; // 应用程序路径
    static FilePathManage* m_pInstance;
};

#endif // FILEPATHMANAGE_H
