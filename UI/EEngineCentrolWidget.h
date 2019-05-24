#pragma once
#include "ECentralWidget.h"
#include "EEnumData.h"

class EMainWindow;
class EEngineMainWidget;
class EEngineCentrolWidget : public ECentralWidget
{
	Q_OBJECT

public:
	explicit EEngineCentrolWidget(EMainWindow *parent);
	~EEngineCentrolWidget();

	void switchToPage(int pageIndex);
	virtual int currentPage();
	
signals:

public slots :
private:
	//目前不添加
	//int addEngineLoginWidget();
	//添加引擎主界面
	int addEngineMainWidget();

	EEngineMainWidget *mEnginMainWidget;

	EMainPageEnum mCurrentPageEnum;
};
