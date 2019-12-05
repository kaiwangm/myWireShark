#include "myWireShark.h"
#include <QtWidgets/QApplication>
#include "packetCapturer.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	myWireShark w;
	w.show();
	return a.exec();
}
