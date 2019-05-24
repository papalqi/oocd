#ifndef EINI_H
#define EINI_H

#include <QString>
#include <QMutex>
#include <QSettings>

//枚举定义类
//#include "PDEnumData.h"

class EIni
{
public:
    static void init();
    static int getUseId();//登录信息ID
    static void setUseId(int nId);

    static QString getAccessToken();//登录账号(现在是手机号)
    static void setAccessToken(const QString &sToken);

    static QString getUserName();//登录账号(现在是手机号)
    static void setUserName(const QString &name);

    static QString getPassword();
    static void setPassword(QString sPassword);

    static int getCloseAppStatus();
    static void setCloseAppStatus( int nCloseAppStatus );

    static void setLoginTime(QString sLoginTime); // 当前用户登录时间
    static QString getLoginTime(); // 当前用户登录时间

    static bool getSetGuide(); // 是否需要设置向导
    static void setSetGuide(bool nflag);

    static bool getDeviceTestStatus();//返回设备检测是否成功
    static void setDeviceTestStatus(bool bSuccess);

    static QString getCameraName();
    static void setCameraName(QString cameraName);

    static QString getMicrophoneName();
    static void setMicphoneName(QString name);

    static QString getMicrophoneDescriptionName();
    static void setMicphoneDescriptionName(QString name);

    static QString getCameraDescriptionName();
    static void setCameraDescriptionName(QString name);

    static QString getSpeakerName();
    static void setSpeakerName(QString name);

    static QString getNetworkName();
    static void setNetworkName(QString name);

    static bool getCheckEquipmentFlag();// 设置第一次设备检测标识
    static void setCheckEquipmentFlag(bool nflag);

    static bool isTest();
    static void setTest(bool enabled);

    static uint getMainWindowHandle();//mainwindow handle
    static void setMainWindowHandle(uint handle);

    static QString getLocalLiveClassFileDir();
    static void setLocalLiveClassFileDir(const QString& dirPath);

    static QString getRecordClassResolutionType();
    static void setRecordClassResolutionType(QString resolution);

    static bool isRecordClassHorizontalFlip();
    static void setRecordClassHorizontalFlip(bool horizontalFlip);

    static QString getOSSEndPoint();
private:
    static QSettings *sSettings;
    static QMutex sMutx;
};

#endif // EINI_H
