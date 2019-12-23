#include <QtWidgets/QApplication>
#include <ui/myWireShark.h>
#include <packet/packetCapturer.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	myWireShark w;
	w.show();
	return a.exec();
}
