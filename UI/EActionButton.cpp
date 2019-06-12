#include "EActionButton.h"
#include <QPainter>
#include <QStyleOption>
#include <qDebug>
EFileTypPix *EFileTypPix::I = nullptr;
EActionButton::EActionButton(QWidget *parent)
    : QAbstractButton(parent)
    , m_checked(false)
{
    init();
    initConnect();
}

void EActionButton::setPixMapNormal(const QPixmap &pixMap)
{
    m_PixMapNormal = pixMap;
}

void EActionButton::setPixMapPressed(const QPixmap &pixMap)
{
    m_PixMapPressed = pixMap;
}

void EActionButton::setItemName(const QString &sName,const QString &spath)
{
    m_sName = sName;
    m_sPath = spath;
}

void EActionButton::setRadius(int nTopLeft, int nTopRight, int nBottomRight, int nBottomLeft)
{
    m_nTopLeftRadius = nTopLeft;
    m_nTopRightRadius = nTopRight;
    m_nBottomRightRadius = nBottomRight;
    m_nBottomLeftRadius = nBottomLeft;

    updateStyleSheet();
}

void EActionButton::setActionEnum()
{
    //m_eAction = eAction;
}

//EActionButtonEnum EActionButton::getActionEnum()
//{
//    return m_eAction;
//}

void EActionButton::slot_click()
{
    //emit sigClick(m_sId);
    setChecked(true);
    updateStyleSheet();
}

void EActionButton::slot_Press()
{
    setChecked(true);
}

void EActionButton::slot_Relea()
{
    setChecked(false);
}

void EActionButton::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //painter
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setBold(true);
    painter.setFont(font);

    //name
    font.setPixelSize((int)(26*mMinFactor));
    painter.setPen(m_checked? QColor(32,200,242):QColor(135,141,160));
    painter.setFont(font);

    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WrapAnywhere);//换行
    QRect rect(0, 160*mMinFactor,this->width(),90*mMinFactor);
    //painter.drawText(rect,Qt::AlignHCenter | Qt::AlignVCenter,m_sName,option);
    painter.drawText(rect, m_sName, option);

    painter.drawPixmap((this->width()-106*mMinFactor)/2,6,106*mMinFactor,106*mMinFactor,m_checked? m_PixMapPressed:m_PixMapNormal);
}

void EActionButton::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    mWFactor = this->width()*1.0/baseSize().width();
    mHFactor = this->height()*1.0/baseSize().height();
    mMinFactor = qMin(mWFactor,mHFactor);
}

void EActionButton::mouseMoveEvent(QMouseEvent *event)
{

    if (m_checked)
    {
        QDrag drag(this);
//        if (m_dragStartPos.isNull())
//            goto end;
//        if ((event->pos() - m_dragStartPos).manhattanLength() < 5)
//            goto end;
        QMimeData* mimeData = new QMimeData();
		mimeData->setData("custom/name", m_sName.toLocal8Bit());
		mimeData->setData("custom/path", m_sPath.toLocal8Bit());
        mimeData->setColorData(QVariant(QColor(Qt::cyan)));
        mimeData->setImageData(QVariant(QPixmap(":/new/prefix1/res/CS_Network_icon_good.png")));
        drag.setMimeData(mimeData);
        drag.setPixmap(m_PixMapNormal);
        Qt::DropAction resultAction = drag.exec(Qt::MoveAction);

        if(resultAction == Qt::MoveAction)
        {
            // 确认移动操作
            // 该操作已经被目标操作接受，且操作类型为MOVE，一般来说应该把被拖动的物体从源窗口删除
        }

            //mouseMoveEvent(event);
    }
    else
        return;
}

void EActionButton::mousePressEvent(QMouseEvent *event)
{
    setChecked(true);
}

void EActionButton::mouseReleaseEvent(QMouseEvent *event)
{
    setChecked(false);
}

void EActionButton::init()
{
    setAcceptDrops(true);

    setBaseSize(255,147);
    setAutoFillBackground(true);  //设置自动填充
    setObjectName("actionButton");

    setAttribute(Qt::WA_TranslucentBackground, true);


    m_nTopLeftRadius = 8;
    m_nTopRightRadius = 8;
    m_nBottomRightRadius = 8;
    m_nBottomLeftRadius = 8;

    updateStyleSheet();
}

void EActionButton::initConnect()
{
    //connect(this,SIGNAL(pressed()),this,SLOT(slot_click()));
    //connect(this,SIGNAL(pressed()),this,SLOT(slot_Press()));
    //connect(this,SIGNAL(released()),this,SLOT(slot_Relea()));
}

bool EActionButton::getChecked() const
{
    return m_checked;
}

void EActionButton::setChecked(bool checked)
{
    m_checked = checked;
}

void EActionButton::updateStyleSheet()
{
    QString sStyle = QStringLiteral("EActionButton{background:%1;"
                                    "border-top-left-radius:%2px;"
                                    "border-top-right-radius:%3px;"
                                    "border-bottom-left-radius:%4px;"
                                    "border-bottom-right-radius:%5px;"
                                    "border:%6px solid white;"
                                    "}");
    setStyleSheet(sStyle.arg(m_checked? "rgba(233,248,255,0)":"rgba(255,255,255,0)")\
                  .arg(m_nTopLeftRadius).arg(m_nTopRightRadius).\
                  arg(m_nBottomLeftRadius).arg(m_nBottomRightRadius)\
                  .arg(m_checked? "2":"0"));
    update();
}
