#include "EMessageBox.h"
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <qDebug>
#include "common/CommonFun.h"
#include "EChartletButton.h"

EMessageBox::EMessageBox(QWidget *parent)
    :QDialog(parent)
{
    setBaseSize(584,356);
    mBkgColor = QColor(245,251,255);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    mCancelButton = new EChartletButton(this);
    mCancelButton->setButtonText(QStringLiteral("取消"));

    mConfirmButton = new EChartletButton(this);
    mConfirmButton->setButtonText(QStringLiteral("确认"));

    mWarnButton = new EChartletButton(this);
    mWarnButton->setButtonText(QStringLiteral("确认"));

    mCloseButton = new QPushButton(this);
    mCloseButton->setIcon(QIcon(":/images/common/TK_cancel_icon_normal.png"));
    mCloseButton->setIconSize(QSize(25,25));
    mCloseButton->setStyleSheet("QPushButton{background:transparent;}");

    mMessageLabel = new QLabel(this);
    mMessageLabel->setAlignment(Qt::AlignCenter);
    mMessageLabel->setWordWrap(true);
    mMessageLabel->setText(mMessageText);
    QPalette pal;
    pal.setColor(QPalette::WindowText,QColor(81,82,89));
    mMessageLabel->setPalette(pal);

    mAwardCountLabel = new QLabel(this);
    mAwardCountLabel->setAlignment(Qt::AlignCenter);
    mAwardCountLabel->setWordWrap(true);
    mAwardCountLabel->setText(mMessageText);
    pal.setColor(QPalette::WindowText,QColor(255,182,53));
    mAwardCountLabel->setPalette(pal);

    mExtraColoredTextLabel = new QLabel(this);
    mExtraColoredTextLabel->setAlignment(Qt::AlignCenter);
    mExtraColoredTextLabel->setWordWrap(true);
    mExtraColoredTextLabel->setText(mMessageText);

    setMessageType(MessageBoxType_Confirm);

    connect(mCloseButton,SIGNAL(clicked()),this,SLOT(slot_cancelButtonClicked()));
    connect(mConfirmButton,SIGNAL(clicked()),this,SLOT(slot_confirmButtonClicked()));
    connect(mWarnButton,SIGNAL(clicked()),this,SLOT(slot_confirmButtonClicked()));
    connect(mCancelButton,SIGNAL(clicked()),this,SLOT(slot_cancelButtonClicked()));
}

void EMessageBox::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(mBkgColor);

    painter.drawRoundedRect(rect(),20,20);

    if(mType == MessageBoxType_Info)
    {
        painter.drawPixmap(218*mMinFactor,112*mMinFactor,58*mMinFactor,47*mMinFactor,mAwardPixmap);
    }

    resize(this->baseSize().width()*mMinFactor,this->baseSize().height()*mMinFactor);

    move(QApplication::desktop()->screen()->width()/2 - this->width()/2,QApplication::desktop()->screen()->height()/2 - this->height()/2);
}

void EMessageBox::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    mWFactor = CommonFun::getWidthFactor();
    mHFactor = CommonFun::getHeightFactor();
    mMinFactor = qMin(mWFactor,mHFactor);

    QFont font(QStringLiteral("Microsoft YaHei"));
    font.setPixelSize(28*mMinFactor);

    if(mType == MessageBoxType_Confirm)
    {
        mWarnButton->setGeometry(this->width()/2-351*mMinFactor/2,240*mMinFactor,351*mMinFactor,97*mMinFactor);
        mCloseButton->setGeometry(this->width() - 45*mMinFactor,20*mMinFactor,25*mMinFactor,25*mMinFactor);

        mWarnButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mWarnButton->width(),35*mMinFactor));
        mWarnButton->setTextFont(font,QColor(Qt::white));

        font.setFamily("Microsoft YaHei");
        font.setBold(false);
        int fontSize = (int)(25*mMinFactor);
        font.setPixelSize(fontSize);

        mMessageLabel->setFont(font);
        mMessageLabel->setGeometry(42*mMinFactor,61*mMinFactor,498*mMinFactor,121*mMinFactor);
    }
    else if(mType == MessageBoxType_Question)
    {
        mConfirmButton->setGeometry(292*mMinFactor,240*mMinFactor,244*mMinFactor,99*mMinFactor);
        mCancelButton->setGeometry(47*mMinFactor,240*mMinFactor,244*mMinFactor,99*mMinFactor);
        mCloseButton->setGeometry(this->width() - 45*mMinFactor,20*mMinFactor,25*mMinFactor,25*mMinFactor);

        mConfirmButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mConfirmButton->width(),35*mMinFactor));
        mCancelButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mCancelButton->width(),35*mMinFactor));
        mConfirmButton->setTextFont(font,QColor(Qt::white));
        mCancelButton->setTextFont(font,QColor(Qt::white));


        font.setFamily("Microsoft YaHei");
        font.setBold(false);
        int fontSize = (int)(25*mMinFactor);
        font.setPixelSize(fontSize);

        mMessageLabel->setFont(font);
        mMessageLabel->setGeometry(42*mMinFactor,61*mMinFactor,498*mMinFactor,121*mMinFactor);
    }
    else if(mType == MessageBoxType_Question2)
    {
        mCancelButton->setGeometry(292*mMinFactor,240*mMinFactor,244*mMinFactor,99*mMinFactor);
        mConfirmButton->setGeometry(47*mMinFactor,240*mMinFactor,244*mMinFactor,99*mMinFactor);
        mCloseButton->setGeometry(this->width() - 45*mMinFactor,20*mMinFactor,25*mMinFactor,25*mMinFactor);

        mCancelButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mCancelButton->width(),35*mMinFactor));
        mConfirmButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mConfirmButton->width(),35*mMinFactor));
        mCancelButton->setTextFont(font,QColor(Qt::white));
        mConfirmButton->setTextFont(font,QColor(Qt::white));


        font.setFamily("Microsoft YaHei");
        font.setBold(false);
        int fontSize = (int)(25*mMinFactor);
        font.setPixelSize(fontSize);

        mMessageLabel->setFont(font);
        mMessageLabel->setGeometry(42*mMinFactor,61*mMinFactor,498*mMinFactor,121*mMinFactor);
    }
    else if(mType == MessageBoxType_Info)
    {
        mWarnButton->setGeometry(this->width()/2-351*mMinFactor/2,188*mMinFactor,351*mMinFactor,97*mMinFactor);
        mWarnButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mWarnButton->width(),35*mMinFactor));
        mWarnButton->setTextFont(font,QColor(Qt::white));

        font.setFamily("Microsoft YaHei");
        font.setBold(false);
        int fontSize = (int)(25*mMinFactor);
        font.setPixelSize(fontSize);

        mAwardCountLabel->setFont(font);
        mAwardCountLabel->setGeometry(289*mMinFactor,118*mMinFactor,79*mMinFactor,34*mMinFactor);

        mMessageLabel->setFont(font);
        mMessageLabel->setGeometry(42*mMinFactor,39*mMinFactor,498*mMinFactor,43*mMinFactor);
    }
    else if(mType == MessageBoxType_Countdown)
    {
        mConfirmButton->setGeometry(292*mMinFactor,240*mMinFactor,244*mMinFactor,99*mMinFactor);
        mCancelButton->setGeometry(47*mMinFactor,240*mMinFactor,244*mMinFactor,99*mMinFactor);
        mCloseButton->setGeometry(this->width() - 45*mMinFactor,20*mMinFactor,25*mMinFactor,25*mMinFactor);

        mConfirmButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mConfirmButton->width(),35*mMinFactor));
        mCancelButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mCancelButton->width(),35*mMinFactor));
        mConfirmButton->setTextFont(font,QColor(Qt::white));
        mCancelButton->setTextFont(font,QColor(Qt::white));

        font.setFamily("Microsoft YaHei");
        font.setBold(false);
        int fontSize = (int)(25*mMinFactor);
        font.setPixelSize(fontSize);

        mMessageLabel->setFont(font);
        mMessageLabel->setGeometry(42*mMinFactor,61*mMinFactor,498*mMinFactor,121*mMinFactor);

        mAwardCountLabel->setFont(font);
        mAwardCountLabel->setGeometry(this->width()/2-32*mMinFactor/2,200*mMinFactor,32*mMinFactor,32*mMinFactor);
    }
    else if(mType == MessageBoxType_ExtraColorText)
    {
        mWarnButton->setGeometry(this->width()/2-351*mMinFactor/2,240*mMinFactor,351*mMinFactor,97*mMinFactor);
        mCloseButton->setGeometry(this->width() - 45*mMinFactor,20*mMinFactor,25*mMinFactor,25*mMinFactor);

        mWarnButton->setTextRect(QRect(0*mMinFactor,26*mMinFactor,mWarnButton->width(),35*mMinFactor));
        mWarnButton->setTextFont(font,QColor(Qt::white));

        font.setFamily("Microsoft YaHei");
        font.setBold(false);
        int fontSize = (int)(25*mMinFactor);
        font.setPixelSize(fontSize);

        mMessageLabel->setFont(font);
        mMessageLabel->setGeometry(0*mMinFactor,61*mMinFactor,this->width(),40*mMinFactor);

        font.setPixelSize((int)(20*mMinFactor));

        mExtraColoredTextLabel->setFont(font);
        mExtraColoredTextLabel->setGeometry(0*mMinFactor,mMessageLabel->pos().y() + mMessageLabel->height() + 26*mMinFactor,this->width(),25*mMinFactor);
    }
}

bool EMessageBox::question(const QString& text,const QString& okButtonText,const QString& cancelButtonText,QWidget *parent)
{
    static EMessageBox *box =  nullptr;
    if(!box)
    {
        box = new EMessageBox(parent);
    }

    box->setButtonNormal(true);
    box->setConfirmButtonText(okButtonText);
    box->setCancelButtonText(cancelButtonText);
    box->setMessageText(text);
    box->setMessageType(MessageBoxType_Question);
    return box->exec() == QDialog::Accepted;
}

bool EMessageBox::question2(const QString& text,const QString& okButtonText,const QString& cancelButtonText,QWidget *parent)
{
    static EMessageBox *box =  nullptr;
    if(!box)
    {
        box = new EMessageBox(parent);
    }
    box->setButtonNormal(false);
    box->setConfirmButtonText(okButtonText);
    box->setCancelButtonText(cancelButtonText);
    box->setMessageText(text);
    box->setMessageType(MessageBoxType_Question2);
    return box->exec() == QDialog::Accepted;
}

bool EMessageBox::countdown(const QString& text,const QString& okButtonText,const QString& cancelButtonText,int seconds,QWidget *parent)
{
    static EMessageBox *box =  nullptr;
    if(!box)
    {
        box = new EMessageBox(parent);
    }
    box->setButtonNormal(true);
    box->setConfirmButtonText(okButtonText);
    box->setCancelButtonText(cancelButtonText);
    box->setMessageText(text);
    box->setMessageType(MessageBoxType_Countdown);
    box->setCountdownTime(seconds);
    return box->exec() == QDialog::Accepted;
}

bool EMessageBox::warning(const QString& text,const QString& okButtonText,QWidget *parent)
{
    static EMessageBox *box =  nullptr;
    if(!box)
    {
        box = new EMessageBox(parent);
    }
    box->setButtonNormal(true);
    box->setWarnButtonText(okButtonText);
    box->setMessageText(text);
    box->setMessageType(MessageBoxType_Confirm);
    return box->exec() == QDialog::Accepted;
}

bool EMessageBox::warning2(const QString &text, const QString &extraColoredText, const QColor &color, const QString &okButtonText, QWidget *parent)
{
    static EMessageBox *box =  nullptr;
    if(!box)
    {
        box = new EMessageBox(parent);
    }
    box->setButtonNormal(true);
    box->setWarnButtonText(okButtonText);
    box->setMessageText(text);
    box->setExtraColoredText(extraColoredText);
    box->setExtraColoredTextColor(color);
    box->setMessageType(MessageBoxType_ExtraColorText);
    return box->exec() == QDialog::Accepted;
}

void EMessageBox::info(const QString& text,const QString& imgPath,int count,const QString& okButtonText,QWidget *parent)
{
    static EMessageBox *box =  nullptr;
    if(!box)
    {
        box = new EMessageBox(parent);
    }
    box->setButtonNormal(true);
    box->setWarnButtonText(okButtonText);
    box->setMessageText(text);
    box->setAwardCount(count);
    box->setImagePath(imgPath);
    box->setMessageType(MessageBoxType_Info);
    box->exec();
}

void EMessageBox::setMessageType(MessageBoxType type)
{
    mType = type;
    if(mType == MessageBoxType_Confirm)
    {
        setBaseSize(584,356);
        mConfirmButton->hide();
        mCancelButton->hide();
        mCloseButton->show();
        mWarnButton->show();
        mAwardCountLabel->hide();
        mExtraColoredTextLabel->hide();
    }
    else if(mType == MessageBoxType_Question || mType == MessageBoxType_Question2)
    {
        setBaseSize(584,356);
        mConfirmButton->show();
        mCloseButton->show();
        mCancelButton->show();
        mWarnButton->hide();
        mAwardCountLabel->hide();
        mExtraColoredTextLabel->hide();
    }
    else if(mType == MessageBoxType_Info)
    {
        setBaseSize(584,316);
        mConfirmButton->hide();
        mCancelButton->hide();
        mWarnButton->show();
        mCloseButton->hide();
        mAwardCountLabel->show();
        mExtraColoredTextLabel->hide();
    }
    else if(mType == MessageBoxType_Countdown)
    {
        setBaseSize(584,356);
        mConfirmButton->show();
        mCloseButton->show();
        mCancelButton->show();
        mWarnButton->hide();
        mAwardCountLabel->show();
        mExtraColoredTextLabel->hide();

        mTimer = new QTimer(this);
        connect(mTimer,SIGNAL(timeout()),this,SLOT(slot_updateCountdown()));
    }
    else if(mType == MessageBoxType_ExtraColorText)
    {
        setBaseSize(584,356);
        mConfirmButton->hide();
        mCancelButton->hide();
        mCloseButton->show();
        mWarnButton->show();
        mAwardCountLabel->hide();
        mExtraColoredTextLabel->show();
    }
}

void EMessageBox::setMessageText(QString text)
{
    mMessageText = text;
    QString text8 = tr("<html><head/><body><p style='line-height:30px'> 中国在在在 </p></body></html>");
    mMessageLabel->setText(mMessageText);
}

void EMessageBox::setExtraColoredText(QString extraColoredText)
{
    mExtraColoredText = extraColoredText;
    mExtraColoredTextLabel->setText(mExtraColoredText);
}

void EMessageBox::setExtraColoredTextColor(QColor color)
{
    mExtraColoredTextLabel->setObjectName("mExtraColoredTextLabel");
    mExtraColoredTextLabel->setStyleSheet(QString("QLabel#mExtraColoredTextLabel{color:rgba(%1,%2,%3,%4);}")
                                          .arg(color.red())
                                          .arg(color.green())
                                          .arg(color.blue())
                                          .arg(color.alpha())
                                          );
}

void EMessageBox::setConfirmButtonText(QString text)
{
    mConfirmButton->setButtonText(text);
}

void EMessageBox::setCancelButtonText(QString text)
{
    mCancelButton->setButtonText(text);
}

void EMessageBox::setWarnButtonText(QString text)
{
    mWarnButton->setButtonText(text);
}

void EMessageBox::setCountdownTime(int seconds)
{
    mCountdownSeconds = seconds;

    if(mType == MessageBoxType_Countdown)
    {
        mTimer->start(1000);
        mAwardCountLabel->setText(QString::number(mCountdownSeconds));
    }
}

void EMessageBox::setBkgColor(QColor bkgColor)
{
    mBkgColor = bkgColor;
}

void EMessageBox::setAwardCount(int count)
{
    QString awardStr = QStringLiteral("x ");
    awardStr.append(QString::number(count));
    mAwardCountLabel->setText(awardStr);
}

void EMessageBox::setImagePath(QString path)
{
    mAwardPixmap.load(path);
}

void EMessageBox::setButtonNormal(bool bNormal)
{
    if(bNormal == true)
    {
        mConfirmButton->setChartlet(":/images/button/orange_Buttons244x99_Normal.png"
                           ,":/images/button/orange_Buttons244x99_Pressing.png"
                           ,":/images/button/orange_Buttons244x99_Pressing.png");

        mCancelButton->setChartlet(":/images/button/gray_Buttons244x99_Normal.png"
                               ,":/images/button/gray_Buttons244x99_Pressing.png"
                               ,":/images/button/gray_Buttons244x99_Pressing.png");
    }
    else
    {
        mConfirmButton->setChartlet(":/images/button/gray_Buttons244x99_Normal.png"
                           ,":/images/button/gray_Buttons244x99_Pressing.png"
                           ,":/images/button/gray_Buttons244x99_Pressing.png");

        mCancelButton->setChartlet(":/images/button/orange_Buttons244x99_Normal.png"
                               ,":/images/button/orange_Buttons244x99_Pressing.png"
                               ,":/images/button/orange_Buttons244x99_Pressing.png");
    }

    mWarnButton->setChartlet(":/images/button/orange_Buttons351x97_Normal.png"
                            ,":/images/button/orange_Buttons351x97_Pressing.png"
                            ,":/images/button/orange_Buttons351x97_Pressing.png");
}

void EMessageBox::slot_updateCountdown()
{
    if(mType == MessageBoxType_Countdown)
    {
        mCountdownSeconds--;
        mAwardCountLabel->setText(QString::number(mCountdownSeconds));

        if(mCountdownSeconds <= 0)
        {
            slot_confirmButtonClicked();
        }
    }
}

void EMessageBox::slot_confirmButtonClicked()
{
    if(mType == MessageBoxType_Countdown)
    {
        mTimer->stop();
    }

    this->accept();
}

void EMessageBox::slot_cancelButtonClicked()
{
    if(mType == MessageBoxType_Countdown)
    {
        mTimer->stop();
    }

    this->reject();
}
