#include <QApplication>
#include "mainwindow.h"

/* runs the application */

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
