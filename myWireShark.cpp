#include "myWireShark.h"

myWireShark::myWireShark(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	aa.start();

	timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateList()));
	timer->start();

	QTableWidget* tableWidget = ui.tableWidget;
	tableWidget->setRowCount(100);
}

void myWireShark::updateList()
{

	QTableWidget* tableWidget = ui.tableWidget;
	tableWidget->setRowCount(aa.vs.size());
	for(int i=0;i<aa.vs.size();++i)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString(aa.vs[i].time.c_str())));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString(aa.vs[i].src.c_str())));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString(aa.vs[i].dst.c_str())));
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString(aa.vs[i].por.c_str())));
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(aa.vs[i].length)));

	}

}
