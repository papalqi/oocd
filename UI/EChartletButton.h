/// \brief 贴图按钮 用来实现复杂阴影效果的按钮
#ifndef ECHARTLETBUTTON_H
#define ECHARTLETBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QFont>
class EChartletButton : public QPushButton
{
    Q_OBJECT
public:
    EChartletButton(QWidget *parent = nullptr);
    void setText(const QString text, const QRect rect);
    void setButtonText(const QString buttonText);
    void setTextRect(const QRect textRect);

    void setChartlet(const QString normalPixmapPath,const QString pressedPixmapPath,const QString disablePixmapPath);
    void setTextFont(const QFont textFont,QColor fontColor);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QRect mTextRect;
    QString mButtonText;
    QFont mTextFont;
    QColor mFontColor;
};

#endif // ECHARTLETBUTTON_H
