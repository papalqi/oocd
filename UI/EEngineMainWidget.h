#pragma once
#include "EWidget.h"
#include <QStackedWidget>
#include <QDockWidget>
#include <QTreeWidget>
#include <QMainWindow>

class RenderWindows;
class EMenuWidget;
class ETreeWidget;
class EResourceExplorerWidget;

class EEngineMainWidget : public QMainWindow
{
	Q_OBJECT
public:
	explicit EEngineMainWidget(EWidget *parent = nullptr);
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

	RenderWindows *mengineWidget;			//引擎窗口

	EMenuWidget   *mMenuWidget;				//菜单窗口
	EResourceExplorerWidget *mResourceWidget;	//资源视图窗口

	ETreeWidget   *mleftTreeWidget;

	QPushButton *testBTN;


private:
	QList<QDockWidget*> mdocks;   //< 记录所有dockWidget的指针
};
