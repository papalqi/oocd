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
	explicit RenderWindows(QWidget *parent = 0) :QWidget(parent)
	{
		setAttribute(Qt::WA_PaintOnScreen, true);
		//setAttribute(Qt::WA_NativeWindow, true);
		 theApp=new Engine((HINSTANCE)111);
		 theApp->GetTimer()->Reset();

		if (!theApp->Initialize((HWND)this->winId()))
		{

		}
		
	};
	~RenderWindows() {};
	void resizeEvent(QResizeEvent *event)
	{
		
		theApp->SetHeight(this->height());
		theApp->SetWidth(this->width());
		theApp->OnResize();
	}

	void paintEvent(QPaintEvent *event)
	{
		QString("asdasd");

		parentWidget()->setWindowTitle(theApp->GetFpsAndMspf().c_str());
		
		theApp->RunWithQTInOne();
		update();

	}
};