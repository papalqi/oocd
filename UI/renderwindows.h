#pragma once
#include "Engine.h"
#include "EWidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

#include <QDragEnterEvent>
#include <QMimeData>

class RenderWindows :public  EWidget
{
	Q_OBJECT
public:

	Engine* theApp;
	explicit RenderWindows(EWidget *parent = 0);
	~RenderWindows() {};

	//QSize sizeHint() const
	//{
	//	return QSize(270, 900); /* 在这里定义dock的初始大小 */
	//}
public:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void MouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);


protected:
	//drop拖拽
	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
private: 
	bool isBoardInput;
	bool isInMousemove;

	int			m_dragMode;
};