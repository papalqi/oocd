#ifndef EMAINWINDOW_H
#define EMAINWINDOW_H

#include <QMainWindow>
class ECentralWidget;

class EMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static EMainWindow& Instance();
    static void Release();
    ~EMainWindow();
    void switchToPage(int index);
    void setECentralWidget(ECentralWidget *widget);
    ECentralWidget *centralWidget();
    void wait();
    void stopWait();
    void showMyMaximized();
    void enableMaskWidget(bool bEnable);
    int titleHeight();
signals:
    void moved();
    void moveSignal(QMoveEvent *event);
    void minimizeSignal();
    void mainWindowShowSignal();
    void mainWindowHideSignal();
public slots:
    void showMinimizeSlot();
    void quitSlot();
protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void changeEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
private:
    explicit EMainWindow(QWidget *parent = 0);
    ECentralWidget *mpCentralWidget;
    static EMainWindow* sMainWindow;
};

#endif // EMAINWINDOW_H
