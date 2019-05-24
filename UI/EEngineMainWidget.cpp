#include "EEngineMainWidget.h"
#include "EEnumData.h"

//窗体类
#include "RenderWindows.h"

#include <qDebug>
#include <qpushbutton.h>

EEngineMainWidget::EEngineMainWidget(EWidget *parent)
	: EWidget(parent)
{
	testBTN = new QPushButton(this);
	testBTN->setText("aaaaaaaaaaaaaaaaaaaaaaaaa此处放菜单栏  待写");

	setBaseSize(1920, 1020);
	setUseDeskTopFactor(false);
	setObjectName("engine");

	addengineWidget();

	mStackedWidget = new QStackedWidget(this);
	mStackedWidget->setStyleSheet("background:rgba(255,255,255,1);");
	mBackgroundMap.load(":/images/student/background.png");
}

EEngineMainWidget::~EEngineMainWidget()
{
}

void EEngineMainWidget::switchToPage(int index)
{
	switch (index)
	{
	//case enginPage:
	//	mStackedWidget->setCurrentWidget();
	//	loadSchduleWebView();
	//	break;
	default:
		break;
	}
}


void EEngineMainWidget::paintEvent(QPaintEvent *event)
{
	EWidget::paintEvent(event);
}

void EEngineMainWidget::resizeEvent(QResizeEvent *event)
{
	EWidget::resizeEvent(event);
	testBTN->setGeometry(0, 0, this->width(), 30);
	mengineWidget->setGeometry(0, 30, this->width(), this->height()/2);
	mStackedWidget->setGeometry(0, 30 + this->height() / 2, this->width(), this->height()/2-30);
}

void EEngineMainWidget::showEvent(QShowEvent *event)
{
	//this->setAttribute(Qt::WA_Mapped);
	EWidget::showEvent(event);
	// mStackedWidget->update();

}

int EEngineMainWidget::addengineWidget()
{
	mengineWidget = new RenderWindows(this);
	return 0;
}
