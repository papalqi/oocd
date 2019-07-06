#include "renderwindows.h"
#include <QDebug>
 RenderWindows::RenderWindows(EWidget * parent) :EWidget(parent)
{
	setAcceptDrops(true);

	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	setFocusPolicy(Qt::ClickFocus);
	theApp = new Engine((HINSTANCE)111);
	theApp->GetTimer()->Reset();
	//捕捉事件
	
	setMouseTracking(true);
	if (!theApp->Initialize((HWND)this->winId()))
	{

	}
	this->activateWindow();
	
	//this->setFocus();

}

 
 void RenderWindows::enterEvent(QEvent * event)
 {
	 qDebug() << "i";
	 if (this->isActiveWindow())  //是否为活动窗体
		 theApp->isBoardInput = true;
 }

 void RenderWindows::leaveEvent(QEvent * event)
 {
	 qDebug() << "o";
	 theApp->isBoardInput = false;
 }

 void RenderWindows::focusOutEvent(QFocusEvent *event)
 {
	 __super::focusOutEvent(event);

	 
 }

 void RenderWindows::focusInEvent(QFocusEvent *event)
 {
	 __super::focusInEvent(event);

 }

 void RenderWindows::resizeEvent(QResizeEvent * event)
{

	 theApp->SetHeight(this->height());
	 theApp->SetWidth(this->width());
	 theApp->OnResize();
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
		 setCursor(QCursor(Qt::BlankCursor));
		 theApp->OnMouseMove(WPARAM(1), 0, 0);
	 }
	 else
		 this->setCursor(Qt::ArrowCursor);


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
	__super::mousePressEvent(event);

	
}

void RenderWindows::mouseReleaseEvent(QMouseEvent *event)
{
	__super::mouseReleaseEvent(event);
	isInMousemove = false;


}

void RenderWindows::keyPressEvent(QKeyEvent *event)
{
	//isBoardInput= this->geometry().contains(this->mapFromGlobal(QCursor::pos()))? true : false;
	//theApp->isBoardInput= isBoardInput ? true : false;

}

void RenderWindows::dragEnterEvent(QDragEnterEvent *event)
{

	//qDebug("dragEnterEvent: enter drag mode ............");
	if (event->mimeData()->hasFormat("custom/name"))
	{
		m_dragMode = 1;
		event->acceptProposedAction();
	}
	else
	{
		event->ignore();
		//dragEnterEvent(event);
	}
}

void RenderWindows::dragLeaveEvent(QDragLeaveEvent *event)
{
	//qDebug("exit topotree, exit drag mode ............");
	m_dragMode = 0;

	//dragLeaveEvent(event);
}

void RenderWindows::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("custom/name"))
	{
		//QTreeWidgetItem* itemOver = itemAt(event->pos());
		//if (itemOver == NULL)
		//{
		//}
		//else
		//{
			// accept
			event->acceptProposedAction();
			//dragMoveEvent(event);
			return;
		//}
	}

	event->ignore();
	//dragMoveEvent(event);
}

void RenderWindows::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat("custom/name"))
	{
		//QTreeWidgetItem* itemOver = itemAt(event->pos());
		//if (itemOver != NULL)
		{
			// 接收此MoveAction，插入节点

			// 提取drag中携带的信息
			QByteArray filename = event->mimeData()->data("custom/name");
			QByteArray filepath = event->mimeData()->data("custom/path");
			m_dragMode = 0; // 结束drag mode

			theApp->BuildOBJMesh(filename.toStdString(), filepath.toStdString());

			event->acceptProposedAction();
			//QTreeWidget::dropEvent(event);
			return;
		}
	}

	event->ignore();
	dropEvent(event);
}

