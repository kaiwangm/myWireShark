/*
 * @Author: your name
 * @Date: 2019-12-22 12:26:19
 * @LastEditTime : 2019-12-23 23:25:53
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \src\ui\devChoice.h
 */

#pragma once
#include <QWidget>
#include <QDialog>
#include <QTreeWidget>
#include <packet/packetCapturer.h>
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
