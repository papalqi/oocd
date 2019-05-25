#ifndef GLOBALSIGNAL_H
#define GLOBALSIGNAL_H

/// \brief 全局信号类

#include <QtCore/QObject>
#include <QObject>
#include "EEnumData.h"

class GlobalSignal: public QObject
{
    Q_OBJECT
public:
    static GlobalSignal* instance();
signals:
    void userActionButtonClick(EMainPageEnum eEnum); //去触发
    void userActionButtonClicked(EMainPageEnum eEnum);//触发之后

    void startDeviceTest();//
private:
    GlobalSignal();
private:
    static GlobalSignal* m_pInstance;
};

#endif // GLOBALSIGNAL_H
