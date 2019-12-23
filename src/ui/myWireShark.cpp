/*
 * @Author: Kai Wang
 * @Date: 2019-12-07 11:21:21
 * @LastEditTime : 2019-12-24 01:24:35
 * @LastEditors  : TinTin Zhuang
 * @Description: 本文件实现了主界面类
 * @FilePath: \src\ui\myWireShark.cpp
 */

#include "myWireShark.h"
#include <QColor>

myWireShark::myWireShark(QWidget *parent)
    : QMainWindow(parent),
      timer(this),
      mPacketCapturer(&mPacketManger),
      mPacketAnalyst(&mPacketManger),
      mFileManger(&mPacketManger),
      currentDev(0)
{
    ui.setupUi(this);

    if (mPacketCapturer.getInitFlag() == 1) {           // 若错误 打开错误窗口 并退出
        errorWindow err;
        err.exec();
        exit(1);
    } else if (mPacketCapturer.getInitFlag() == 0) {    // 若成功 打开网卡选择窗口 设置当前网卡
        devChoice choice(&mPacketCapturer);
        choice.exec();
        currentDev = choice.getCurrentDev();
        mPacketCapturer.setHandle(currentDev);
        ui.statusBar->showMessage((tr("Current network card: ") + QString(mPacketCapturer.networkDevice[currentDev]->description)), 5000);
    }


    mPacketCapturer.start();                            // 开启抓包线程
    mPacketAnalyst.start();                             // 开启分析线程
    ui.actionStart->setEnabled(false);

    QTimer *m_timer = new QTimer;
    m_timer->start(20);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateList()));
    connect(&mPacketManger, SIGNAL(sigUpdate()), this, SLOT(upDateBar()));

    QTreeWidget *tre = ui.treeWidget_dev;

    for (int i = 0; i < mPacketCapturer.networkDevice.size(); ++i) {
        QStringList infoList;

        infoList.append(QString(mPacketCapturer.networkDevice[i]->name));
        infoList.append(QString(mPacketCapturer.networkDevice[i]->description));
        QTreeWidgetItem *item = new QTreeWidgetItem(infoList);

        if (i == currentDev) {
            item->setBackgroundColor(0, QColor("gray"));    // 设置选中网卡为灰色
            item->setBackgroundColor(1, QColor("gray"));
        }


        tre->addTopLevelItem(item);
    }
}

void myWireShark::updateList()
{

    QTreeWidget *tre = ui.treeWidget;

    if (ui.lineEdit->text() != "") {
        return;
    }
    if (ui.lineEdit->text() == "" && needClear) {
        tre->clear();
        needClear = false;
    }
    for (int i = tre->topLevelItemCount(); i < mPacketManger.completionSize(); ++i) {   // 将信息展示到包列表
        QStringList infoList;
        infoList.append(QString::number(i));
        infoList.append(mPacketManger[i].captureInformation["time"].toString());
        infoList.append(QString::number(mPacketManger[i].captureInformation["length"].toInt()));
        if (mPacketManger[i].IPLayerInformation["source"].toString() == "") {
            infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["source"].toString());
            infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["destination"].toString());
        } else {
            infoList.append(mPacketManger[i].IPLayerInformation["source"].toString());
            infoList.append(mPacketManger[i].IPLayerInformation["destination"].toString());
        }
        infoList.append(QString(mPacketManger[i].portocol.c_str()));
        infoList.append(QString(mPacketManger[i].extractInfo.c_str()));
        QTreeWidgetItem *item = new QTreeWidgetItem(infoList);

        if (mPacketManger[i].portocol == "http") {                  // 设置不同协议颜色
            item->setBackgroundColor(0, QColor("#f8b195"));
        }
        if (mPacketManger[i].portocol == "udp") {
            item->setBackgroundColor(0, QColor("#8bbabb"));
        }
        if (mPacketManger[i].portocol == "tcp") {
            item->setBackgroundColor(0, QColor("#6c7b95"));
        }
        if (mPacketManger[i].portocol == "ipv4") {
            item->setBackgroundColor(0, QColor("#eb8242"));
        }
        if (mPacketManger[i].portocol == "ipv6") {
            item->setBackgroundColor(0, QColor("#f6da63"));
        }
        if (mPacketManger[i].portocol == "dns") {
            item->setBackgroundColor(0, QColor("#347474"));
        }
        if (mPacketManger[i].portocol == "Ethernet II") {
            item->setBackgroundColor(0, QColor("#fcf9ea"));
        }
        tre->addTopLevelItem(item);
    }

}

myWireShark::~myWireShark()
{
    if (mPacketCapturer.isRunning()) {
        mPacketCapturer.terminate();        // 关闭包捕获线程
    }
    if (mPacketAnalyst.isRunning()) {
        mPacketAnalyst.terminate();         // 关闭包分析线程
    }
}

void myWireShark::upDateBar()
{
    int numPkg = mPacketManger.unresolveSize();
    if (numPkg <= 30000) {
        ui.progressBar->setValue(numPkg);   // 设置进度条
    }
}

void myWireShark::startCapture()            // 开始抓包线程
{
    if (!mPacketCapturer.isRunning()) {
        mPacketCapturer.start();
    }
    if (!mPacketAnalyst.isRunning()) {
        mPacketAnalyst.start();
    }
    ui.actionPause->setEnabled(true);
    ui.actionStart->setEnabled(false);
}

void myWireShark::pauseCapture()            // 暂停抓包线程
{
    if (mPacketCapturer.isRunning()) {
        mPacketCapturer.terminate();
    }
    ui.actionPause->setEnabled(false);
    ui.actionStart->setEnabled(true);
}

void myWireShark::updateDetailsInfo()       // 更新详细信息
{
    int index = ui.treeWidget->currentItem()->data(0, 0).toInt();
    Packet currentPacket = mPacketManger[index];
    auto tre = ui.treeWidget_det;
    tre->clear();

    // 更新包基础信息
    QTreeWidgetItem *FrameLayer = new QTreeWidgetItem();
    FrameLayer->setText(0, "Frame");
    QVariantMap Framemp = currentPacket.captureInformation.toVariantMap();
    QVariantMap::const_iterator i = Framemp.constBegin();
    while (i != Framemp.constEnd()) {
        QTreeWidgetItem *newChild = new QTreeWidgetItem();
        newChild->setText(0, i.key() + QString(":") + i.value().toString());
        FrameLayer->addChild(newChild);
        ++i;
    }
    tre->addTopLevelItem(FrameLayer);

    // 更新链路层详细信息
    QTreeWidgetItem *LinkLayer = new QTreeWidgetItem();
    LinkLayer->setText(0, "Link layer");
    QVariantMap Linkmp = currentPacket.linkLayerInformation.toVariantMap();
    i = Linkmp.constBegin();
    while (i != Linkmp.constEnd()) {
        QTreeWidgetItem *newChild = new QTreeWidgetItem();
        newChild->setText(0, i.key() + QString(":") + i.value().toString());
        LinkLayer->addChild(newChild);
        ++i;
    }
    tre->addTopLevelItem(LinkLayer);

    // 更新网络层详细信息
    QTreeWidgetItem *IpLayer = new QTreeWidgetItem();
    IpLayer->setText(0, "IP layer");
    QVariantMap IPmp = currentPacket.IPLayerInformation.toVariantMap();
    i = IPmp.constBegin();
    while (i != IPmp.constEnd()) {
        QTreeWidgetItem *newChild = new QTreeWidgetItem();
        newChild->setText(0, i.key() + QString(":") + i.value().toString());
        IpLayer->addChild(newChild);
        ++i;
    }
    tre->addTopLevelItem(IpLayer);

    // 更新传输层详细信息
    QTreeWidgetItem *TransportLayer = new QTreeWidgetItem();
    TransportLayer->setText(0, "Transport layer");
    QVariantMap Transportmp = currentPacket.transportLayerInformation.toVariantMap();
    i = Transportmp.constBegin();
    while (i != Transportmp.constEnd()) {
        QTreeWidgetItem *newChild = new QTreeWidgetItem();
        newChild->setText(0, i.key() + QString(":") + i.value().toString());
        TransportLayer->addChild(newChild);
        ++i;
    }
    tre->addTopLevelItem(TransportLayer);

    // 更新应用层详细信息
    QTreeWidgetItem *ApplicationLayer = new QTreeWidgetItem();
    ApplicationLayer->setText(0, "Application layer");
    QVariantMap Applicationmp = currentPacket.applicationLayerInformation.toVariantMap();
    i = Applicationmp.constBegin();
    while (i != Applicationmp.constEnd()) {
        QTreeWidgetItem *newChild = new QTreeWidgetItem();
        newChild->setText(0, i.value().toString());
        ApplicationLayer->addChild(newChild);
        ++i;
    }
    tre->addTopLevelItem(ApplicationLayer);

    ui.textBrowser->setText(currentPacket.org_data.c_str());
    ui.textBrowser_2->setText(currentPacket.ascii_data.c_str());
}

void myWireShark::openSaveWindow()          // 打开保存窗口
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Save"));

    QString fileName;
    fileName = fileDialog->getSaveFileName(); // 获取文件名
    mFileManger.save(fileName);

    delete fileDialog;

}

void myWireShark::openReadWindow()          //打卡读取窗口
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open"));

    QString fileName;
    fileName = fileDialog->getOpenFileName(); // 获取文件名
    mFileManger.open(fileName);

    delete fileDialog;
}

void myWireShark::openErrorWindow()         // 打开错误窗口
{
    errorWindow win;
    win.exec();
}

void myWireShark::changeDev()               // 改变选中网卡
{

    mPacketCapturer.terminate();
    QTreeWidget *tre = ui.treeWidget_dev;
    currentDev = ui.treeWidget_dev->currentIndex().row();
    mPacketCapturer.setHandle(currentDev);

    // 更新网卡列表
    tre->clear();
    for (int i = 0; i < mPacketCapturer.networkDevice.size(); ++i) {
        QStringList infoList;

        infoList.append(QString(mPacketCapturer.networkDevice[i]->name));
        infoList.append(QString(mPacketCapturer.networkDevice[i]->description));
        QTreeWidgetItem *item = new QTreeWidgetItem(infoList);

        if (i == currentDev) {
            item->setBackgroundColor(0, QColor("gray")); // 设置选中网卡为灰色
            item->setBackgroundColor(1, QColor("gray"));
        }

        tre->addTopLevelItem(item);
    }
    mPacketCapturer.start();
    ui.statusBar->showMessage((tr("Current network card: ") + QString(mPacketCapturer.networkDevice[currentDev]->description)), 5000);
}

void myWireShark::setSearch()               // 设置筛选
{
    searchOption = ui.lineEdit->text().toStdString();
    cout << searchOption << endl;

    QTreeWidget *tre = ui.treeWidget;

    tre->clear();
    for (int i = tre->topLevelItemCount(); i < mPacketManger.completionSize(); ++i) {
        if (mPacketManger[i].portocol != searchOption) {  // 只展示协议符合的数据包
            continue;
        }
        QStringList infoList;
        infoList.append(QString::number(i));
        infoList.append(mPacketManger[i].captureInformation["time"].toString());
        infoList.append(QString::number(mPacketManger[i].captureInformation["length"].toInt()));
        if (mPacketManger[i].IPLayerInformation["source"].toString() == "") {
            infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["source"].toString());
            infoList.append(QString("[MAC] ") + mPacketManger[i].linkLayerInformation["destination"].toString());
        } else {
            infoList.append(mPacketManger[i].IPLayerInformation["source"].toString());
            infoList.append(mPacketManger[i].IPLayerInformation["destination"].toString());
        }

        infoList.append(QString(mPacketManger[i].portocol.c_str()));
        infoList.append(QString(mPacketManger[i].extractInfo.c_str()));
        QTreeWidgetItem *item = new QTreeWidgetItem(infoList);
        if (mPacketManger[i].portocol == "http") {
            item->setBackgroundColor(0, QColor("#f8b195"));
        }
        if (mPacketManger[i].portocol == "udp") {
            item->setBackgroundColor(0, QColor("#8bbabb"));
        }
        if (mPacketManger[i].portocol == "tcp") {
            item->setBackgroundColor(0, QColor("#6c7b95"));
        }
        if (mPacketManger[i].portocol == "ipv4") {
            item->setBackgroundColor(0, QColor("#eb8242"));
        }
        if (mPacketManger[i].portocol == "ipv6") {
            item->setBackgroundColor(0, QColor("#f6da63"));
        }
        if (mPacketManger[i].portocol == "dns") {
            item->setBackgroundColor(0, QColor("#347474"));
        }
        if (mPacketManger[i].portocol == "Ethernet II") {
            item->setBackgroundColor(0, QColor("#fcf9ea"));
        }
        tre->addTopLevelItem(item);
    }

    needClear = true;
}

