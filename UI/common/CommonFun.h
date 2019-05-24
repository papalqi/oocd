#ifndef COMMONFUN_H
#define COMMONFUN_H

#include <QString>
#include <QStringList>
#include <QApplication>
#include <QDesktopWidget>
#include <QPolygonF>
#include <QPainterPath>
class CommonFun
{
public:
    static QString getCurDateTime();
    static QString getCurPrintDateTime();
    static QString getPrintDateTime(QString sDateTime);
    static QString getFormatDate(int nYear, int nMonth, int nDay, QString sFormat = "-"); // 获取格式化日期
    static QString getFormatDate(int ms);
    static void adjustDateTime(QString sDateTime);
    static QString trunkDateTime(QString sDateTime);
    static QString getHttpParamSign(QStringList paramList);
    static QString getRealInStr(QString text);//获取字段中的数字部分
    static QString getUUID();
    static QString getAppName();
    static QString getAppVersion();
    static QString getAppUpdateTime();
    static QString getOsVersion();
//    static bool isWindow64();
    static QString getBranchName();
    static int getPDRuningNum();
    static bool killPD();

    static QString getVeriFyCodeErrorText(int errorCode);//验证码错误文案
    //2位小数点处理
    static float StringToFloat( QString strValue );
    static QString httpNullToString( QString strHttpInfo ); 
    static int getWeekDay(int nYear, int nMonth, int nDay); // 获取指定的某一天是周几(适用于 1900 - 2099年)
    static bool isLeapYear(int nYear);
    static int getMonthDay(int nYear, int nMonth);

    static QString getStringMd5(QString str);
    static bool pingScale(QString ip);

    static QString getFileCreateDate(QString sFilePath);
    static QString getFileModifyDate(QString sFilePath);

    static inline float getHeightFactor()
    {
        //return 1.0;
        return QApplication::desktop()->screen()->height()/1020.0;
    }

    static inline float getWidthFactor()
    {
        //return 1.0;
        return QApplication::desktop()->screen()->width()/1920.0;
    }

    static void lineToPolygon(QPainterPath& painterPath,const QPointF& pStart, const QPointF& pEnd,
                                       const qreal& pStartWidth, const qreal& pEndWidth);

    static QString colorToRGBString(QColor &color);
    static QString loadFontFamilyFromTTF(const QString& fontPath);
    static QString loadClassTimerFontFamilyFromTTF();
    static QString loadPingFangFontFamilyFromTTF();

    static QString getAppTempFilePath();

    static bool deleteDir(const QString& dirPath, int Days = 0);
    static QPixmap  pixmapToRoundcap(const QPixmap & img, int radius);

    //数字转字符串
    static QString getNumString(int num);
    //字符串自动换行
    static void getMultiContent(QPainter *painter, QString text, int width, QStringList &list);
    static QString getFileMd5(QString filePath);
    //获取设备信息
    static QString getDeviceInfo();

//    static bool createQRCode(const QString& text,QPixmap& pixmap);

    static int iGetProcessID(char * pResult);
    static int iGetCmdResult(char * pCmd, int * pResult, int iLen);
    static void killProcessWithoutCurrent(QString processName);
    static bool KillProcessFromName(QString strProcessName);

    static void xorData(QByteArray &data);
private:
    static QString getMacAddress();
    static int getProcessNum(QString sProcessName);
    static bool killProcess(QString sProcessName);
};

#endif // COMMONFUN_H
