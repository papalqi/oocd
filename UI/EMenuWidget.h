#pragma once

#include "qwidget.h"
#include "qmenubar.h"
#include "qmenu.h"

class EMenuWidget : public QWidget
{
	Q_OBJECT

public:
	EMenuWidget(QWidget *parent = Q_NULLPTR);
	~EMenuWidget();

	QSize sizeHint() const
	{
		return QSize(1920, 30); /* �����ﶨ��dock�ĳ�ʼ��С */
	}
signals:

protected:
	void resizeEvent(QResizeEvent *rEvent);
	void paintEvent(QPaintEvent *pEvent);

private:
	void init();
	void initconnect();

private:
	QMenuBar	*mMenuBar;

	QMenu   *mMenu1;
};
