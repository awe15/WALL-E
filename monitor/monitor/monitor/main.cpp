#include "monitor.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	monitor w;
	w.show();
	return a.exec();
}
