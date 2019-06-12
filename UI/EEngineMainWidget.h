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

	//�Ƴ�����������dock
	void removeAllDock();
	//��ʾdock����
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

	RenderWindows *mengineWidget;			//���洰��

	EMenuWidget   *mMenuWidget;				//�˵�����
	EResourceExplorerWidget *mResourceWidget;	//��Դ��ͼ����

	ETreeWidget   *mleftTreeWidget;

	QPushButton *testBTN;


private:
	QList<QDockWidget*> mdocks;   //< ��¼����dockWidget��ָ��
};
