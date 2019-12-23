/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 13:03:41
 * @LastEditTime : 2019-12-24 01:10:32
 * @LastEditors  : TinTin Zhuang
 * @Description: 本文件实现了错误窗口类
 * @FilePath: \src\ui\errorWindow.cpp
 */

#include "errorWindow.h"

errorWindow::errorWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

errorWindow::~errorWindow()
{
}
