#include "EWidget.h"
#include <QPainter>
#include <QStyleOption>
#include "common/CommonFun.h"


EWidget::EWidget(QWidget *parent)
    :QWidget(parent)
{
    mUseDesktopFactor = true;
    //缩放比例相关
    mWFactor = CommonFun::getWidthFactor();
    mHFactor = CommonFun::getHeightFactor();
    mMinFactor = qMin(mWFactor,mHFactor);
}

EWidget::~EWidget()
{

}

void EWidget::setfactor(const float wFactor, const float hFactor)
{
    mWFactor = wFactor;
    mHFactor = hFactor;
    mMinFactor = qMin(mWFactor,mHFactor);
}

void EWidget::setBackgroundPixmap(const QPixmap &map)
{
    mBackgroundMap = map;
    mCurrentMap = mBackgroundMap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    update();
}

void EWidget::setUseDeskTopFactor(bool used)
{
    mUseDesktopFactor = used;
}

void EWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);

    if(!mCurrentMap.isNull())
    {
        if(mCurrentMap.width() != this->width() && mCurrentMap.height() != this->height())
        {
            // mCurrentMap = mBackgroundMap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        }

        QRect sourceRect(0,0,this->width(),this->height());
        QRect targetRect(0,0,this->width(),this->height());


        if(mCurrentMap.width() > this->width())
        {
            sourceRect.setX(mCurrentMap.width()/2 -  this->width()/2);
            sourceRect.setWidth(this->width());
        }

        if(mCurrentMap.height() > this->height())
        {
            sourceRect.setY(mCurrentMap.height()/2 -  this->height()/2);
            sourceRect.setHeight(this->height());
        }
        painter.drawPixmap(targetRect,mCurrentMap,sourceRect);
    }
}

void EWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if(!mUseDesktopFactor)
    {
        if(baseSize().width() !=0 && baseSize().height() !=0)
        {
            mWFactor = this->width()*1.0/baseSize().width();
            mHFactor = this->height()*1.0/baseSize().height();
            mMinFactor = qMin(mWFactor,mHFactor);
        }
    }

    if(!mBackgroundMap.isNull())
    {
        mCurrentMap = mBackgroundMap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    }
}



