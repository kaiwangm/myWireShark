/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 13:03:41
 * @LastEditTime : 2019-12-24 01:24:28
 * @LastEditors  : JinHua Xv
 * @Description: 本文件定义了错误窗口类
 * @FilePath: \src\ui\errorWindow.h
 */

#pragma once
#include <QDialog>
#include "ui_errorWindow.h"

/**
 * @description: 错误窗口类
 */
class errorWindow : public QDialog
{
    Q_OBJECT
public:
    errorWindow(QWidget *parent = Q_NULLPTR);
    ~errorWindow();

private:
    Ui::errorWindow ui;
};
