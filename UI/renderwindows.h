#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "Engine.h"

class RenderWindows :public  QWidget
{
	Q_OBJECT
public:

	Engine* theApp;
	explicit RenderWindows(QWidget *parent = 0);
	~RenderWindows() {};
public:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void MouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
private: 
	bool isInMousemove;
};