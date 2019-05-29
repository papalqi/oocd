#include "EEngineMainWidget.h"
#include "EEnumData.h"

//������
#include "RenderWindows.h"
#include "EMenuWidget.h"
#include "ETreeWidget.h"

#include <qDebug>
#include <qpushbutton.h>
#include <QTextEdit>

EEngineMainWidget::EEngineMainWidget(QMainWindow *parent)
	: QMainWindow(parent)
{
	//testBTN = new QPushButton(this);
	//testBTN->setText("aaaaaaaaaaaaaaaaaaaaaaaaa�˴��Ų˵���  ��д");

	setWindowFlags(Qt::WindowTitleHint);


	setBaseSize(1920, 1020);
	//setUseDeskTopFactor(false);
	setObjectName("engine");

	addengineWidget();

	init();
	initconnect();

	//setStyleSheet("	QMainWindow::separator{background:rgb(0, 222, 255);width:2px;height:1px;}");

}

EEngineMainWidget::~EEngineMainWidget()
{
}

void EEngineMainWidget::init()
{

	//mDockWidget = new QDockWidget(this);
	//mDockWidget->setStyleSheet("background:rgba(0,255,255,1);");

	mStackedWidget = new QStackedWidget();
	//mStackedWidget->setStyleSheet("background:rgba(255,255,255,0.1);");
	//mBackgroundMap.load(":/images/student/background.png");


	mMenuWidget = new EMenuWidget();
	//mMenuWidget->setStyleSheet("background:rgba(255,0,255,1);");

	setDockNestingEnabled(true);
	//����ͣ����
	QDockWidget *centerWidget = new QDockWidget();
	setCentralWidget(centerWidget);	//���Ĵ��ڲ�����QMainWindow::centerwidget
	centerWidget->setWidget(mengineWidget);
	centerWidget->setWindowFlags(Qt::FramelessWindowHint);

	QWidget *center_titleWidget = centerWidget->titleBarWidget();
	QWidget *center_tempWidget = new QWidget();
	centerWidget->setTitleBarWidget(center_tempWidget);
	delete center_titleWidget;

	//��ͣ����
	QDockWidget *mTopDock = new QDockWidget(QStringLiteral("�˵�"), this);
	mTopDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	mTopDock->setWidget(mMenuWidget);
	addDockWidget(Qt::TopDockWidgetArea, mTopDock);

	QWidget *top_titleWidget = mTopDock->titleBarWidget();
	QWidget *top_tempWidget = new QWidget();
	mTopDock->setTitleBarWidget(top_tempWidget);
	delete top_titleWidget;

	//��ͣ����
	QDockWidget *mLeftDock = new QDockWidget(QStringLiteral("�ṹ"), this);
	mLeftDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	mleftTreeWidget = new ETreeWidget(this);
	mLeftDock->setWidget(mleftTreeWidget);    //�����ؼ���ӵ���ͣ������
	addDockWidget(Qt::LeftDockWidgetArea, mLeftDock);

	//QWidget *left_titleWidget = mLeftDock->titleBarWidget();
	//left_titleWidget->setBaseSize(left_titleWidget->width(), 10);


	//��ͣ����

	QDockWidget *mRightDock = new QDockWidget(QStringLiteral("���ô���"), this);

	mRightDock->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QTextEdit *dock2Text = new QTextEdit(QStringLiteral("�Ҳ�"));

	mRightDock->setWidget(dock2Text);

	addDockWidget(Qt::RightDockWidgetArea, mRightDock);

	//�ײ�ͣ����

	QDockWidget *mBottomDock = new QDockWidget(QStringLiteral("�ײ�"), this);
	mBottomDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	QTextEdit *dock3Text = new QTextEdit(QStringLiteral("�ײ�"));
	mBottomDock->setWidget(dock3Text);
	addDockWidget(Qt::BottomDockWidgetArea, mBottomDock);


	//splitDockWidget(dw1, dw3, Qt::Vertical);
	//tabifyDockWidget();

	//dw1->setWindowFlags(Qt::SplashScreen);
	mdocks << centerWidget << mLeftDock << mRightDock << mBottomDock;

	
}
void EEngineMainWidget::initconnect()
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

void EEngineMainWidget::removeAllDock()
{
	for (int i = 0; i<3; ++i)
	{
		//removeDockWidget(mdocks[i]);

	}
}

void EEngineMainWidget::showDock(const QList<int>& index)
{
	if (index.isEmpty())
	{
		for (int i = 0; i<3; ++i)
		{
			mdocks[i]->show();
		}
	}
	else
	{
		foreach(int i, index) {
			mdocks[i]->show();
		}
	}
}


void EEngineMainWidget::paintEvent(QPaintEvent *event)
{
	//EWidget::paintEvent(event);
}

void EEngineMainWidget::resizeEvent(QResizeEvent *event)
{
	//EWidget::resizeEvent(event);
	//testBTN->setGeometry(0, 0, this->width(), 30);
	//mMenuWidget->setGeometry(0, 0, this->width(), 50);
	//mengineWidget->setGeometry(0, 50, this->width(), this->height()/2);
	//mDockWidget->setGeometry(0, 50 + this->height() / 2, this->width(), this->height()/2-50);
}

void EEngineMainWidget::showEvent(QShowEvent *event)
{
	//this->setAttribute(Qt::WA_Mapped);
	//EWidget::showEvent(event);
	// mStackedWidget->update();

}

int EEngineMainWidget::addengineWidget()
{
	mengineWidget = new RenderWindows();
	return 0;
}
