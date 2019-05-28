#include "ECentralWidget.h"
#include "ETitleWidget.h"
#include "EMainWindow.h"
#include <QVBoxLayout>
#include <QPainter>
#include <Windows.h>
#include <QStyleOption>

//#include "PDEnumdata.h"
//#include "PDPageWaitWidget.h"
ECentralWidget::ECentralWidget(EMainWindow *parent)
    : EWidget(parent)
    ,bDisEnabledForWait(false)
{
    setObjectName("ECentralWidget");
    setUseDeskTopFactor(false);
    setBaseSize(1920,1020);
    mpTitleWidget = new ETitleWidget(this);
    mpStackedWidget = new QStackedWidget(this);

    mMaskWidget = new QWidget(this);
    mMaskWidget->setWindowFlags(Qt::FramelessWindowHint);
    //mMaskWidget->setStyleSheet("QWidget{background:rgba(0,0,0,0.5);}");
    enableMaskWidget(false);


    connect(mpTitleWidget,&ETitleWidget::minimumSignnal,&EMainWindow::Instance(),&EMainWindow::showMinimizeSlot);
    connect(mpTitleWidget,&ETitleWidget::quitSignal,&EMainWindow::Instance(),&EMainWindow::quitSlot);

}

ECentralWidget::~ECentralWidget()
{

}

void ECentralWidget::setTitlebarBackgroundColor(const QColor &color)
{
    mpTitleWidget->setBackgroundColor(color);
    update();
}

void ECentralWidget::enableMaskWidget(bool bEnable)
{
    mMaskWidget->raise();
    mMaskWidget->setVisible(bEnable);
}

int ECentralWidget::titleHeight()
{
    return mpTitleWidget->height();
}

void ECentralWidget::setDisEnabledForWait(bool enabled)
{
    bDisEnabledForWait = enabled;
}

void ECentralWidget::paintEvent(QPaintEvent *event)
{
    EWidget::paintEvent(event);

}

void ECentralWidget::resizeEvent(QResizeEvent *event)
{
	EWidget::resizeEvent(event);

    mpTitleWidget->setGeometry(0,0,this->width(),20);
    mpStackedWidget->setGeometry(0,mpTitleWidget->height(),this->width(),this->height() - mpTitleWidget->height());
    mMaskWidget->setGeometry(0,mpTitleWidget->height(),this->width(),this->height() - mpTitleWidget->height());
    //mWaitWidget->setGeometry(0,mpTitleWidget->height(),this->width(),this->height());
    //PDPageWaitWidget::Instance().setGeometry(0,mpTitleWidget->height(),this->width(),this->height() - mpTitleWidget->height());
}



int ECentralWidget::addNewPage(QWidget *widget)
{
    int index = mpStackedWidget->addWidget(widget);
    return index;
}

