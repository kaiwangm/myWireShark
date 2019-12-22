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
