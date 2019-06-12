#ifndef ERESOURCEVIEW_H
#define ERESOURCEVIEW_H

#include "EActionButton.h"

#include <QWidget>
#include <QScrollArea>
#include <QSplitter>

class ETreeWidget;

class EResourceViewWidget : public QWidget
{
	Q_OBJECT
public:
    EResourceViewWidget(QWidget *parent = 0);
    ~EResourceViewWidget();
	
    void loadResource(QString DirPath);
    void clearResorce();

protected:
    void resizeEvent(QResizeEvent *E);
    void paintEvent(QPaintEvent *E);

private slots:
    void slotBtnclick(int sid);

private:
    void init();

private:
    QList<EActionButton *> mBtns;
    QString mCurrentPath;
    int mRowSize, mColSize;


    QWidget *mScrollAreaWidget;
    QScrollArea *mScrollArea;
};

#endif // ERESOURCEVIEW_H
