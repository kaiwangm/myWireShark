/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 01:53:00
 * @LastEditTime : 2019-12-24 01:25:05
 * @LastEditors  : Kai Wang
 * @Description: 定义了数据包捕获类
 * @FilePath: \src\packet\packetCapture.h
 */

#pragma once
#include <iostream>
#include <functional>
#include <QThread>
#include <QListWidget>
#include <QStringList>
#include <sstream>
#include <ctime>
#include <pcap.h>
#include <vector>
#include <string>
#include <map>
#include <controller/packetManger.h>
#include "Frame.h"

using namespace std;
using namespace networkPacket;

/**
*@description: 包捕获类，可开启线程进行包捕获
*/
class packetCapturer : public QThread
{
    Q_OBJECT
private:
    /**
     * @description: 线程运行函数
     */
    void run() override;
    /**
    * @description: 包捕获函数
    */
    void packet_handler(const struct pcap_pkthdr *header, const u_char *pkt_data);
    pcap_t *handler;        // 当前网卡指针
    packetManger *manger;   // 包管理器指针
    int ErrorFlag;          // 错误信号

public:
    packetCapturer(packetManger *man);
    vector<pcap_if_t *> networkDevice;  // 网卡指针数组

    /**
     * @description: 设置当前网卡
     * @param int i 网卡序号
     */
    void setHandle(int i);
    int getInitFlag();
};


