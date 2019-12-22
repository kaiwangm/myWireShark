#pragma once

#include <QtWidgets/QMainWindow>
#include <QListWidgetItem>
#include <QStringList>
#include <QListWidget>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QToolButton>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QFileDialog>
#include <QTextBrowser>
#include "packetCapturer.h"
#include "packetAnalyst.h"
#include "packetManger.h"
#include "FileManger.h"
#include "devChoice.h"
#include "errorWindow.h"
#include "ui_myWireShark.h"

class myWireShark : public QMainWindow
{
	Q_OBJECT

public:
	myWireShark(QWidget *parent = Q_NULLPTR);
	~myWireShark();
private slots:
	void updateList();
	void startCapture();
	void pauseCapture();
	void updateDetailsInfo();
	void openSaveWindow();
	void openReadWindow();
	void openErrorWindow();
	void changeDev();
	void setSearch();
private:
	Ui::myWireSharkClass ui;
	packetCapturer mPacketCapturer;
	packetAnalyst mPacketAnalyst;
	packetManger mPacketManger;
	FileManger mFileManger;
	QTimer timer;
	int currentDev;
	string searchOption;
	bool needClear;
};
