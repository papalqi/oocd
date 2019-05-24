#include "EMainWindow.h"
#include <QString>
#include <ECentralWidget.h>
#include <QThread>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include <EMessageBox.h>
#include <Windows.h>
#include "common/ThreadManager.h"
#include "common/EIni.h"


EMainWindow* EMainWindow::sMainWindow = nullptr;

EMainWindow::EMainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,mpCentralWidget(nullptr)
{
    setObjectName("EMainWindow");
    setWindowTitle(QStringLiteral("OOCD"));
    //setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(Qt::FramelessWindowHint);

    //setWindowFlags();
//    setMinimumSize(1920*0.5,1020*0.5);
//    setBaseSize(1920,1020);
    setStyleSheet("QMainWindow#EMainWindow{background:rgb(217,244,255)}");
    //connect(this,&EMainWindow::mainWindowShowSignal,&PDAnimationWidget::instance(),&PDAnimationWidget::show);
    //connect(this,&EMainWindow::mainWindowHideSignal,&PDAnimationWidget::instance(),&PDAnimationWidget::hide);
    //connect(this,&EMainWindow::mainWindowShowSignal,&PDPageWaitWidget::Instance(),&PDPageWaitWidget::show);
    //connect(this,&EMainWindow::mainWindowHideSignal,&PDPageWaitWidget::Instance(),&PDPageWaitWidget::hide);


    ::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);

     //PDAnimationWidget::instance().setParent(this);
     //PDPageWaitWidget::Instance().setParent(this);
}

EMainWindow &EMainWindow::Instance()
{
    if(!sMainWindow)
    {
        sMainWindow = new  EMainWindow;
    }
    return *sMainWindow;
}

void EMainWindow::Release()
{
    if(sMainWindow)
    {
        delete sMainWindow;
        sMainWindow = nullptr;
    }

}

void EMainWindow::switchToPage(int index)
{
    if(mpCentralWidget)
    {
        mpCentralWidget->switchToPage(index);
    }
}

void EMainWindow::setECentralWidget(ECentralWidget *widget)
{
    mpCentralWidget = widget;
    QMainWindow::setCentralWidget(mpCentralWidget);
}

ECentralWidget *EMainWindow::centralWidget()
{
    return mpCentralWidget;
}

void EMainWindow::wait()
{
    float factor = this->height()/1020.0;
    QRect rect(this->width()/2 - 300*factor/2,this->height()/2 - 300*factor/2,300*factor,300*factor);
    //PDPageWaitWidget::Instance().startPageWaitAnimation(rect);
    qDebug() << "**************************** EMainWindow::wait()";
    //mpCentralWidget->setDisEnabledForWait(true);
}

void EMainWindow::stopWait()
{
    //PDPageWaitWidget::Instance().stopPageWaitAnimation();
    qDebug() << "**************************** EMainWindow::stopWait()";
    //mpCentralWidget->setDisEnabledForWait(false);
}

void EMainWindow::showMyMaximized()
{
    QRect rect = QApplication::desktop()->availableGeometry();
    this->setGeometry(rect);
    this->show();
}

void EMainWindow::enableMaskWidget(bool bEnable)
{
    mpCentralWidget->enableMaskWidget(bEnable);
}

int EMainWindow::titleHeight()
{
    return mpCentralWidget->titleHeight();
}

EMainWindow::~EMainWindow()
{

}

void EMainWindow::showMinimizeSlot()
{
    this->showMinimized();
}

void EMainWindow::quitSlot()
{
    enableMaskWidget(true);
    if(EMessageBox::question2("Are you sure you want to exit?","Yes","No",&EMainWindow::Instance()))
    {
        //PDTeacherLiveController::Instance()->quit(false);
        //PDTeacherLiveController::Instance()->Release();
        qApp->quit();
    }
    enableMaskWidget(false);
}

void EMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    //PDAnimationWidget::instance().setGeometry(0,0,this->width(),this->height());

    emit moved();
}

void EMainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
    emit moveSignal(event);
    emit moved();
}
void EMainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

   /* if(event->type() == QEvent::ActivationChange)
    {

    }*/


}

void EMainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
    if(event->key() == Qt::Key_F1
            && (event->modifiers() & Qt::ControlModifier))
    {
        EIni::setTest(!EIni::isTest());
        if(EIni::isTest())
        {
            EMessageBox::warning("test is opened!","Ok",&EMainWindow::Instance());
        }
        else
        {
            EMessageBox::warning("test is closed!","Ok",&EMainWindow::Instance());
        }
    }
//    else if(event->key() == Qt::Key_F12
//            && (event->modifiers() & Qt::ControlModifier))
//    {
//        PDWebSocktController::Instance()->onSocketError(QAbstractSocket::RemoteHostClosedError);
//    }
//    else if(event->key() == Qt::Key_F11
//            && (event->modifiers() & Qt::ControlModifier))
//    {
//        PDTeacherLiveController::Instance()->leaveChannel();
//        emit PDTeacherLiveController::Instance()->connectionLost();
//    }
//    else if(event->key() == Qt::Key_F10
//            && (event->modifiers() & Qt::ControlModifier))
//    {
//        PDTeacherLiveController::Instance()->leaveChannel();
//        emit PDTeacherLiveController::Instance()->connectionInterrupted();
//    }
//    else if(event->key() == Qt::Key_F9
//            && (event->modifiers() & Qt::ControlModifier))
//    {
//        PDTeacherLiveController::Instance()->leaveChannel();
//        emit PDTeacherLiveController::Instance()->connectionError(1);
//    }
}

void EMainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    mpCentralWidget->update();
    emit mainWindowShowSignal();

}

void EMainWindow::hideEvent(QHideEvent *event)
{
    QMainWindow::hideEvent(event);
    emit mainWindowHideSignal();
}
