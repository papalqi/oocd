#ifndef EANIMATIONWIDGET_H
#define EANIMATIONWIDGET_H

#include <QWidget>
#include "QStateMachine"
#include <QGraphicsPixmapItem>
#include <QEasingCurve>
#include <QParallelAnimationGroup>
#include <QtCore/qpropertyanimation.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSequentialAnimationGroup>
#include <QStyleOptionGraphicsItem>

class PixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    PixmapItem(const QPixmap &pix) : QGraphicsPixmapItem(pix)
    {
        msacle = pix.size().width();
        //this->setFlag(ItemSendsGeometryChanges);

        this->setOpacity(0.5);
        setTransformOriginPoint(pix.size().width()/2,\
                                pix.size().height()/2);//旋转原点

    }
    QRect geometry() const;
    void setGeometry(QRect geometry);

    QRectF boundingRect() const;
signals:
    void scaleChanged();
    void rotationChanged();
    void colling(PixmapItem *);
protected:
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

private:
    QRect getOffset(QRect G);
private:
    QRect mgeometry;
    qreal msacle;
};

struct SAnimationRule
{
    QPixmap   pix;
    QVariant  startValue;
    QVariant  endValue;
    float     Duration;
    QEasingCurve::Type animationType;
    QByteArray   prop;
};




class Animation : public QPropertyAnimation {
public:
    enum PathType {
        LinearPath,
        CirclePath,
        NPathTypes
    };
    Animation(QObject *target, const QByteArray &prop)
        : QPropertyAnimation(target, prop)
    {
        setPathType(LinearPath);
    }

    void setRule(SAnimationRule R)
    {
        setEasingCurve(R.animationType);
        setStartValue(R.startValue);
        setEndValue(R.endValue);
        setDuration(R.Duration);
    }

    void setPathType(PathType pathType)
    {
        if (pathType >= NPathTypes)
            qWarning("Unknown pathType %d", pathType);

        m_pathType = pathType;
        m_path = QPainterPath();
    }

    void updateCurrentTime(int currentTime) Q_DECL_OVERRIDE
    {
        if (m_pathType == CirclePath) {
            if (m_path.isEmpty()) {
                QPointF to = endValue().toPointF();
                QPointF from = startValue().toPointF();
                m_path.moveTo(from);
                m_path.addEllipse(QRectF(from, to));
            }
            int dura = duration();
            const qreal progress = ((dura == 0) ? 1 : ((((currentTime - 1) % dura) + 1) / qreal(dura)));

            qreal easedProgress = easingCurve().valueForProgress(progress);
            if (easedProgress > 1.0) {
                easedProgress -= 1.0;
            } else if (easedProgress < 0) {
                easedProgress += 1.0;
            }
            QPointF pt = m_path.pointAtPercent(easedProgress);
            updateCurrentValue(pt);
            emit valueChanged(pt);
        } else {
            QPropertyAnimation::updateCurrentTime(currentTime);
        }
    }

    QPainterPath m_path;
    PathType m_pathType;
};


class EAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    static EAnimationWidget& instance();

    void startAnimation();

    void pauseAnimation();

    void stopAnimation();

    void appendAnimation(QList<SAnimationRule> temp);

    void deleteAnimation();

    void setAnimationLoopState(int c);

signals:
    void signAnimationFinish();

public slots:
    void slotAnimationFinish();
    void slotCurrentAnimationChanged(QAbstractAnimation*current);

private:
    explicit EAnimationWidget(QWidget *parent = 0);

    void init();

private:
    static EAnimationWidget *sAnimationWidget;

    QStateMachine  *mStateMachine;

    Animation      *mAnimation;
    QEasingCurve   *mEasingCurve;

    QGraphicsScene  m_scene;
    PixmapItem     *m_item[100];

    QList<PixmapItem*>     m_PixmapItemList;
    QList<QParallelAnimationGroup *> m_OpacityGroupList; //并行动画组
//m_group
    QSequentialAnimationGroup *m_group;//串行动画组

    QGraphicsView *mgraphicsView;

    bool  isRunning;
};

#endif // EANIMATIONWIDGET_H
