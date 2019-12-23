/*
 * @Author: your name
 * @Date: 2019-12-21 01:51:59
 * @LastEditTime: 2019-12-23 23:25:41
 * @LastEditors: your name
 * @Description: In User Settings Edit
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
class packetAnalyst : public QThread
{
	Q_OBJECT

private:
	packetManger* manger;
	void run() override;
	Packet analysisPacket(Frame& orgin);
public:
	packetAnalyst(packetManger* man);
	~packetAnalyst();
};
