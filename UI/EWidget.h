#ifndef EWIDGET_H
#define EWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>

class EWidget:public QWidget
{
    Q_OBJECT
public:
    explicit EWidget(QWidget *parent = 0);
    virtual ~EWidget();
    inline int AW(const int width)
    {
        return width*mWFactor;
    }
    inline int AH(const int height)
    {
        return height*mHFactor;
    }
    void setfactor(const float wFactor,const float hFactor);
    void setBackgroundPixmap(const QPixmap& map);
    void setUseDeskTopFactor(bool used);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    /// \brief 背景图片
    QPixmap mBackgroundMap;
    /// \brief 当前适用图片
    QPixmap mCurrentMap;
    float mHFactor;
    float mWFactor;
    /// \brief 最小放缩比
    float mMinFactor;
    bool mUseDesktopFactor;
};

#endif // EWIDGET_H
