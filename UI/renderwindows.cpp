#include "renderwindows.h"

 RenderWindows::RenderWindows(EWidget * parent) :EWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	theApp = new Engine((HINSTANCE)111);
	theApp->GetTimer()->Reset();
	//²¶×½ÊÂ¼þ
	
	setMouseTracking(true);
	if (!theApp->Initialize((HWND)this->winId()))
	{

	}
	isInMousemove = false;

}

 void RenderWindows::resizeEvent(QResizeEvent * event)
{

	//theApp->SetHeight(this->height());
	//theApp->SetWidth(this->width());
	//theApp->OnResize();
}

 void RenderWindows::paintEvent(QPaintEvent * event)
{
//	QString("asdasd");
	 auto debugString = theApp->GetFpsAndMspf().c_str();
	 if (debugString != "fuck")
	 {
		 parentWidget()->setWindowTitle(debugString);

	 }
	 if (isInMousemove)
	 {
		 theApp->OnMouseMove(WPARAM(1), 0, 0);
	 }


	 theApp->RunWithQTInOne();
	 update();

}

void RenderWindows::MouseMoveEvent(QMouseEvent* event)
{
	update();
}

void RenderWindows::mousePressEvent(QMouseEvent* event)
{
	isInMousemove = true;
}

void RenderWindows::mouseReleaseEvent(QMouseEvent *event)
{
	isInMousemove = false;

}

void RenderWindows::keyPressEvent(QKeyEvent *event)
{

}

