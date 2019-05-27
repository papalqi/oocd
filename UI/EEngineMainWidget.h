#pragma once
#include "EWidget.h"
#include <QStackedWidget>
#include <QDockWidget>
#include <QTreeWidget>
#include <QMainWindow>
#include <qmainwindow.h>
class RenderWindows;
class EMenuWidget;

class EEngineMainWidget : public QMainWindow
{
	Q_OBJECT
public:
	explicit EEngineMainWidget(QMainWindow *parent = nullptr);
	~EEngineMainWidget();
	void switchToPage(int index);

	//移除并隐藏所有dock
	void removeAllDock();
	//显示dock窗口
	void showDock(const QList<int>& index = QList<int>());
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
	RenderWindows *mengineWidget;

	EMenuWidget   *mMenuWidget;
	QPushButton *testBTN;


private:
	QList<QDockWidget*> mdocks;   //< 记录所有dockWidget的指针
};
