#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "Engine.h"

class D3d12RenderWidget :public  QWidget
{
	Q_OBJECT
public:

	Engine* theApp;
	explicit D3d12RenderWidget(QWidget *parent = 0) :QWidget(parent) 
	{
		setAttribute(Qt::WA_PaintOnScreen, true);
		setAttribute(Qt::WA_NativeWindow, true);
		 theApp=new Engine((HINSTANCE)111);
		 theApp->GetTimer()->Reset();

		if (!theApp->Initialize((HWND)this->winId()))
		{

		}
		
	};
	~D3d12RenderWidget() {};


	void D3d12RenderWidget::paintEvent(QPaintEvent *event)
	{
		theApp->RunWithQTInOne();
		update();

	}
};