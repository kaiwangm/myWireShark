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
#include "Frame.h"
#include "packetManger.h"
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
	
	
public:
	packetCapturer(packetManger* man);
	vector<pcap_if_t*> networkDevice;

};


