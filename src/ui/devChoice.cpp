#include "devChoice.h"


devChoice::devChoice(packetCapturer* pac):currentDev(0)
{
	ui.setupUi(this);

	QTreeWidget* tre = ui.treeWidget_dev_2;

	for (int i = 0; i < pac->networkDevice.size(); ++i)
	{
		QStringList infoList;

		infoList.append(QString(pac->networkDevice[i]->name));
		infoList.append(QString(pac->networkDevice[i]->description));
		tre->addTopLevelItem(new QTreeWidgetItem(infoList));
	}
}

void devChoice::setCurrentDev()
{
	currentDev = ui.treeWidget_dev_2->currentIndex().row();
	cout << currentDev << endl;
}

devChoice::~devChoice()
{
}

int devChoice::getCurrentDev()
{
	return currentDev;
}
