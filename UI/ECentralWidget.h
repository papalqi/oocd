#ifndef ECENTRALWIDGET_H
#define ECENTRALWIDGET_H

#include "EWidget.h"
#include <QStackedWidget>
class ETitleWidget;
class EMainWindow;
//class PDWaitWidget;
class ECentralWidget : public EWidget
{
    Q_OBJECT
public:
    explicit ECentralWidget(EMainWindow *parent = 0);
    virtual ~ECentralWidget();
    /// \param pageIndex 就是枚举PDMainPageEnum
    virtual void switchToPage(int pageIndex) = 0;
    virtual int currentPage() = 0;
    void setTitlebarBackgroundColor(const QColor& color);
    void enableMaskWidget(bool bEnable);
    int titleHeight();
    void setDisEnabledForWait(bool enabled);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    int addNewPage(QWidget *widget);
protected:
    ETitleWidget *mpTitleWidget;
    QStackedWidget *mpStackedWidget;
    QWidget *mMaskWidget;
    //PDWaitWidget *mWaitWidget;
    bool bDisEnabledForWait;
};

#endif // ECENTRALWIDGET_H
