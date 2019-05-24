#ifndef EMESSAGEBOX_H
#define EMESSAGEBOX_H
#include <QDialog>

typedef enum _tag_messageboxtype
{
    MessageBoxType_Confirm = 0,//only one  "OK" button
    MessageBoxType_Question,
    MessageBoxType_Question2,//区别：消息关闭消息和cancel一致
    MessageBoxType_Info,
    MessageBoxType_Countdown, //倒计时弹窗
    MessageBoxType_ExtraColorText
}
MessageBoxType;

class QWidget;
class EChartletButton;
class QLabel;
class QPixmap;
class QTimer;

class EMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit EMessageBox(QWidget *parent = nullptr);
    static bool warning(const QString& text,const QString& okButtonText = "warning",QWidget *parent = nullptr);
    static bool warning2(const QString& text,const QString& extraColoredText,const QColor& color,const QString& okButtonText = "Ok",QWidget *parent = nullptr);
    static bool question(const QString& text,const QString& okButtonText = "confirm",const QString& cancelButtonText = "cancel",QWidget *parent = nullptr);
    static bool question2(const QString& text,const QString& okButtonText = "confirm",const QString& cancelButtonText = "cancel",QWidget *parent = nullptr);
    static void info(const QString& text,const QString& imgPath,int count = 0,const QString& okButtonText = "warning",QWidget *parent = nullptr);
    static bool countdown(const QString& text,const QString& okButtonText = "confirm",const QString& cancelButtonText = "cancel",int seconds = 5,QWidget *parent = nullptr);

    void setMessageType(MessageBoxType type);
    void setMessageText(QString text);
    void setExtraColoredText(QString extraColoredText);
    void setExtraColoredTextColor(QColor color);

    void setConfirmButtonText(QString text);
    void setCancelButtonText(QString text);
    void setWarnButtonText(QString text);

    void setCountdownTime(int seconds);
    void setBkgColor(QColor bkgColor);
    void setAwardCount(int count);
    void setImagePath(QString path);

    void setButtonNormal(bool bNormal);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void slot_confirmButtonClicked();
    void slot_cancelButtonClicked();
    void slot_updateCountdown();
private:
    MessageBoxType mType;//type
    QString mMessageText;
    QString mExtraColoredText;
    QPixmap mAwardPixmap;
    QColor mBkgColor;

    //button
    EChartletButton *mCancelButton;//取消
    EChartletButton *mConfirmButton;//确认
    EChartletButton *mWarnButton;//警告
    QPushButton *mCloseButton;//关闭

    QLabel *mMessageLabel;
    QLabel *mAwardCountLabel;
    QLabel *mExtraColoredTextLabel;

    //放缩比例
    float mWFactor;
    float mHFactor;
    float mMinFactor;

    //for countdown messagebox
    QTimer *mTimer;
    int mCountdownSeconds;
};

#endif // EMESSAGEBOX_H
