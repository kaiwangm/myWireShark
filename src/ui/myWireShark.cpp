#include "myWireShark.h"
#include <QColor>
myWireShark::myWireShark(QWidget* parent)
	: QMainWindow(parent),
	timer(this),
	mPacketCapturer(&mPacketManger),
	mPacketAnalyst(&mPacketManger),
	mFileManger(&mPacketManger),
	currentDev(0)
{
	ui.setupUi(this);



	if (mPacketCapturer.getInitFlag() == 1)
	{
		errorWindow err;
		err.exec();
		exit(1);
	}
	else if (mPacketCapturer.getInitFlag() == 0)
	{
		devChoice choice(&mPacketCapturer);
		choice.exec();
		currentDev = choice.getCurrentDev();
		mPacketCapturer.setHandle(currentDev);
		ui.statusBar->showMessage((tr("Current network card: ") + QString(mPacketCapturer.networkDevice[currentDev]->description)), 5000);
	}


	mPacketCapturer.start();
	mPacketAnalyst.start();
	ui.actionStart->setEnabled(false);

	QTimer* m_timer = new QTimer;
	m_timer->start(20);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateList()));
	connect(&mPacketManger, SIGNAL(sigUpdate()), this, SLOT(upDateBar()));

	QTreeWidget* tre = ui.treeWidget_dev;

	for (int i = 0; i < mPacketCapturer.networkDevice.size(); ++i)
	{
		QStringList infoList;

		infoList.append(QString(mPacketCapturer.networkDevice[i]->name));
		infoList.append(QString(mPacketCapturer.networkDevice[i]->description));
		QTreeWidgetItem* item = new QTreeWidgetItem(infoList);

		if (i == currentDev)
		{
			item->setBackgroundColor(0, QColor("gray"));
			item->setBackgroundColor(1, QColor("gray"));
		}


		tre->addTopLevelItem(item);
	}
}

void myWireShark::updateList()
{

	QTreeWidget* tre = ui.treeWidget;

	//cout << mPacketManger.unresolveSize() << endl;
	if (ui.lineEdit->text() != "")
	{
		return;
	}
	if (ui.lineEdit->text() == "" && needClear)
	{
		tre->clear();
		needClear = false;
	}
	for (int i = tre->topLevelItemCount(); i < mPacketManger.completionSize(); ++i)
	{
		QStringList infoList;
		infoList.append(QString::number(i));
		infoList.append(mPacketManger[i].captureInformation["time"].toString());
		infoList.append(QString::number(mPacketManger[i].captureInformation["length"].toInt()));
		if (mPacketManger[i].IPLayerInformation["source"].toString() == "")
		{
			infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["source"].toString());
			infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["destination"].toString());
		}
		else
		{
			infoList.append(mPacketManger[i].IPLayerInformation["source"].toString());
			infoList.append(mPacketManger[i].IPLayerInformation["destination"].toString());
		}
		infoList.append(QString(mPacketManger[i].portocol.c_str()));
		infoList.append(QString(mPacketManger[i].extractInfo.c_str()));
		QTreeWidgetItem* item = new QTreeWidgetItem(infoList);

		if (mPacketManger[i].portocol == "http")
		{
			item->setBackgroundColor(0, QColor("#f8b195"));
		}
		if (mPacketManger[i].portocol == "udp")
		{
			item->setBackgroundColor(0, QColor("#8bbabb"));
		}
		if (mPacketManger[i].portocol == "tcp")
		{
			item->setBackgroundColor(0, QColor("#6c7b95"));
		}
		if (mPacketManger[i].portocol == "ipv4")
		{
			item->setBackgroundColor(0, QColor("#eb8242"));
		}
		if (mPacketManger[i].portocol == "ipv6")
		{
			item->setBackgroundColor(0, QColor("#f6da63"));
		}
		if (mPacketManger[i].portocol == "dns")
		{
			item->setBackgroundColor(0, QColor("#347474"));
		}
		if (mPacketManger[i].portocol == "Ethernet II")
		{
			item->setBackgroundColor(0, QColor("#fcf9ea"));
		}
		tre->addTopLevelItem(item);
	}

}

myWireShark::~myWireShark()
{
	if (mPacketCapturer.isRunning())
	{
		mPacketCapturer.terminate();
	}
	if (mPacketAnalyst.isRunning())
	{
		mPacketAnalyst.terminate();
	}
}

void myWireShark::upDateBar()
{
	int numPkg = mPacketManger.unresolveSize();
	if (numPkg <= 30000)
	{
		ui.progressBar->setValue(numPkg);
	}
}

void myWireShark::startCapture()
{
	if (!mPacketCapturer.isRunning())
	{
		mPacketCapturer.start();
	}
	if (!mPacketAnalyst.isRunning())
	{
		mPacketAnalyst.start();
	}
	ui.actionPause->setEnabled(true);
	ui.actionStart->setEnabled(false);
}

void myWireShark::pauseCapture()
{
	if (mPacketCapturer.isRunning())
	{
		mPacketCapturer.terminate();
	}
	ui.actionPause->setEnabled(false);
	ui.actionStart->setEnabled(true);
}

void myWireShark::updateDetailsInfo()
{
	int index = ui.treeWidget->currentItem()->data(0, 0).toInt();
	Packet currentPacket = mPacketManger[index];
	auto tre = ui.treeWidget_det;
	tre->clear();

	QTreeWidgetItem* FrameLayer = new QTreeWidgetItem();
	FrameLayer->setText(0, "Frame");

	QVariantMap Framemp = currentPacket.captureInformation.toVariantMap();
	QVariantMap::const_iterator i = Framemp.constBegin();
	while (i != Framemp.constEnd()) {
		QTreeWidgetItem* newChild = new QTreeWidgetItem();
		newChild->setText(0, i.key() + QString(":") + i.value().toString());
		FrameLayer->addChild(newChild);
		++i;
	}
	tre->addTopLevelItem(FrameLayer);

	QTreeWidgetItem* LinkLayer = new QTreeWidgetItem();
	LinkLayer->setText(0, "Link layer");

	QVariantMap Linkmp = currentPacket.linkLayerInformation.toVariantMap();
	i = Linkmp.constBegin();
	while (i != Linkmp.constEnd()) {
		QTreeWidgetItem* newChild = new QTreeWidgetItem();
		newChild->setText(0, i.key() + QString(":") + i.value().toString());
		LinkLayer->addChild(newChild);
		++i;
	}
	tre->addTopLevelItem(LinkLayer);


	QTreeWidgetItem* IpLayer = new QTreeWidgetItem();
	IpLayer->setText(0, "IP layer");

	QVariantMap IPmp = currentPacket.IPLayerInformation.toVariantMap();
	i = IPmp.constBegin();
	while (i != IPmp.constEnd()) {
		QTreeWidgetItem* newChild = new QTreeWidgetItem();
		newChild->setText(0, i.key() + QString(":") + i.value().toString());
		IpLayer->addChild(newChild);
		++i;
	}
	tre->addTopLevelItem(IpLayer);

	QTreeWidgetItem* TransportLayer = new QTreeWidgetItem();
	TransportLayer->setText(0, "Transport layer");

	QVariantMap Transportmp = currentPacket.transportLayerInformation.toVariantMap();
	i = Transportmp.constBegin();
	while (i != Transportmp.constEnd()) {
		QTreeWidgetItem* newChild = new QTreeWidgetItem();
		newChild->setText(0, i.key() + QString(":") + i.value().toString());
		TransportLayer->addChild(newChild);
		++i;
	}
	tre->addTopLevelItem(TransportLayer);

	QTreeWidgetItem* ApplicationLayer = new QTreeWidgetItem();
	ApplicationLayer->setText(0, "Application layer");

	QVariantMap Applicationmp = currentPacket.applicationLayerInformation.toVariantMap();
	i = Applicationmp.constBegin();
	while (i != Applicationmp.constEnd()) {
		QTreeWidgetItem* newChild = new QTreeWidgetItem();
		newChild->setText(0, i.value().toString());
		ApplicationLayer->addChild(newChild);
		++i;
	}
	tre->addTopLevelItem(ApplicationLayer);

	ui.textBrowser->setText(currentPacket.org_data.c_str());
	ui.textBrowser_2->setText(currentPacket.ascii_data.c_str());
}

void myWireShark::openSaveWindow()
{
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Save"));

	QString fileName;
	fileName = fileDialog->getSaveFileName();

	mFileManger.save(fileName);

	delete fileDialog;

}

void myWireShark::openReadWindow()
{
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open"));

	QString fileName;
	fileName = fileDialog->getOpenFileName();

	mFileManger.open(fileName);

	delete fileDialog;
}

void myWireShark::openErrorWindow()
{
	errorWindow* win = new errorWindow();
	win->exec();
}

void myWireShark::changeDev()
{

	mPacketCapturer.terminate();
	QTreeWidget* tre = ui.treeWidget_dev;
	currentDev = ui.treeWidget_dev->currentIndex().row();
	mPacketCapturer.setHandle(currentDev);
	tre->clear();
	for (int i = 0; i < mPacketCapturer.networkDevice.size(); ++i)
	{
		QStringList infoList;

		infoList.append(QString(mPacketCapturer.networkDevice[i]->name));
		infoList.append(QString(mPacketCapturer.networkDevice[i]->description));
		QTreeWidgetItem* item = new QTreeWidgetItem(infoList);

		if (i == currentDev)
		{
			item->setBackgroundColor(0, QColor("gray"));
			item->setBackgroundColor(1, QColor("gray"));
		}


		tre->addTopLevelItem(item);
	}
	mPacketCapturer.start();
	ui.statusBar->showMessage((tr("Current network card: ") + QString(mPacketCapturer.networkDevice[currentDev]->description)), 5000);
}

void myWireShark::setSearch()
{
	searchOption = ui.lineEdit->text().toStdString();
	cout << searchOption << endl;

	QTreeWidget* tre = ui.treeWidget;

	tre->clear();
	for (int i = tre->topLevelItemCount(); i < mPacketManger.completionSize(); ++i)
	{
		if (mPacketManger[i].portocol != searchOption)
		{
			continue;
		}
		QStringList infoList;
		infoList.append(QString::number(i));
		infoList.append(mPacketManger[i].captureInformation["time"].toString());
		infoList.append(QString::number(mPacketManger[i].captureInformation["length"].toInt()));
		if (mPacketManger[i].IPLayerInformation["source"].toString() == "")
		{
			infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["source"].toString());
			infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["destination"].toString());
		}
		else
		{
			infoList.append(mPacketManger[i].IPLayerInformation["source"].toString());
			infoList.append(mPacketManger[i].IPLayerInformation["destination"].toString());
		}

		infoList.append(QString(mPacketManger[i].portocol.c_str()));
		infoList.append(QString(mPacketManger[i].extractInfo.c_str()));
		QTreeWidgetItem* item = new QTreeWidgetItem(infoList);
		if (mPacketManger[i].portocol == "http")
		{
			item->setBackgroundColor(0, QColor("#f8b195"));
		}
		if (mPacketManger[i].portocol == "udp")
		{
			item->setBackgroundColor(0, QColor("#8bbabb"));
		}
		if (mPacketManger[i].portocol == "tcp")
		{
			item->setBackgroundColor(0, QColor("#6c7b95"));
		}
		if (mPacketManger[i].portocol == "ipv4")
		{
			item->setBackgroundColor(0, QColor("#eb8242"));
		}
		if (mPacketManger[i].portocol == "ipv6")
		{
			item->setBackgroundColor(0, QColor("#f6da63"));
		}
		if (mPacketManger[i].portocol == "dns")
		{
			item->setBackgroundColor(0, QColor("#347474"));
		}
		if (mPacketManger[i].portocol == "Ethernet II")
		{
			item->setBackgroundColor(0, QColor("#fcf9ea"));
		}
		tre->addTopLevelItem(item);
	}

	needClear = true;
}

