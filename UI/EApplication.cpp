#include "EApplication.h"
#include <QWindow>
#include <QDesktopWidget>
#include <exception>
#include <QTextCodec>
#include <QDebug>
#include <QFile>

//附加功能 版本控制 配置文件
//#include "FilePathManage.h"
//#include "PDNetworkAccessManager.h"
//#include "SingletonManager.h"
//#include "PDLiveContainWidget.h"
//#include "PDAnimationWidget.h"
//#include "PDVersionManageController.h"
//#include "GetSysInfo.h"

#include "common\EIni.h"
#include "common\CommonFun.h"
#include "EMainWindow.h"
#include "EEngineCentrolWidget.h"


ECloseShortCutEventFilter::ECloseShortCutEventFilter()
    :QAbstractNativeEventFilter()
{

}

bool ECloseShortCutEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    MSG* pMsg = reinterpret_cast<MSG*>(message);
//    if(pMsg && pMsg->message == WM_HOTKEY)
//    {
//        int msgS1 = pMsg->lParam;
//        int msgS2 = pMsg->wParam;
//        UINT fuModifiers = (UINT) LOWORD(msgS1);	// key-modifier flags
//        UINT uVirtKey = (UINT) HIWORD(msgS1);       // virtual-key code

//        if(MOD_ALT == msgS1 && msgS2 == VK_F4)
//        {
//            EMainWindow::Instance().quitSlot();
//            return true;
//        }
//    }

    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
            MSG* msg = static_cast<MSG *>(message);
            switch (msg->message)
            {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                {
                    if ((VK_F4 == msg->wParam) && (::GetKeyState(VK_MENU) & 0xF000))
                    {
                        qApp->exit();
                        return true;
                    }
                }
                break;
            default:
                break;
            }
        }

    return false;
}




EEngineCentrolWidget *EApplication::centralWidget = nullptr;
EApplication::EApplication(int argc, char *argv[])
    :QtSingleApplication(argc,argv)
{
    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO) // legacy in 5.6, but still functional
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS"))
    {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
}

EApplication::~EApplication()
{

}

bool EApplication::notify(QObject *object, QEvent *event)
{

    if (QEvent::Show == event->type())
    {
        if (object->isWidgetType())
        {
            QWidget* pWidget = qobject_cast<QWidget*>(object);
            if ((NULL != pWidget))
            {
                pWidget->setAttribute(Qt::WA_Mapped);
            }
        }
    }

    /*if (QEvent::WindowStateChange == event->type())
    {
        if (object->isWidgetType())
        {
            QWidget* pWidget = qobject_cast<QWidget*>(object);
            if ((NULL != pWidget))
            {
                pWidget->update();
                event->ignore();
                //pWidget->repaint();
                //pWidget->update();
            }
        }
    }*/

    bool ret = false;
    try
    {
        ret = QtSingleApplication::notify(object,event);
    }
    catch(std::exception& e)
    {
        qCritical() << "application notify catch exception:" << e.what();
    }
    return ret;
}

int EApplication::exec()
{
    this->installNativeEventFilter(new ECloseShortCutEventFilter);
    CommonFun::KillProcessFromName(this->applicationName() + ".exe");

    //setApplicationVersion(PDVERSION);

    setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    //加载闹钟字体
    CommonFun::loadClassTimerFontFamilyFromTTF();

    //EIni::init();

    //QFile qssFile(FilePathManage::instance()->getQSSPath());
    //if(qssFile.open(QFile::ReadOnly))
    //{
    //    QString qss = QLatin1String(qssFile.readAll());
    //    setStyleSheet(qss);
    //    qssFile.close();
    //}
    //else
    //{
    //    qWarning() << QStringLiteral("加载样式表失败！");
    //}

    //SingletonManager manager;
	
	centralWidget = new EEngineCentrolWidget(&EMainWindow::Instance());
    EMainWindow::Instance().setECentralWidget(centralWidget);
    EMainWindow::Instance().hide();

    //EIni::setMainWindowHandle(::GetCurrentProcessId());
	EMainWindow::Instance().showMyMaximized();

	//版本检测
    //connect(&EVersionManageController::instance(),&EVersionManageController::checkVersionFinished,[this](){
    //    EMainWindow::Instance().showMyMaximized();
    //});

    //EVersionManageController::instance().httpCheckVersion();

    return QtSingleApplication::exec();
}

