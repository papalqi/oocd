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
	//mModel->setFilterRegExp("[^C:c:].*");//����
	//��ȱʡĿ¼��������  
	mTreeView = new QTreeView(this);
	mTreeView->setModel(mModel);
	//mTreeView->setRootIndex());
	QModelIndex index = mModel->index(QDir::currentPath());
	mTreeView->scrollTo(index);
	mTreeView->resizeColumnToContents(0);
	//����һ��viewȥ��ʾmodel�е����ݣ�ֻ��Ҫ�򵥵ص���setModel(),����Ŀ¼model��Ϊ��������  
		//setRootIndex()����views��ʾ�ĸ�Ŀ¼����Ϣ������Ҫ�ṩһ��model index,Ȼ�������  
		//model indexȥmodel��ȥ��ȡ����  
		//index()���������QDirModel���еģ�ͨ����һ��Ŀ¼��Ϊ�������õ�����Ҫ��model index  
		//�����Ĵ���ֻ�Ǵ���show����,���������¼�ѭ���ͺ���  
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));   //������ʾ  
	//QTextCodec *codec = QTextCodec::codecForLocale();
	//QString a = codec->toUnicode("Ŀ¼");
	setWindowTitle("explorer");




	//����������ʾ�ļ����ļ�����
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
