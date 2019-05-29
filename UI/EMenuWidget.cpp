#include "EMenuWidget.h"
#include "qdebug.h"

EMenuWidget::EMenuWidget(QWidget *parent)
	: QWidget(parent)
{

	init();
	initconnect();
}

EMenuWidget::~EMenuWidget()
{
}

void EMenuWidget::init()
{
	mMenuBar = new QMenuBar(this);
	QStringList theMenu,theAct1,theAct2,theAct3,theAct4;
	theMenu << "File" << "Edit" << "View" << "Help";
	for (int i = 0; i < theMenu.size(); i++)
	{
		mMenu[i] = new QMenu(theMenu[i]);
		mMenuBar->addMenu(mMenu[i]);
	}

	//File
	mAct[0] = new QAction("New Scence", this);
	mAct[0]->setShortcut(Qt::CTRL | Qt::Key_N);
	mAct[0]->setStatusTip(tr("这是一个新建菜单"));

	mAct[1] = new QAction("Open", this);
	mAct[1]->setShortcut(Qt::CTRL | Qt::Key_O);
	mAct[1]->setCheckable(true);

	mmMenu[0] = new QMenu("Open recent");
	mmMenu[1] = new QMenu("Demo Scenes");

	mAct[2] = new QAction("Save", this);
	mAct[2]->setShortcut(Qt::CTRL | Qt::Key_S);

	mAct[3] = new QAction("Save as", this);
	mAct[3]->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);

	mAct[3] = new QAction("Import", this);
	mAct[3]->setShortcut(Qt::CTRL | Qt::Key_I);

	mAct[4] = new QAction("Merge", this);
	mAct[4]->setShortcut(Qt::CTRL | Qt::Key_M);

	mAct[5] = new QAction("Save", this);
	mAct[5]->setShortcut(Qt::CTRL | Qt::Key_S);

	mAct[6] = new QAction("Quit", this);
	mAct[6]->setShortcut(Qt::CTRL | Qt::Key_Q);

	mMenu[0]->addAction(mAct[0]);
	mMenu[0]->addSeparator();
	mMenu[0]->addAction(mAct[1]);
	mMenu[0]->addMenu(mmMenu[0]);
	mMenu[0]->addMenu(mmMenu[1]);
	mMenu[0]->addSeparator();
	mMenu[0]->addAction(mAct[2]);
	mMenu[0]->addAction(mAct[3]);
	mMenu[0]->addSeparator();
	mMenu[0]->addAction(mAct[4]);
	mMenu[0]->addAction(mAct[5]);
	mMenu[0]->addSeparator();
	mMenu[0]->addAction(mAct[6]);

	mAct[6] = new QAction("Undo", this);
	mAct[6]->setShortcut(Qt::CTRL | Qt::Key_Z);

	mAct[7] = new QAction("Redo", this);
	mAct[7]->setShortcut(Qt::CTRL | Qt::Key_Y);

	mAct[8] = new QAction("Copy", this);
	mAct[8]->setShortcut(Qt::CTRL | Qt::Key_Q);

	mAct[9] = new QAction("Paste", this);
	mAct[9]->setShortcut(Qt::CTRL | Qt::Key_Q);

	mAct[10] = new QAction("Delete", this);
	mAct[10]->setShortcut(Qt::Key_Delete);

	mAct[11] = new QAction("Resource collector", this);

	mAct[12] = new QAction("Missing image file", this);

	mAct[13] = new QAction("Port", this);

	mAct[14] = new QAction("Preferences", this);
	mAct[14]->setShortcut(Qt::CTRL | Qt::Key_P);

	mMenu[1]->addAction(mAct[6]);
	mMenu[1]->addAction(mAct[7]);
	mMenu[1]->addSeparator();
	mMenu[1]->addAction(mAct[8]);
	mMenu[1]->addAction(mAct[9]);
	mMenu[1]->addAction(mAct[10]);
	mMenu[1]->addSeparator();
	mMenu[1]->addAction(mAct[11]);
	mMenu[1]->addAction(mAct[12]);
	mMenu[1]->addAction(mAct[13]);
	mMenu[1]->addSeparator();
	mMenu[1]->addAction(mAct[14]);

	mAct[80] = new QAction("Website", this);

	mAct[81] = new QAction("Forum", this);

	mAct[82] = new QAction("Youtube channel", this);
	mAct[82]->setShortcut(Qt::Key_F1);

	mAct[83] = new QAction("Welcome", this);

	mmMenu[19] = new QMenu("Shortcuts");

	mAct[84] = new QAction("About Twinmotion", this);

	mMenu[3]->addAction(mAct[80]);
	mMenu[3]->addAction(mAct[81]);
	mMenu[3]->addAction(mAct[82]);
	mMenu[3]->addSeparator();
	mMenu[3]->addAction(mAct[83]);
	mMenu[3]->addMenu(mmMenu[19]);
	mMenu[3]->addSeparator();
	mMenu[3]->addAction(mAct[84]);
}

void EMenuWidget::initconnect()
{
	bool a = connect(mMenuBar, SIGNAL(triggered(QAction*)), this, SLOT(slotMenuTriggered(QAction*)));
}

void EMenuWidget::resizeEvent(QResizeEvent *rEvent)
{
	QWidget::resizeEvent(rEvent);
	mMenuBar->setGeometry(0, 0, this->width(), this->height());
}
void EMenuWidget::paintEvent(QPaintEvent *pEvent)
{

}

void EMenuWidget::slotMenuTriggered(QAction *act)
{
	qDebug() << act->text();
}