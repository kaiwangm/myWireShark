/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 12:26:19
 * @LastEditTime : 2019-12-24 01:06:58
 * @LastEditors  : Kai Wang
 * @Description: 本文件定义了网卡选择界面类
 * @FilePath: \src\ui\devChoice.h
 */

#pragma once
#include <QWidget>
#include <QDialog>
#include <QTreeWidget>
#include <packet/packetCapturer.h>
#include "ui_devChoice.h"

/**
 * @description: 网卡选择界面类
 */
class devChoice : public QDialog
{
    Q_OBJECT
public:
    devChoice(packetCapturer *pac);
    ~devChoice();

    /**
    * @description: 获取当前选择的网卡
    * @return: int 当前选择的网卡编号
    */
    int getCurrentDev();

private slots:

    /**
     * @description: 设置当前网卡序号
     */
    void setCurrentDev();

private:
    int currentDev;      // 当前网卡序号
    Ui::devChoice ui;
};
