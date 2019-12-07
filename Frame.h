#pragma once
#include <string>
#include <ctime>
#include <pcap.h>
#include <cstdint>
#include <cstring>
class Frame
{
public:
	struct Interface
	{
		std::string time;
		int length;
	} Interface;

	Frame(const struct pcap_pkthdr* header, const u_char* pck)
	{
		struct tm* ltime;
		time_t local_tv_sec;
		char timestr[16];
		local_tv_sec = header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

		Interface.time = timestr;
		Interface.length = header->len;
	}
};

class EthernetPacket :public Frame
{
public:
	u_char destination[6];
	u_char source[6];
	u_char type[2];

	int offset;
	EthernetPacket(const struct pcap_pkthdr* header, const u_char* pck)
		:Frame(header, pck)
	{
		memcpy(destination, pck, 6 * (sizeof u_char));
		memcpy(source, pck + 6, 6 * (sizeof u_char));
		memcpy(type, pck + 12, 2 * (sizeof u_char));

		offset = 14;
	}


};

typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;
class IpPacket :public EthernetPacket
{
public:
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
	IpPacket(const struct pcap_pkthdr* header, const u_char* pck)
		:EthernetPacket(header, pck)
	{
		const u_char* pckFirst = pck + offset;
		memcpy(&ver_ihl, pckFirst, 1 * (sizeof u_char));
		memcpy(&tos, pckFirst + 1, 1 * (sizeof u_char));
		memcpy(&tlen, pckFirst + 2, 2 * (sizeof u_char));
		memcpy(&identification, pckFirst + 4, 2 * (sizeof u_char));
		memcpy(&flags_fo, pckFirst + 6, 2 * (sizeof u_char));
		memcpy(&ttl, pckFirst + 8, 1 * (sizeof u_char));
		memcpy(&proto, pckFirst + 9, 1 * (sizeof u_char));
		memcpy(&crc, pckFirst + 10, 2 * (sizeof u_char));
		memcpy(&saddr, pckFirst + 14, 4 * (sizeof u_char));
		memcpy(&daddr, pckFirst + 18, 4 * (sizeof u_char));
		memcpy(&op_pad, pckFirst + 22, 4 * (sizeof u_char));

		offset += 26;
	}
};
