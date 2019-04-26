/********************************************************************************
** Form generated from reading UI file 'UI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_H
#define UI_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <renderwindows.h>

QT_BEGIN_NAMESPACE

class Ui_UIClass
{
public:
    QWidget *centralWidget;
    RenderWindows *graphicsView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *UIClass)
    {
        if (UIClass->objectName().isEmpty())
            UIClass->setObjectName(QString::fromUtf8("UIClass"));
        UIClass->resize(1123, 879);
        centralWidget = new QWidget(UIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        graphicsView = new RenderWindows(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 10, 800, 600));
        UIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(UIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1123, 23));
        UIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(UIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        UIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(UIClass);

        QMetaObject::connectSlotsByName(UIClass);
    } // setupUi

    void retranslateUi(QMainWindow *UIClass)
    {
        UIClass->setWindowTitle(QApplication::translate("UIClass", "UI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UIClass: public Ui_UIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_H
