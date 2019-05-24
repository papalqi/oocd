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
	//Ŀǰ�����
	//int addEngineLoginWidget();
	//�������������
	int addEngineMainWidget();

	EEngineMainWidget *mEnginMainWidget;

	EMainPageEnum mCurrentPageEnum;
};
