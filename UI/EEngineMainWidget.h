#pragma once
#include "EWidget.h"
#include <QStackedWidget>
#include <QDockWidget>

class RenderWindows;
class EMenuWidget;

class EEngineMainWidget : public EWidget
{
	Q_OBJECT
public:
	explicit EEngineMainWidget(EWidget *parent);
	~EEngineMainWidget();
	void switchToPage(int index);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

private:
	void init();
	void initconnect();

	int addengineWidget();

private:
	QStackedWidget *mStackedWidget;
	QDockWidget *mDockWidget;
	RenderWindows *mengineWidget;

	EMenuWidget   *mMenuWidget;
	QPushButton *testBTN;
};
