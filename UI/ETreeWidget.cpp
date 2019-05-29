#include "ETreeWidget.h"

ETreeWidget::ETreeWidget(QWidget *parent)
	: QTreeView(parent)
{
	grabKeyboard();
	setFocusPolicy(Qt::NoFocus);

	init();
	initconnect();
}

ETreeWidget::~ETreeWidget()
{
}

void ETreeWidget::init()
{
	mModel = new QDirModel;
	mModel->setReadOnly(false);
	mModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
	//mModel->setFilterRegExp("[^C:c:].*");//过滤
	//从缺省目录创建数据  
	mTreeView = new QTreeView(this);
	mTreeView->setModel(mModel);
	//mTreeView->setRootIndex());
	QModelIndex index = mModel->index(QDir::currentPath());
	mTreeView->scrollTo(index);
	mTreeView->resizeColumnToContents(0);
	//配置一个view去显示model中的数据，只需要简单地调用setModel(),并把目录model作为参数传递  
		//setRootIndex()告诉views显示哪个目录的信息，这需要提供一个model index,然后用这个  
		//model index去model中去获取数据  
		//index()这个函数是QDirModel特有的，通过把一个目录做为参数，得到了需要的model index  
		//其他的代码只是窗口show出来,进入程序的事件循环就好了  
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));   //中文显示  
	//QTextCodec *codec = QTextCodec::codecForLocale();
	//QString a = codec->toUnicode("目录");
	setWindowTitle("explorer");




	//树，用于显示文件和文件管理
	//mleftTreeWidget = new ETreeWidget(this);
	//QTreeWidget *mTreeWidge = new QTreeWidget(this);

	//for (int j = 0; j < 3; j++)
	//{
	//	QString mRootName = "root" + QString::number(j + 1);
	//	QTreeWidgetItem *mRoot = new QTreeWidgetItem(mTreeWidge);
	//	mRoot->setText(0, mRootName);

	//	mRoot->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	//	QString mItemName;

	//	for (int i = 0; i < 3; i++)
	//	{
	//		mItemName = "child" + QString::number(i + 1);
	//		QTreeWidgetItem *mChild1 = new QTreeWidgetItem(mRoot);
	//		mChild1->setText(0, mItemName);

	//		mChild1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	//	}
	//}
}
void ETreeWidget::initconnect()
{

}



void ETreeWidget::paintEvent(QPaintEvent *event)
{
	//EWidget::paintEvent(event);
}

void ETreeWidget::keyPressEvent(QKeyEvent * event)
{
}

void ETreeWidget::resizeEvent(QResizeEvent *event)
{
	//EWidget::resizeEvent(event);
	mTreeView->setGeometry(0, 0, this->width(), this->height());
}

void ETreeWidget::slotMenuTriggered(QAction *act)
{

}
