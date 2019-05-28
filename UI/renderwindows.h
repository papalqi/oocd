#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "Engine.h"
#include "EWidget.h"

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
	virtual void focusOutEvent(QFocusEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void MouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
private: 
	bool isBoardInput;
	bool isInMousemove;
};