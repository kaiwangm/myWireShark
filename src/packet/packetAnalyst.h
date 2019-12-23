/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 01:52:00
 * @LastEditTime : 2019-12-24 01:25:03
 * @LastEditors  : Kai Wang
 * @Description: 定义了数据包分析类
 * @FilePath: \src\packet\packetAnalyst.h
 */

#pragma once
#include <Qthread>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <controller/packetManger.h>

using namespace std;

/**
 * @description: 包分析器类，可创建线程并从包管理器中取出并分析包
 */
class packetAnalyst : public QThread
{
    Q_OBJECT

private:
    packetManger *manger;     // 指向包管理器的指针

    /**
     * @description: 线程运行函数
     */
    void run() override;

    /**
    * @description: 包分析函数
    * @param Frame &orgin 原始数据包
    * @return: Packet 分析完成的包
    */
    Packet analysisPacket(Frame &orgin);
public:
    packetAnalyst(packetManger *man);
    ~packetAnalyst();
};
