#pragma once

#include <QWidget>
#include <QSplitter>


class ETreeWidget;
class EResourceViewWidget;

class EResourceExplorerWidget : public QWidget
{
	Q_OBJECT

public:
	EResourceExplorerWidget(QWidget *parent);
	~EResourceExplorerWidget();

	QSize sizeHint() const
	{
		return QSize(this->width(), 300); /* 在这里定义dock的初始大小 */
	}
protected:
	void resizeEvent(QResizeEvent *E);
private slots:
	void slotTreeChange(QString fpath);
private:
	void init();
private:
	ETreeWidget				*mDirWidget;
	EResourceViewWidget		*mResourceViewWidget;
	QSplitter				*mSpliter;
};
