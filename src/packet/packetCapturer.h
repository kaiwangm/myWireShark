/*
 * @Author: your name
 * @Date: 2019-12-07 11:21:21
 * @LastEditTime : 2019-12-23 23:25:45
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \src\packet\packetCapturer.h
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

class packetCapturer : public QThread
{
	Q_OBJECT
private:
	void run() override;
	void packet_handler(const struct pcap_pkthdr* header, const u_char* pkt_data);
	map<int, string> portocolMap;
	pcap_t* handler;
	packetManger* manger;
	int ErrorFlag;
	
public:
	packetCapturer(packetManger* man);
	vector<pcap_if_t*> networkDevice;
	void setHandle(int i);
	int getInitFlag();
};


