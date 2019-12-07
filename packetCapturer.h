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
using namespace std;


class packet;

class packetCapturer : public QThread
{
	Q_OBJECT
private:
	
	map<int, string> portocolMap;
	pcap_if_t* all_devs;
	char ErrorBuf[PCAP_ERRBUF_SIZE] = { 0 };
	pcap_t* handler;
	void run();
	
	void packet_handler(const struct pcap_pkthdr* header, const u_char* pkt_data);
public:
	vector<packet> vs;
	packetCapturer();

};

/* 4 bytes IP address */


/* IPv4 header */
typedef struct ip_header
{
	u_char    ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
	u_char    tos;            // Type of service 
	u_short tlen;            // Total length 
	u_short identification; // Identification
	u_short flags_fo;        // Flags (3 bits) + Fragment offset (13 bits)
	u_char    ttl;            // Time to live
	u_char    proto;            // Protocol
	u_short crc;            // Header checksum
	ip_address    saddr;        // Source address
	ip_address    daddr;        // Destination address
	u_int    op_pad;            // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;            // Source port
	u_short dport;            // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum
}udp_header;

class packet
{
public:
	string time;
	string src;
	string dst;
	string por;
	int length;
};