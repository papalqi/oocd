#include "CommonFun.h"
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>
#include <QNetworkInterface>
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <windows.h>
#include <math.h>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QBitmap>
#include <QPainter>
#include "GetSysInfo.h"
#include "EIni.h"
//#include "qrencode.h"
#include "tlhelp32.h"
#include "ELog.h"


static QString g_sHttpAPP_Key = "test";
static QStringList numStringList = {QStringLiteral("零"),
                                    QStringLiteral("一"),
                                    QStringLiteral("二"),
                                    QStringLiteral("三"),
                                    QStringLiteral("四"),
                                    QStringLiteral("五"),
                                    QStringLiteral("六"),
                                    QStringLiteral("七"),
                                    QStringLiteral("八"),
                                    QStringLiteral("九"),
                                    QStringLiteral("十"),
                                    QStringLiteral("十一"),
                                    QStringLiteral("十二"),
                                    QStringLiteral("十三"),
                                    QStringLiteral("十四"),
                                    QStringLiteral("十五"),
                                    QStringLiteral("十六"),
                                    QStringLiteral("十七"),
                                    QStringLiteral("十八"),
                                    QStringLiteral("十九"),
                                    QStringLiteral("二十"),
                                    QStringLiteral("二十一"),
                                    QStringLiteral("二十二"),
                                    QStringLiteral("二十三"),
                                    QStringLiteral("二十四"),
                                    QStringLiteral("二十五"),
                                    QStringLiteral("二十六"),
                                    QStringLiteral("二十七"),
                                    QStringLiteral("二十八"),
                                    QStringLiteral("二十九"),
                                    QStringLiteral("三十")};

QString CommonFun::getCurDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

QString CommonFun::getCurPrintDateTime()
{
    return QDateTime::currentDateTime().toString("yy-MM-dd HH:mm:ss");
}

QString CommonFun::getPrintDateTime(QString sDateTime)
{
    return QDateTime::fromString(sDateTime, "yyyy-MM-dd HH:mm:ss").toString("yy-MM-dd HH:mm:ss");
}

QString CommonFun::getFormatDate(int nYear, int nMonth, int nDay, QString sFormat)
{
    QString sMonth;
    if (nMonth < 10)
    {
        sMonth =  QString("0%1").arg(nMonth);
    }
    else
    {
        sMonth =  QString("%1").arg(nMonth);
    }

    QString sDay;
    if (nDay < 10)
    {
        sDay = QString("0%1").arg(nDay);
    }
    else
    {
        sDay = QString("%1").arg(nDay);
    }


    return QString("%1%2%3%4%5").arg(nYear).arg(sFormat).arg(sMonth).arg(sFormat).arg(sDay);
}

QString CommonFun::getFormatDate(int ms)
{
    long long ss = 1000;
    long long mi = ss * 60;
    long long hh = mi * 60;
    long long dd = hh * 24;

    long long day = ms / dd;
    long long hour = (ms - day * dd) / hh;
    long long minute = (ms - day * dd - hour * hh) / mi;
    long long second = (ms - day * dd - hour * hh - minute * mi) / ss;
    long long milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

    QString hou = QString::number(hour,10);
    QString min = QString::number(minute,10);
    QString sec = QString::number(second,10);
    QString msec = QString::number(milliSecond,10);

    min = QString::number(minute+hour*60);
    min = min.length() == 1 ? "0"+min:min;

    sec = sec.length() == 1 ? "0"+sec:sec;

    return min + ":" + sec ;
}

void CommonFun::adjustDateTime(QString sDateTime)
{
    if (sDateTime.isEmpty())
    {
        return;
    }
    QDateTime dateTime = QDateTime::fromString(sDateTime, "yyyy-MM-dd HH:mm:ss");
    if (!dateTime.isValid())
    {
        return;
    }
    QDateTime curDateTime = QDateTime::currentDateTime();
    int nDifferTime = dateTime.secsTo(curDateTime);
    if (nDifferTime > 60 || -nDifferTime > 60) // 如果日期不相等，矫正机器时间
    {
        ELog::instance()->writeErrorLog(QStringLiteral("本地时间：%1,服务器时间：%2")
                                         .arg(curDateTime.toString("yyyy-MM-dd HH:mm:ss")).arg(sDateTime));
        SYSTEMTIME   st;
        GetLocalTime(&st);
        st.wYear=dateTime.date().year();
        st.wMonth=dateTime.date().month();
        st.wDay=dateTime.date().day();
        st.wHour = dateTime.time().hour() - 8;
        st.wMinute = dateTime.time().minute();
        st.wSecond = dateTime.time().second();
        SetSystemTime(&st);    //Win32 API 设置系统时间
    }
}

QString CommonFun::trunkDateTime(QString sDateTime)
{
    QDateTime dateTime = QDateTime::fromString(sDateTime, "yyyy-MM-dd HH:mm:ss");
    return dateTime.toString("MM-dd HH:mm");
}

QString CommonFun::getHttpParamSign(QStringList paramList)
{
    paramList.sort();
    int nLen = paramList.length();
    QString sParams;
    int nIndex = 0;
    for (int i = 0; i < nLen; i++)
    {
        nIndex = paramList[i].indexOf("=");
        sParams.append(paramList[i].replace(nIndex, 1, ""));
    }

    QString appSecretParam = getStringMd5(g_sHttpAPP_Key).toUpper();

    appSecretParam = QString("app_secret").append(appSecretParam);

    QString sMd5 = getStringMd5(appSecretParam.append(sParams));
    return sMd5.toUpper();
}


QString CommonFun::getRealInStr(QString text)
{
    QRegExp rx("(\\d+)|(\\d*\\.\\d+)");//整数与浮点数匹配
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        text.remove(pos,rx.matchedLength());
        pos += rx.matchedLength();
    }
    return text;
}

QString CommonFun::getUUID()
{
    return getStringMd5(getMacAddress());
}

QString CommonFun::getAppName()
{
    return  "";
}

QString CommonFun::getAppVersion()
{
    QSettings settings("./readme.ini", QSettings::IniFormat);
    return settings.value("Readme/AppVersion", "255.255.255.255").toString();
}

QString CommonFun::getAppUpdateTime()
{
    QSettings settings("./readme.ini", QSettings::IniFormat);
    return settings.value("Readme/AppUpdateTime","").toString();
}

QString CommonFun::getOsVersion()
{
    return QString::number(QSysInfo::WindowsVersion);
}


QString CommonFun::getBranchName()
{
    return "";
}

int CommonFun::getPDRuningNum()
{
    QString sProcessName;
    return getProcessNum(sProcessName);
}

bool CommonFun::killPD()
{
    QString sProcessName;
    return killProcess(sProcessName);
}

QString CommonFun::getVeriFyCodeErrorText(int errorCode)
{
    QString errorText = QStringLiteral("获取验证码失败!");
    switch (errorCode)
    {
    //    case 401:
    //        errorText = QStringLiteral("验证码错误,请检查");
    //        break;
    //    case 402:
    //        errorText = QStringLiteral("验证码已使用,请重新获取");
    //        break;
    //    case 403:
    //        errorText = QStringLiteral("验证码已过期,请重新获取");
    //        break;
    case 405:
        errorText = QStringLiteral("验证码发送异常,请重试");
        break;
    case 406:
        errorText = QStringLiteral("验证码信息保存异常");
        break;
    case 407:
        errorText = QStringLiteral("请输入正确的手机号");
        break;
    case 408:
        errorText = QStringLiteral("获取验证码过于频繁,请等待1分钟后再试");
        break;
    case 409:
        errorText = QStringLiteral("获取验证码过于频繁,请等待1小时后再试");
        break;
    case 410:
        errorText = QStringLiteral("获取验证码过于频繁,请等待24小时后再试");
        break;
    default:
        break;
    }
    return errorText;
}

//小数位为2位的处理
float CommonFun::StringToFloat( QString strValue )
{
    //    float fValue = strValue.toFloat() + 0.000000001;
    //    return fValue;
    return strValue.toFloat();
}

QString CommonFun::httpNullToString( QString strHttpInfo )
{
    if( strHttpInfo == "null" ) {
        strHttpInfo = "";
    }

    return strHttpInfo;
}

int CommonFun::getWeekDay(int nYear, int nMonth, int nDay)
{
    // 年份
    int nYearFlag = 0;
    int nDisYear = nYear - 2000;
    if (nDisYear >= 0)
    {
        nYearFlag = (nDisYear/4 + nDisYear) % 7;
    }
    else
    {
        nYearFlag = (nDisYear/4 + nDisYear) % 7 + 1;
    }

    // 月份
    int nMothFlag = 0;
    bool bLeapYear = isLeapYear(nYear);
    switch (nMonth) {
    case 5:
        nMothFlag = 0;
        break;
    case 8:
        nMothFlag = 1;
        break;
    case 2:
        nMothFlag = 2 - (bLeapYear?1:0);
        break;
    case 3:
    case 11:
        nMothFlag = 2;
        break;
    case 6:
        nMothFlag = 3;
        break;
    case 9:
    case 12:
        nMothFlag = 4;
        break;
    case 4:
    case 7:
        nMothFlag = 5;
        break;
    case 1:
        nMothFlag = 6 - (bLeapYear?1:0);
        break;
    case 10:
        nMothFlag = 6;
        break;
    }

    return (nYearFlag + nMothFlag + nDay)%7;
}

bool CommonFun::isLeapYear(int nYear)
{
    if(((nYear%4==0) && (nYear%100!=0))
            ||(nYear%400==0))
    {
        return true;
    }
    return false;
}

int CommonFun::getMonthDay(int nYear, int nMonth)
{
    switch (nMonth) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    }

    if (isLeapYear(nYear))
    {
        return 29;
    }
    return 28;
}

QString CommonFun::getStringMd5(QString str)
{
    QByteArray bb = QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Md5);
    return bb.toHex();
}

//获取pos的mac地址
QString CommonFun::getMacAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (int i = 0; i < list.length(); i++)
    {
        if (list[i].humanReadableName().trimmed() == QStringLiteral("本地连接"))
        {
            return list[i].hardwareAddress();
        }
    }

    for (int i = 0; i < list.length(); i++)
    {
        if (!list[i].humanReadableName().isEmpty())
        {
            return list[i].hardwareAddress();
        }
    }

    ELog::instance()->writeErrorLog(QString("empty macAddress"));
    return "";
}

int CommonFun::getProcessNum(QString sProcessName)
{
    QStringList strList;
    strList.append("-NH");
    strList.append("-FI");
    strList.append(QString("imagename eq %1").arg(sProcessName));
    QProcess process;
    process.start("tasklist", strList);
    process.waitForFinished();
    QString msg = process.readAllStandardOutput();
    QList<QString> list = msg.split("\r\n");
    int nNum = 0;
    int nLen = list.length();
    for (int i = 0; i < nLen; i++)
    {
        if (list[i].contains(QString("%1").arg(sProcessName)))
        {
            nNum++;
        }
    }
    return nNum;
}

bool CommonFun::killProcess(QString sProcessName)
{
    QStringList strList;
    strList.append("-f");
    strList.append("-t");
    strList.append("-im");
    strList.append(QString("%1").arg(sProcessName));
    QProcess process;
    process.start("taskkill", strList);
    process.waitForFinished();
    int nExitCode = process.exitCode();
    if (nExitCode == QProcess::NormalExit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//判断秤网络是否连通
bool CommonFun::pingScale(QString ip)
{
    QProcess pingProcess;
    QString exec = "ping";
    QStringList params;
    params << "-n" << "1" << "-w" << "1000" <<ip;
    pingProcess.start(exec, params, QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);
    QByteArray p_stdout = pingProcess.readAllStandardOutput();

    QString qstr=QString::fromLocal8Bit(p_stdout);
    qDebug()<<qstr;
    if(qstr.contains("往返行程的估计时间",Qt::CaseSensitive))
    {
        return true;
    }
    else if(qstr.contains("Approximate round trip times in",Qt::CaseSensitive))
    {
        return true;
    }
    else if(qstr.contains("无法访问目标主机",Qt::CaseSensitive))
    {
        return false;
    }
    else if(qstr.contains("请求超时",Qt::CaseSensitive))
    {
        return false;
    }
    else
    {
        return false;
    }
}

QString CommonFun::getFileCreateDate(QString sFilePath)
{
    QFileInfo fileInfo(sFilePath);
    return fileInfo.created().toString("yyyyMMddhhmmss");
}

QString CommonFun::getFileModifyDate(QString sFilePath)
{
    QFileInfo fileInfo(sFilePath);
    return fileInfo.lastModified().toString("yyyyMMddhhmmss");
}

void CommonFun::lineToPolygon(QPainterPath &painterPath, const QPointF& pStart, const QPointF& pEnd,
                              const qreal& pStartWidth, const qreal& pEndWidth)
{

    qreal x1 = pStart.x();
    qreal y1 = pStart.y();

    qreal x2 = pEnd.x();
    qreal y2 = pEnd.y();

    QLineF line(pStart, pEnd);

    qreal alpha = (90.0 - line.angle()) * 3.1416 / 180.0;
    qreal hypothenuseStart = pStartWidth / 2;

    qreal hypothenuseEnd = pEndWidth / 2;

    qreal sinAlpha = sin(alpha);
    qreal cosAlpha = cos(alpha);

    qreal oppositeStart = sinAlpha * hypothenuseStart;
    qreal adjacentStart = cosAlpha * hypothenuseStart;

    QPointF p1a(x1 - adjacentStart, y1 - oppositeStart);
    QPointF p1b(x1 + adjacentStart, y1 + oppositeStart);

    qreal oppositeEnd = sinAlpha * hypothenuseEnd;
    qreal adjacentEnd = cosAlpha * hypothenuseEnd;

    QPointF p2a(x2 - adjacentEnd, y2 - oppositeEnd);

    painterPath.moveTo(p1a);
    painterPath.lineTo(p2a);

    painterPath.arcTo(x2 - hypothenuseEnd, y2 - hypothenuseEnd, pEndWidth, pEndWidth, (90.0 + line.angle()), -180.0);

    painterPath.lineTo(p1b);

    painterPath.arcTo(x1 - hypothenuseStart, y1 - hypothenuseStart, pStartWidth, pStartWidth, -1 * (90.0 - line.angle()), -180.0);

    painterPath.closeSubpath();
}

QString CommonFun::colorToRGBString(QColor &color)
{
    int alpha = color.alpha();
    double a = alpha/255.0;
    if(0.01 < a && a < 0.1)
    {
        a = 0.1;
    }
    else if(alpha == 255)
    {
        a = 1;
    }
    QString strAlpha =   QString::number(a,'g',1);
    QString rgbStr = QString("rgba(%1,%2,%3,%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(strAlpha);
    return rgbStr;
}

QString CommonFun::loadClassTimerFontFamilyFromTTF()
{
    static QString font;
    static bool loaded = false;
    if(!loaded)
    {
        loaded = true;
        font = CommonFun::loadFontFamilyFromTTF(QCoreApplication::applicationDirPath() + "/font/04B_24.TTF");
    }

    return font;
}

QString CommonFun::loadPingFangFontFamilyFromTTF()
{
    static QString font;
    static bool loaded = false;
    if(!loaded)
    {
        loaded = true;
        font = CommonFun::loadFontFamilyFromTTF(QCoreApplication::applicationDirPath() + "/font/PingFang.TTF");
    }

    return font;
}

QString CommonFun::getAppTempFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

bool CommonFun::deleteDir(const QString &dirPath, int Days)
{
    if (dirPath.isEmpty()){
        return false;
    }
    QDir dir(dirPath);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList){ //遍历文件信息
        qint64 nSecs = 1;
        if (Days != 0)
            nSecs = file.created().addDays(7).secsTo(QDateTime::currentDateTime());
        if (file.isFile() && nSecs > 0){ // 是文件 并且创建时间大于7天，删除
            file.dir().remove(file.fileName());
        }else{ // 递归删除
            deleteDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}

QPixmap CommonFun::pixmapToRoundcap(const QPixmap &img, int radius)
{
    if (img.isNull())
    {
        return QPixmap();
    }
    QSize size(img.size());
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(mask.rect(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addRoundedRect(mask.rect(), radius, radius);
    // painter.drawRoundedRect(mask.rect(), radius, radius);
    painter.fillPath(path,QColor(0, 0, 0));
    painter.drawPath(path);
    QPixmap image = img;
    image.setMask(mask);
    return image;
}

QString CommonFun::getNumString(int num)
{
    if(num > 30 || num <0)
    {
        return "";
    }
    return numStringList[num];
}

QString CommonFun::loadFontFamilyFromTTF(const QString& fontPath)
{
    //qDebug() << fontPath;
    QString font;
    int loadedFontID = QFontDatabase::addApplicationFont(fontPath);
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty())
        font = loadedFontFamilies.at(0);
    //qDebug()<< "new font:" << font;
    return font;
}

void CommonFun::getMultiContent(QPainter *painter, QString text, int width, QStringList &list)
{
    list.clear();
    QString temp = text;
    while(!temp.isEmpty())
    {
        QString content = painter->fontMetrics().elidedText(temp,Qt::ElideRight,width);
        if(content.right(1) == QStringLiteral("…"))
        {
            content = content.left(content.length()-1);
        }
        list.append(content);
        temp = temp.right(temp.length() - content.length());
    }
}

QString  CommonFun::getFileMd5(QString filePath)
{
    //    QFile file(filePath);
    //    file.open(QIODevice::ReadOnly);
    //    QByteArray ba = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
    //    file.close();
    //    return ba.toHex().toUpper().constData();

    QFile sourceFile(filePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return hash.result().toHex().toUpper();
    }
    return QString("");
}

QString CommonFun::getDeviceInfo()
{
    QString deviceInfo;
    return  deviceInfo;
}

//bool CommonFun::createQRCode(const QString &text, QPixmap &pixmap)
//{
//    QRcode *qr = QRcode_encodeString(text.toUtf8(), 3, QR_ECLEVEL_L, QR_MODE_8,1);
//    if(!qr)
//    {
//        return false;
//    }

//    pixmap.fill(Qt::white);
//    QPainter painter(&pixmap);
//    QColor fg(Qt::black);
//    QColor bg(Qt::white);
//    painter.setBrush(bg);
//    painter.setPen(Qt::NoPen);
//    painter.drawRect(0,0,pixmap.width(),pixmap.height());
//    painter.setBrush(fg);
//    const int s=qr->width>0?qr->width:1;
//    const double w= pixmap.width();
//    const double h= pixmap.height();
//    const double aspect=w/h;
//    const double scale=((aspect>1.0)?h:w)/s;
//    for(int y = 0;y < s; y++)
//    {
//        const int yy=y*s;
//        for(int x=0; x < s; x++)
//        {
//            const int xx=yy+x;
//            const unsigned char b=qr->data[xx];
//            if(b &0x01)
//            {
//                const double rx1=x*scale, ry1=y*scale;
//                QRectF r(rx1, ry1, scale, scale);
//                painter.drawRects(&r,1);
//            }
//        }
//    }
//    QRcode_free(qr);
//    return true;
//}

void CommonFun::killProcessWithoutCurrent(QString processName)
{
    int arrResult[100] = {0};
    int iID = GetCurrentProcessId();//当前进程ID,不能删
    QString cmdStr = QString("tasklist | findstr ") + processName;
    char*  cmdCharstr;
    QByteArray byteArray = cmdStr.toLatin1(); // must
    cmdCharstr =byteArray.data();
    int iQuantity = iGetCmdResult(cmdCharstr, arrResult, 1024);
    for(int k = 0; k < iQuantity; k++)
    {
        if(iID != arrResult[k])
        {
            HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,DWORD(arrResult[k]));
            ::TerminateProcess(hProcess,0);
            CloseHandle(hProcess);

            //            char cstrCmd[100] = {0};
            //            sprintf(cstrCmd, "taskkill /F /pid %d /T", arrResult[k]);
            //            WinExec(cstrCmd, SW_HIDE);

            //            QStringList strList;
            //            strList.append("-pid");
            //            strList.append(QString("%1").arg(arrResult[k]));
            //            strList.append("-F");

            //            QProcess process;
            //            process.start("taskkill", strList);
            //            process.waitForFinished();
        }
    }

}


bool CommonFun::KillProcessFromName(QString strProcessName)
{
    //创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)
    HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

    //PROCESSENTRY32进程快照的结构体
    PROCESSENTRY32 pe;

    //实例化后使用Process32First获取第一个快照的进程前必做的初始化操作
    pe.dwSize = sizeof(PROCESSENTRY32);


    //下面的IF效果同:
    //if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄
    if(!Process32First(hSnapShot,&pe))
    {
        return FALSE;
    }

    //将字符串转换为小写
    strProcessName = strProcessName.toLower();

    //如果句柄有效  则一直获取下一个句柄循环下去
    while (Process32Next(hSnapShot,&pe))
    {

        //pe.szExeFile获取当前进程的可执行文件名称
        QString scTmp = QString::fromWCharArray(pe.szExeFile);


        //将可执行文件名称所有英文字母修改为小写
        scTmp = scTmp.toLower();

        //比较当前进程的可执行文件名称和传递进来的文件名称是否相同
        //从快照进程中获取该进程的PID(即任务管理器中的PID)
        DWORD dwProcessID = pe.th32ProcessID;
        if(dwProcessID != GetCurrentProcessId() && scTmp == strProcessName)
        {
            HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);
            ::TerminateProcess(hProcess,0);
            CloseHandle(hProcess);
            CloseHandle(hSnapShot);
            return true;
        }
    }
    CloseHandle(hSnapShot);
    return false;
}

void CommonFun::xorData(QByteArray &data)
{
    static QByteArray key1 = "shilinhahahaha";
    static QByteArray key2 = "pandaabchahaha";
    for(int i = 0; i < data.size(); ++i)
    {
        int keyIndex1 = i % key1.size();
        int keyIndex2 = i % key2.size();
        data[i] = data[i] ^ key1[keyIndex1];
        data[i] = data[i] ^ key2[keyIndex2];
    }
}

int CommonFun::iGetProcessID(char * pResult)
{
    //认为每行是一个记录.记录第一列是程序名,占30个字符,随后是4个字符构成的进程ID
    QString qstrLine = QString(QLatin1String(pResult));
    QString qstrVal = qstrLine.mid(30, 4);
    bool bSuccess;
    return qstrVal.toInt(&bSuccess);
}

int CommonFun::iGetCmdResult(char * pCmd, int * pResult, int iLen)
{
    //进程被视为一个文件,也用open打开,用close关闭
    FILE * fp = _popen(pCmd, "r");
    int k = 0;

    if(fp != NULL)
    {
        char arrTmp[1024] = {0};

        while(fgets(arrTmp, iLen, fp) != NULL)
        {
            pResult[k++] = iGetProcessID(arrTmp);
            memset(arrTmp, 0, sizeof(arrTmp));
        }
        _pclose(fp);
    }

    return k;
}


