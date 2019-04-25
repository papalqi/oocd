#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UI.h"

class UI : public QMainWindow
{
	Q_OBJECT

public:
	UI(QWidget *parent = Q_NULLPTR);
	Ui::UIClass Getui() { return ui; };
private:
	Ui::UIClass ui;
};
