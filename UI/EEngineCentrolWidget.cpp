#include "EEngineCentrolWidget.h"
#include "EEngineMainWidget.h"

#include <qDebug>
#include <QDesktopWidget>
#include <QApplication>


EEngineCentrolWidget::EEngineCentrolWidget(EMainWindow *parent)
	: ECentralWidget(parent)
{
	//setTitlebarBackgroundColor(QColor(255, 255, 255));
	
	addEngineMainWidget();

	mCurrentPageEnum = enginPage;
}

EEngineCentrolWidget::~EEngineCentrolWidget()
{
}

void EEngineCentrolWidget::switchToPage(int pageIndex)
{
	if (pageIndex == enginPage)
	{
		mpStackedWidget->setCurrentWidget(mEnginMainWidget);
	}
	if (pageIndex == otherPage)
	{
		//
	}
	mCurrentPageEnum = (EMainPageEnum)pageIndex;
}


int EEngineCentrolWidget::currentPage()
{
	if (mpStackedWidget->currentWidget() == mEnginMainWidget)
	{
		return enginPage;
	}

	if (mpStackedWidget->currentWidget() == mEnginMainWidget)
	{
		return otherPage;
	}
	return enginPage;
}

int EEngineCentrolWidget::addEngineMainWidget()
{
	mEnginMainWidget = new EEngineMainWidget();
	return addNewPage((QWidget *)mEnginMainWidget);
}
