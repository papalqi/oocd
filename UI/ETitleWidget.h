#ifndef ETitleWidget_H
#define ETitleWidget_H

#include <QWidget>
#include <QResizeEvent>
#include <QColor>
#include <QPushButton>

class ETitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ETitleWidget(QWidget *parent = 0);
    ~ETitleWidget();
    void setBackgroundColor(const QColor &backgroundColor);

signals:
    void quitSignal();
    void minimumSignnal();
protected:
    void resizeEvent(QResizeEvent *rEvent);
    void paintEvent(QPaintEvent *pEvent);

private:
    void init();
    QPushButton *mMinButton;
    QPushButton *mQuitButton;
    QColor mBackgroundColor;
};

#endif // ETitleWidget_H
