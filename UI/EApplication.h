#ifndef EAPPLICATION_H
#define EAPPLICATION_H
#include <QAbstractNativeEventFilter>
#include <Windows.h>
#include <QDebug>
#include "common/qtsingleapplication.h"



class EEngineCentrolWidget;

class ECloseShortCutEventFilter:public QAbstractNativeEventFilter
{
public:
    ECloseShortCutEventFilter();
protected:
     bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

};

class EApplication : public QtSingleApplication
{
public:
    explicit EApplication(int argc,char *argv[]);
    ~EApplication();
    bool notify(QObject *object, QEvent *event);
    int exec();

    static EEngineCentrolWidget *centralWidget;
};

#endif // EAPPLICATION_H
