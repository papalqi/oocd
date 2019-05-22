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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UIClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QListWidget *listWidget;
    QLineEdit *lineEdit;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *UIClass)
    {
        if (UIClass->objectName().isEmpty())
            UIClass->setObjectName(QString::fromUtf8("UIClass"));
        UIClass->resize(1123, 879);
        centralWidget = new QWidget(UIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(1014, 10, 101, 23));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(0, 11, 181, 831));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(210, 810, 551, 20));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(215, 620, 541, 191));
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
        pushButton->setText(QApplication::translate("UIClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UIClass: public Ui_UIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_H
