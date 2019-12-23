/*
 * @Author: your name
 * @Date: 2019-12-07 11:21:21
 * @LastEditTime : 2019-12-23 23:26:01
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \src\ui\myWireShark.h
 */

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
#include <packet/packetCapturer.h>
#include <packet/packetAnalyst.h>
#include <controller/packetManger.h>
#include <controller/FileManger.h>
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
	void upDateBar();
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
