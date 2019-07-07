#include <QtWidgets/QApplication>
#include "FileSystem_Q.h"
#include "stdafx.h"
int main(int argc, char *argv[])
{
	//Format();
	inti_main();
	QApplication a(argc, argv);
	FileSystem_Q w;
	w.show();
	return a.exec();
}
