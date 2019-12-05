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

}

void myWireShark::updateList()
{
	cout << ui.listWidget->currentRow() << endl;
	cout << ui.listWidget->count() - 1 << endl;
	bool currisend = (ui.listWidget->currentRow() == (ui.listWidget->count() - 1));

	ui.listWidget->clear();
	QStringList items;
	for (string &i:aa.vs)
	{
		items << QString::fromStdString(i);
	}
	
	ui.listWidget->addItems(items);
	if(currisend)
		ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
}
