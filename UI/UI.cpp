#include "UI.h"
#include <renderwindows.h>
#include <qtimer.h>
UI::UI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//RenderWindows *graphicsView = new RenderWindows(this);
	//setCentralWidget(graphicsView);
	this->setMouseTracking(true);
	RenderWindows  *graphicsView = new RenderWindows(this);
	graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
	graphicsView->setGeometry(QRect(180, 10, 800, 600));
	//ÉèÖÃË¢ÐÂÆµÂÊ
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), graphicsView, SLOT(paintEvent()));
	timer->start(0.001);


}

void UI::SetLineEdited()
{
	auto lineEdit=ui.lineEdit;
	connect(lineEdit,SIGNAL(returnPressed()), this, SLOT(CommendReture()));
}

void UI::CommendReture()
{
	int fuck = 111;
}
