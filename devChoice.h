#pragma once

#include <QWidget>
#include <QDialog>
#include <QTreeWidget>
#include "packetCapturer.h"
#include "ui_devChoice.h"

class devChoice : public QDialog
{
	Q_OBJECT

public:
	devChoice(packetCapturer* pac);
	~devChoice();
	int getCurrentDev();
private slots:
	void setCurrentDev();

private:
	int currentDev;
	Ui::devChoice ui;
};
