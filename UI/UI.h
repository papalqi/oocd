#pragma once

#include <QtWidgets/QMainWindow>
#include "Commend.h"
#include "ui_UI.h"

class UI : public QMainWindow
{
	Q_OBJECT

public:
	UI(QWidget *parent = Q_NULLPTR);
	Ui::UIClass Getui() { return ui; };
	void SetLineEdited();
public slots:
	void CommendReture();
private:
	Ui::UIClass ui;
	fCommend *CommendControl;
};
