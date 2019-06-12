#include "EResourceViewWidget.h"
#include "ETreeWidget.h"


EResourceViewWidget::EResourceViewWidget(QWidget *parent)
    : QWidget(parent)
{
    init();
}

EResourceViewWidget::~EResourceViewWidget()
{

}

void EResourceViewWidget::loadResource(QString DirPath)
{
	clearResorce();

	QDir dir(DirPath);
	if (!dir.exists()) {
		return;
	}
	mCurrentPath = DirPath + "/";
	QStringList filelist;
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
	QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
	foreach(QFileInfo file, fileList) { //遍历文件信息
		if (file.isFile())
			filelist.append(file.fileName());
	}

	for (int i = 0; i < filelist.size(); i++)
	{
		QStringList theFileList = filelist[i].split('.');
		if (theFileList.size() <= 1)
			continue;
		QString theFileTyp = "." + theFileList.last();

		EActionButton *tBtn = new EActionButton(this);
		tBtn->setItemName(filelist[i], mCurrentPath);
		tBtn->setPixMapNormal(QPixmap(EFileTypPix::instance().getPixFromTyp(theFileTyp)));
		tBtn->setPixMapPressed(QPixmap(EFileTypPix::instance().getPixFromTyp(theFileTyp)));
		mBtns.append(tBtn);
	}
	repaint();
}

void EResourceViewWidget::clearResorce()
{
    qDeleteAll(mBtns);
    mBtns.clear();
}

void EResourceViewWidget::resizeEvent(QResizeEvent *E)
{
	mScrollAreaWidget->resize(this->size());
	mScrollArea->resize(this->size());
}

void EResourceViewWidget::paintEvent(QPaintEvent *E)
{
    mColSize = this->width()/90;
	if (mColSize <= 0) mColSize = 1;
    int toffset = 0;
    foreach (auto item, mBtns) {
        item->setGeometry(8 + 90*(toffset%mColSize), \
                          8 + 90*(toffset/mColSize), \
                          80,80);
		item->show();
        toffset++;
    }
}

void EResourceViewWidget::slotBtnclick(int sid)
{

}

void EResourceViewWidget::init()
{
    mScrollArea = new QScrollArea(this);
    mScrollAreaWidget = new QWidget(this);
    mRowSize = 5; mColSize = 15;

    //mScrollAreaWidget->setMinimumSize(860,480);
    mScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mScrollArea->setWidget(mScrollAreaWidget);

    mCurrentPath = "/";
	loadResource("E:/tool");
}
