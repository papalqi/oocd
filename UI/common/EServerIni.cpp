#include "EServerIni.h"
#include <QSettings>
#include <QDebug>

static QString sPDServerPath = "EServer.ini";
static QStringList serverModelList = {"TestServerInfo","ReleaseServerInfo","GrapServerInfo"};

QString EServerIni::getServerApiIp()
{
    QSettings settings( sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/serverApi", "https://api.aircourses.com").toString();
    return sHttpHostIP;
}

QString EServerIni::getServerWebSocketIp()
{
    if(serverModelToString() != "ReleaseServerInfo")
    {
        return getDefaultServerWebSocketIp();
    }

    QString currentServerWebSocketIp = getCurrentServerWebSocketIp();
    if(currentServerWebSocketIp.isEmpty())
    {
        return getDefaultServerWebSocketIp();
    }
    return currentServerWebSocketIp;
//    QSettings settings(sPDServerPath, QSettings::IniFormat);
//    settings.setIniCodec("UTF-8");
//    QString sHttpHostIP = settings.value(serverModelToString()+"/serverWebSocket", "http://192.168.40.254:8101/websocket/n").toString();
//    return sHttpHostIP;
}

QString EServerIni::getDefaultServerWebSocketIp()
{
    QSettings settings(sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/serverWebSocket","wss://socket.aircourses.com/websocket").toString();
    return sHttpHostIP;
}

QString EServerIni::getCurrentServerWebSocketIp()
{
    QSettings settings(sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/currentServerWebSocket","").toString();
    return sHttpHostIP;
}

void EServerIni::setCurrentServerWebSocketIp(const QString &serverWsIp)
{
    qDebug() << "setCurrentServerWebSocketIp:" << serverWsIp;
    if(serverModelToString() != "ReleaseServerInfo")
    {
        return ;
    }
    QSettings settings(sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue(serverModelToString()+"/currentServerWebSocket",serverWsIp);
}

QString EServerIni::getServerScheduleIp()
{
    QSettings settings( sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/setverClient", "https://client.aircourses.com").toString();
    return sHttpHostIP;
}

QString EServerIni::getServerClassRecordIp()
{
    QSettings settings( sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/setverClient", "https://client.aircourses.com").toString();
    return sHttpHostIP;
}

QString EServerIni::getServerPurchaseIp()
{
    QSettings settings( sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/setverClient", "https://client.aircourses.com").toString();
    return sHttpHostIP;
}

QString EServerIni::getServerBeginnersGuideIp()
{
    QSettings settings( sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString sHttpHostIP = settings.value(serverModelToString()+"/setverClient", "https://client.aircourses.com").toString();
    return sHttpHostIP;
}

int EServerIni::getServerModel()
{
    QSettings settings(sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    return settings.value("App/ServerModel",1).toInt();
}

void EServerIni::setServerModel(int model)
{
    QSettings settings(sPDServerPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("App/ServerModel",model);
}

QString EServerIni::serverModelToString()
{
    int Enum =  getServerModel();
    if(Enum >= 0 && Enum < serverModelList.length())
    {
        return serverModelList[getServerModel()];
    }
    return serverModelList[0];
}


