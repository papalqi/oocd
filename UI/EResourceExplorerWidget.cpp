#include "EResourceExplorerWidget.h"
#include "ETreeWidget.h"
#include "EResourceViewWidget.h"
#include <qDebug>
EResourceExplorerWidget::EResourceExplorerWidget(QWidget *parent)
	: QWidget(parent)
{
	init();
}

EResourceExplorerWidget::~EResourceExplorerWidget()
{
}

void EResourceExplorerWidget::resizeEvent(QResizeEvent *E)
{
	mSpliter->resize(this->size());
}

void EResourceExplorerWidget::init()
{
	mSpliter = new QSplitter(Qt::Horizontal,this);

	mDirWidget = new ETreeWidget(mSpliter);
	mResourceViewWidget = new EResourceViewWidget(mSpliter);

	mSpliter->addWidget(mDirWidget);
	mSpliter->addWidget(mResourceViewWidget);

	mSpliter->setStretchFactor(0, 1);//设置左右两列的宽度比为1：4
	mSpliter->setStretchFactor(1, 8);

	mSpliter->show();


	connect(mDirWidget, SIGNAL(sig_treeTriggered(QString)), this, SLOT(slotTreeChange(QString)));
}


void EResourceExplorerWidget::slotTreeChange(QString fpath)
{
	mResourceViewWidget->loadResource(fpath);
}