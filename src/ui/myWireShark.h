/*
 * @Author: Kai Wang
 * @Date: 2019-12-07 11:21:21
 * @LastEditTime : 2019-12-24 01:14:36
 * @LastEditors  : Kai Wang
 * @Description: 本文件定义了主界面类
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

/**
 * @description: 主程序类
 */
class myWireShark : public QMainWindow
{
    Q_OBJECT

public:
    myWireShark(QWidget *parent = Q_NULLPTR);
    ~myWireShark();
private slots:
    void updateList();					// 更新包列表
    void upDateBar();					// 更新进度条
    void startCapture();				// 开始捕获包
    void pauseCapture();				// 暂停捕获包
    void updateDetailsInfo();			// 更新详细信息
    void openSaveWindow();				// 打开保存窗口
    void openReadWindow();				// 打开读取窗口
    void openErrorWindow();				// 打开错误窗口
    void changeDev();					// 切换网卡
    void setSearch();					// 设置筛选
private:
    Ui::myWireSharkClass ui;
    packetCapturer mPacketCapturer;		// 包捕获器
    packetAnalyst mPacketAnalyst;		// 包分析器
    packetManger mPacketManger;			// 包管理器
    FileManger mFileManger;				// 文件管理器
    QTimer timer;						// 计时器
    int currentDev;						// 当前设备序号
    string searchOption;				// 搜索字符串
    bool needClear;						// 是否需要清理标志
};
