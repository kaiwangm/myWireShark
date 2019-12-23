/*
 * @Author: your name
 * @Date: 2019-12-22 13:03:41
 * @LastEditTime: 2019-12-23 23:25:56
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \src\ui\errorWindow.h
 */

#pragma once
#include <QDialog>
#include "ui_errorWindow.h"

class errorWindow : public QDialog
{
	Q_OBJECT
public:
	errorWindow(QWidget *parent = Q_NULLPTR);
	~errorWindow();

private:
	Ui::errorWindow ui;
};
