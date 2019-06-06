#include "EAnimationWidget.h"
#include "EMainWindow.h"
#include <QDebug>

EAnimationWidget *EAnimationWidget::sAnimationWidget = NULL;
EAnimationWidget &EAnimationWidget::instance()
{
    if (!sAnimationWidget)
    {
        sAnimationWidget = new EAnimationWidget(&EMainWindow::Instance());
    }
    return *sAnimationWidget;
}

void EAnimationWidget::startAnimation()
{
    //if(m_group->state() != QAbstractAnimation::Running)
    {
        this->showNormal();
        this->raise();
        m_group->start();
        isRunning = true;
    }
}

void EAnimationWidget::pauseAnimation()
{
    m_group->pause();
    this->hide();
}

void EAnimationWidget::stopAnimation()
{
    if (!isRunning)
    {

    }
    m_group->stop();
    this->hide();

}

void EAnimationWidget::appendAnimation(QList<SAnimationRule> temp)
{
    if (m_group->loopCount() == -1)
    {
        m_group->stop();
        m_group->setLoopCount(1);
    }

    if (temp.size() > 1)
    {
        QParallelAnimationGroup *theOpacityGroup = new QParallelAnimationGroup(this);
        m_OpacityGroupList.append(theOpacityGroup);

        foreach (auto theitem, temp) {
            PixmapItem *mPixmapItem = new PixmapItem(theitem.pix);
            m_PixmapItemList.append(mPixmapItem);
            //m_scene.addItem(mPixmapItem);//这是添加场景动画不到时间不动

            Animation* theAnimation = new Animation(mPixmapItem, theitem.prop);
            theAnimation->setRule(theitem);
            theOpacityGroup->addAnimation(theAnimation);


            connect(mPixmapItem, &PixmapItem::colling,this,[&](PixmapItem* P){

            });
        }
        m_group->addAnimation(theOpacityGroup);//theOpacityGroup添加动画后再add  add后直接触发CurrentAnimationChanged
    }
    else if(temp.size() == 1)
    {
        PixmapItem *mPixmapItem = new PixmapItem(temp[0].pix);
        m_PixmapItemList.append(mPixmapItem);
        //m_scene.addItem(mPixmapItem);

        Animation* theAnimation = new Animation(mPixmapItem, temp[0].prop);
        theAnimation->setRule(temp[0]);
        //m_group->addPause(temp.Duration);//暂停动画
        m_group->addAnimation(theAnimation);
    }
}

void EAnimationWidget::deleteAnimation()
{
    //clear 并行动画组内动画
    foreach (auto theitem, m_OpacityGroupList) {
             theitem->clear();
}

//    foreach (auto theitem, m_PixmapItemList) {
//             delete theitem;
//}
    m_OpacityGroupList.clear();
    m_PixmapItemList.clear();

    //clear  qdelete_all
    m_group->clear();
}

void EAnimationWidget::setAnimationLoopState(int c)
{
    m_group->setLoopCount(c);
}

void EAnimationWidget::slotAnimationFinish()
{
    deleteAnimation();
    emit signAnimationFinish();
    stopAnimation();
}

//串行动画中动画改变--动画开始前添加场景图
void EAnimationWidget::slotCurrentAnimationChanged(QAbstractAnimation *current)
{
    QParallelAnimationGroup *theGroup = dynamic_cast<QParallelAnimationGroup *>(current);
    if (theGroup != NULL)
    {
        m_scene.clear();//清空场景
        for(int i = 0; i < theGroup->animationCount(); i++)
        {
            Animation* theA = dynamic_cast<Animation *>(theGroup->animationAt(i));
            m_scene.addItem((PixmapItem *)theA->targetObject());
        }//添加并行动画组图
    }

    Animation *theAnimation = dynamic_cast<Animation *>(current);
    if (theAnimation != NULL)
    {
        m_scene.clear();//清空场景
        m_scene.addItem((PixmapItem *)theAnimation->targetObject());

        //        connect(theAnimation, &Animation::finished,this, [&](){
        //            qDebug() << "m_group count" << m_group->animationCount();
        //            //m_scene.clear();
        //            //m_group->removeAnimation(theAnimation);
        //        });
    }//添加动画图
}

EAnimationWidget::EAnimationWidget(QWidget *parent) : QWidget(parent)
{
	qDebug() << parent->width() << parent->height();
	resize(parent->width(), parent->height());
    init();
}

void EAnimationWidget::init()
{
    isRunning = false;
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground,true);

    setStyleSheet("QWidget{background-color:rgba(0,0,0,0.5);}");
    this->hide();

    mgraphicsView = new QGraphicsView(this);
    mgraphicsView->setScene(&m_scene);
    move(0,0);
    mgraphicsView->resize(this->width(), this->height());
    mEasingCurve = new QEasingCurve(QEasingCurve::OutInBounce);

    m_group = new QSequentialAnimationGroup(this);
    m_group->setDirection(QAbstractAnimation::Forward);     //动画方向
    m_group->setLoopCount(1);                               //一次


    connect(m_group, SIGNAL(finished()),this,SLOT(slotAnimationFinish()));
    connect(m_group,SIGNAL(currentAnimationChanged(QAbstractAnimation*)), \
            this, SLOT(slotCurrentAnimationChanged(QAbstractAnimation*)));

}

QRect PixmapItem::geometry() const
{
    return this->pixmap().rect();
}

void PixmapItem::setGeometry(QRect geometry)
{
    this->setPos(geometry.x(),geometry.y());
    setScale(geometry.width()/msacle);

    if (collidingItems().size() > 0)
        emit colling(this);

    //shape();
}

QRectF PixmapItem::boundingRect() const
{
    //qDebug() << "boundingRect";
    QRect rect = this->pixmap().rect();
    //return QRectF(rect);
    return QRectF(0,0,rect.width(),rect.width()+15);
}

//void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
//           QWidget *widget)
//{
//    //QPixmap pixmap = this->pixmap();
//    //qDebug() << "paint";
//        QMatrix leftmatrix;
//        leftmatrix.rotate((360.0 / 10) * 1);
//        QPixmap pix = this->pixmap().transformed(leftmatrix, Qt::SmoothTransformation);

//    QRect rect = this->pixmap().rect();

//    painter->drawPixmap(mgeometry,pix);


//    //print name,calculate the text's heigh & width for center layout
//    QPen pen(Qt::black);
//    painter->setPen(pen);
//    painter->setRenderHint(QPainter::Antialiasing);
//    //QFont font("Verdana",8, QFont::Normal);
//    //painter->setFont(font);
//    //painter->drawText(QRectF(0,rect.height(),rect.width(),15),Qt::AlignCenter,myText);

//    if (option->state & QStyle::State_Sunken)
//    {
//        //QRectF rect1 = boundingRect();
//        painter->setPen(QPen(Qt::darkGreen));
//    }else
//    {

//    }
//}
