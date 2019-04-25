/********************************************************************************
** Form generated from reading UI file 'UI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_H
#define UI_UI_H
#include "stdafx.h"
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UIClass
{
public:
	QWidget *centralWidget;
	QOpenGLWidget *openGLWidget;
	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *UIClass)
	{
		if (UIClass->objectName().isEmpty())
			UIClass->setObjectName(QString::fromUtf8("UIClass"));
		UIClass->resize(625, 388);
		centralWidget = new QWidget(UIClass);
		centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
		openGLWidget = new QOpenGLWidget(centralWidget);
		openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
		openGLWidget->setGeometry(QRect(0, 0, 321, 331));
	

		UIClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(UIClass);
		menuBar->setObjectName(QString::fromUtf8("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 625, 23));
		UIClass->setMenuBar(menuBar);
		mainToolBar = new QToolBar(UIClass);
		mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
		UIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
		statusBar = new QStatusBar(UIClass);
		statusBar->setObjectName(QString::fromUtf8("statusBar"));
		UIClass->setStatusBar(statusBar);

		retranslateUi(UIClass);

		QMetaObject::connectSlotsByName(UIClass);
	} // setupUi

	void retranslateUi(QMainWindow *UIClass)
	{
		UIClass->setWindowTitle(QApplication::translate("UIClass", "UI", nullptr));
	} // retranslateUi

};

namespace Ui {
	class UIClass : public Ui_UIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_H
