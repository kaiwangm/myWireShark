#include "myWireShark.h"

myWireShark::myWireShark(QWidget* parent)
	: QMainWindow(parent),
	timer(this),
	mPacketCapturer(&mPacketManger),
	mPacketAnalyst(&mPacketManger),
	mFileManger(&mPacketManger)
{
	ui.setupUi(this);
	//mPacketCapturer.start();
	mPacketAnalyst.start();
	
	timer.setInterval(500);
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateList()));
	timer.start();

	QTreeWidget* tre = ui.treeWidget_dev;

	for (int i = 0; i < mPacketCapturer.networkDevice.size(); ++i)
	{
		QStringList infoList;

		infoList.append(QString(mPacketCapturer.networkDevice[i]->name));
		infoList.append(QString(mPacketCapturer.networkDevice[i]->description));
		tre->addTopLevelItem(new QTreeWidgetItem(infoList));
	}
}

void myWireShark::updateList()
{

	QTreeWidget* tre = ui.treeWidget;
	
	//tre->clear();
	for(int i=tre->topLevelItemCount();i<mPacketManger.completionSize();++i)
	{
		QStringList infoList;
		infoList.append(QString::number(i));
		infoList.append(mPacketManger[i].captureInformation["time"].toString());
		infoList.append(QString::number(mPacketManger[i].captureInformation["length"].toInt()));
		infoList.append(mPacketManger[i].IPLayerInformation["source"].toString());
		infoList.append(mPacketManger[i].IPLayerInformation["destination"].toString());
		infoList.append(QString(mPacketManger[i].portocol.c_str()));
		infoList.append(QString(mPacketManger[i].extractInfo.c_str()));
		QTreeWidgetItem *item = new QTreeWidgetItem(infoList);

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
}

void myWireShark::pauseCapture()
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

void myWireShark::updateDetailsInfo()
{
	int index = ui.treeWidget->currentIndex().row();
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
	LinkLayer->setText(0,"Link layer");

	QVariantMap Linkmp = currentPacket.linkLayerInformation.toVariantMap();
	i = Linkmp.constBegin();
	while (i != Linkmp.constEnd()) {
		QTreeWidgetItem* newChild = new QTreeWidgetItem();
		newChild->setText(0,i.key() + QString(":") + i.value().toString());
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

