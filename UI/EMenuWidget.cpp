#include "EMenuWidget.h"
#include "qdebug.h"

EMenuWidget::EMenuWidget(QWidget *parent)
	: QWidget(parent)
{
	init();
}

EMenuWidget::~EMenuWidget()
{
}

void EMenuWidget::init()
{
	mMenuBar = new QMenuBar(this);
	mMenu1 = new QMenu(this);
	mMenu1->addAction("´ó´óÉ³·¢");

	mMenuBar->addAction("aaaaaaaaaaaaa");
	mMenuBar->addAction("bbbbbbbbbbbbb");
	mMenuBar->addAction("ccccccccccccc");
	mMenu1 = mMenuBar->addMenu("aaaa");
	mMenu1->addAction("bbbb");
	mMenu1->addAction("ccc");
	mMenu1->addAction("dddd");
}

void EMenuWidget::initconnect()
{

}

void EMenuWidget::resizeEvent(QResizeEvent *rEvent)
{
	QWidget::resizeEvent(rEvent);
	mMenuBar->setGeometry(0, 0, this->width(), this->height());
}
void EMenuWidget::paintEvent(QPaintEvent *pEvent)
{

}