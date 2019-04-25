#include "UI.h"

#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UI w;
	;
	//EnginMain((HWND)w.Getui().openGLWidget->winId());
	EnginMain((HWND)w.winId());
	//w.render();
	w.show();

	return a.exec();
}
