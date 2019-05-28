#include "ETitleWidget.h"
#include <QPalette>
#include <QStyleOption>
#include <QPainter>
ETitleWidget::ETitleWidget(QWidget *parent) :
    QWidget(parent)
{
    init();
}
void ETitleWidget::init()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumWidth(150);
    setBaseSize(1920,20);
    setFocusPolicy(Qt::NoFocus);
    //mBackgroundColor = QColor(217,244,255);
    mMinButton = new QPushButton(this);
    mMinButton->setObjectName("minButton");
    mQuitButton = new QPushButton(this);
    mQuitButton->setObjectName("quitButton");
    mMinButton->setFocusPolicy(Qt::NoFocus);
    mQuitButton->setFocusPolicy(Qt::NoFocus);
    connect(mMinButton,&QPushButton::clicked,this,&ETitleWidget::minimumSignnal);
    connect(mQuitButton,&QPushButton::clicked,this,&ETitleWidget::quitSignal);
}

void ETitleWidget::setBackgroundColor(const QColor &backgroundColor)
{
    mBackgroundColor = backgroundColor;
}

ETitleWidget::~ETitleWidget()
{

}

void ETitleWidget::resizeEvent(QResizeEvent *rEvent)
{
    QWidget::resizeEvent(rEvent);
    mQuitButton->setGeometry(this->width() - this->height(),0,this->height(),this->height());
    mMinButton->setGeometry(this->width() - this->height()*2,0,this->height(),this->height());
}

void ETitleWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(mBackgroundColor);
    painter.drawRect(this->rect());
    QWidget::paintEvent(pEvent);

}


