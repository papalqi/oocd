#pragma once

#include <QTreeView>
#include <QDirModel>
class ETreeWidget : public QTreeView
{
	Q_OBJECT

public:
	ETreeWidget(QWidget *parent);
	~ETreeWidget();

	QSize sizeHint() const
	{
		return QSize(300, this->height()); /* �����ﶨ��dock�ĳ�ʼ��С */
	}
signals:

public slots :
	void slotMenuTriggered(QAction *act);

protected:
	void resizeEvent(QResizeEvent *rEvent);
	void paintEvent(QPaintEvent *pEvent);
	void keyPressEvent(QKeyEvent *event);

private:
	void init();
	void initconnect();

private:
	QTreeView	*mTreeView;
	QDirModel	*mModel;
};
