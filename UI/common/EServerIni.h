#ifndef ESERVERINI_H
#define ESERVERINI_H

#include <QString>

class EServerIni
{
public:
    static QString getServerApiIp();
    static QString getServerWebSocketIp();

    static QString getDefaultServerWebSocketIp();
    static QString getCurrentServerWebSocketIp();
    static void setCurrentServerWebSocketIp(const QString& serverWsIp);

    static QString getServerScheduleIp();
    static QString getServerClassRecordIp();
    static QString getServerPurchaseIp();
    static QString getServerBeginnersGuideIp();

    //服务器模式
    static int getServerModel();
    static void setServerModel(int model);

    static QString serverModelToString();
};

#endif // ESERVERINI_H
