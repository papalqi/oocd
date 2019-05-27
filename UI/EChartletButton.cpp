#include "EChartletButton.h"
#include <QPainter>
EChartletButton::EChartletButton(QWidget *parent)
    :QPushButton(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    mTextFont.setFamily(QStringLiteral("Microsoft YaHei"));
    mTextFont.setPixelSize(12);
}

void EChartletButton::setText(const QString text, const QRect rect)
{
    mButtonText = text;
    mTextRect = rect;
    update();
}
void EChartletButton::setTextFont(const QFont textFont,QColor fontColor)
{
    mTextFont = textFont;
    mFontColor = fontColor;
    update();
}

void EChartletButton::setChartlet(const QString normalPixmapPath,const QString pressedPixmapPath,const QString disablePixmapPath)
{
    //QString styleStr("QPushButton{background:transparent;border-image:url(%1)}"
     //             "QPushButton:pressed{background:transparent;border-image:url(%2)}"
     //             "QPushButton:!enabled{background:transparent;border-image:url(%3)}");
    //setStyleSheet(styleStr.arg(normalPixmapPath).arg(pressedPixmapPath).arg(disablePixmapPath));
}

void EChartletButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(mFontColor));
    painter.setFont(mTextFont);
    painter.drawText(mTextRect,Qt::AlignCenter,mButtonText);
}

void EChartletButton::setTextRect(const QRect textRect)
{
    mTextRect = textRect;
    update();
}

void EChartletButton::setButtonText(const QString buttonText)
{
    mButtonText = buttonText;
    update();
}
