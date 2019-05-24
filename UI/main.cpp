#include <qapplication.h>
#include "EApplication.h"

int main(int argc, char *argv[])
{
	EApplication a(argc, argv);
	return a.exec();
}
