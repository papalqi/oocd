#include "EIni.h"
#include <QSettings>
#include <QFile>
#include <QDate>
#include "FilePathManage.h"
#include "EServerIni.h"
#include <QDebug>
#include <QMutexLocker>
#include "ThreadManager.h"
#include "CommonFun.h"

//上传文件
//#include "EHttpUploadFiles.h"

QSettings *EIni::sSettings = nullptr;
QMutex EIni::sMutx;

void EIni::init()
{
    QMutexLocker locker(&sMutx);
    if(!sSettings)
    {
        sSettings = new QSettings(FilePathManage::instance()->getIniFilePath(), QSettings::IniFormat);
        sSettings->setIniCodec("UTF-8");
    }
}

int EIni::getUseId()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("UserInfo/userId", 0).toInt();
}

void EIni::setUseId(int nId)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("UserInfo/userId",nId);
}

QString EIni::getAccessToken()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("UserInfo/AccessToken", QString()).toString();
}

void EIni::setAccessToken(const QString &sToken)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("UserInfo/AccessToken",sToken);
}

QString EIni::getUserName()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("UserInfo/userName", QString()).toString();
}

void EIni::setUserName(const QString &name)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("UserInfo/userName",name);
}

QString EIni::getPassword()
{
    QMutexLocker locker(&sMutx);
    QString xorPWData =  sSettings->value("UserInfo/UserPasswd", "").toString();
    QByteArray pwData =  xorPWData.toUtf8();
    CommonFun::xorData(pwData);
    return QString(pwData);
}

void EIni::setPassword( QString sPassword )
{
    QMutexLocker locker(&sMutx);
    QByteArray pwData =  sPassword.toUtf8();
    CommonFun::xorData(pwData);
    sSettings->setValue("UserInfo/UserPasswd", QString(pwData));
}

int EIni::getCloseAppStatus()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("App/CloseAppStatus", 1).toInt();
}

void EIni::setCloseAppStatus( int nCloseAppStatus )
{
    QMutexLocker locker(&sMutx);;
    sSettings->setValue("App/CloseAppStatus", nCloseAppStatus );
}

void EIni::setLoginTime(QString sLoginTime)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("UserInfo/loginTime", sLoginTime);
}

QString EIni::getLoginTime()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("UserInfo/loginTime", "").toString();
}

bool EIni::getSetGuide()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("loginPD/setGuideFlag", true).toBool();
}

void EIni::setSetGuide(bool nflag)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("loginPD/setGuideFlag", nflag);
}

bool EIni::getDeviceTestStatus()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("DeviceTest/isSuccess", false).toBool();
}

void EIni::setDeviceTestStatus(bool bSuccess)
{
    qDebug() << "DeviceTest Rusult  Pini:" << bSuccess;

    QMutexLocker locker(&sMutx);;
    sSettings->setValue("DeviceTest/isSuccess", bSuccess);
}

QString EIni::getCameraName()
{
    QMutexLocker locker(&sMutx);;
    return sSettings->value("DeviceTest/CameraName","").toString();
}

void EIni::setCameraName(QString cameraName)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("DeviceTest/CameraName",cameraName);
}

QString EIni::getMicrophoneName()
{
   QMutexLocker locker(&sMutx);
    return sSettings->value("DeviceTest/MicrophoneName","").toString();
}

void EIni::setMicphoneName(QString name)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("DeviceTest/MicrophoneName",name);
}

QString EIni::getMicrophoneDescriptionName()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("DeviceTest/MicrophoneDescriptionName","").toString();
}

void EIni::setMicphoneDescriptionName(QString name)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("DeviceTest/MicrophoneDescriptionName",name);
}

QString EIni::getCameraDescriptionName()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("DeviceTest/CameraDescriptionName","").toString();
}

void EIni::setCameraDescriptionName(QString name)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("DeviceTest/CameraDescriptionName",name);
}

QString EIni::getSpeakerName()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("DeviceTest/SpeakerName","").toString();
}

void EIni::setSpeakerName(QString name)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("DeviceTest/SpeakerName",name);
}

QString EIni::getNetworkName()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("DeviceTest/NetworkName","").toString();
}

void EIni::setNetworkName(QString name)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("DeviceTest/NetworkName",name);
}

bool EIni::getCheckEquipmentFlag()
{
    QMutexLocker locker(&sMutx);;
    return sSettings->value("loginPD/CheckEquipmentFlag", true).toBool();
}

void EIni::setCheckEquipmentFlag(bool nflag)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("loginPD/CheckEquipmentFlag", nflag);
}

bool EIni::isTest()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("App/test", false).toBool();
}

void EIni::setTest(bool enabled)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("App/test",enabled);
}

uint EIni::getMainWindowHandle()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("App/mainWindowHandle",-1).toUInt();
}

void EIni::setMainWindowHandle(uint handle)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("App/mainWindowHandle",handle);
}

QString EIni::getLocalLiveClassFileDir()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("App/liveClassFileDir","d:/ACVideo").toString();
}

void EIni::setLocalLiveClassFileDir(const QString &dirPath)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("App/liveClassFileDir",dirPath);
}

QString EIni::getRecordClassResolutionType()
{
    QMutexLocker locker(&sMutx);
    QString resolutionStr = sSettings->value("RecordClass/resolution","720P").toString();
    if(!resolutionStr.compare("720P"))
    {
        return QString("720P");
    }
    else if(!resolutionStr.compare("480P"))
    {
        return QString("480P");
    }
    else
    {
        return QString("240P");
    }
}

void EIni::setRecordClassResolutionType(QString resolution)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("RecordClass/resolution",resolution);
}

bool EIni::isRecordClassHorizontalFlip()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("RecordClass/HorizontalFlip", false).toBool();
}

void EIni::setRecordClassHorizontalFlip(bool horizontalFlip)
{
    QMutexLocker locker(&sMutx);
    sSettings->setValue("RecordClass/HorizontalFlip",horizontalFlip);
}

QString EIni::getOSSEndPoint()
{
    QMutexLocker locker(&sMutx);
    return sSettings->value("App/endPoint","http://oss-accelerate.aliyuncs.com").toString();
}
