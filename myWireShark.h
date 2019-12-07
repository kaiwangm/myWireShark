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
#include "packetCapturer.h"
#include "ui_myWireShark.h"

class myWireShark : public QMainWindow
{
	Q_OBJECT

public:
	myWireShark(QWidget *parent = Q_NULLPTR);

private slots:
	void updateList();

private:
	Ui::myWireSharkClass ui;
	packetCapturer aa;
	QTimer* timer;
};
