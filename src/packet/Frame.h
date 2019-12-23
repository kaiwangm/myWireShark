/*
 * @Author: your name
 * @Date: 2019-12-07 20:40:32
 * @LastEditTime : 2019-12-23 23:25:36
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \src\packet\Frame.h
 */

#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <pcap.h>
#include <cstdint>
#include <cstring>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <sstream>
#include <iomanip>

using namespace std;
namespace tool
{
	string dec2hex(int i);
	u_char hex2dec(char a, char b);
	u_short bswap_16(const u_short& x);
	u_int bswap_32(const u_int& x);
	vector<string> split(const string& str, const string& pattern);
}

namespace networkPacket
{
	class Packet
	{
	public:
		std::string extractInfo;
		std::string org_data;
		std::string ascii_data;
		std::string portocol;
		QJsonObject captureInformation;
		QJsonObject linkLayerInformation;
		QJsonObject IPLayerInformation;
		QJsonObject transportLayerInformation;
		QJsonObject applicationLayerInformation;
	};

	class Frame
	{
	public:
		struct Interface
		{
			std::string time;
			int length;
		} Interface;
		std::vector<u_char> org_data;
		Frame(QString data, QString time, int length);
		Frame(const struct pcap_pkthdr* header, const u_char* pck);
		virtual QJsonObject toObject() const;
	};


	class EthernetPacket :public Frame
	{
	public:
		struct FrameHeader
		{
			u_char destination[6];
			u_char source[6];
			u_char type[2];
		} FrameHeader;

		int offset;
		EthernetPacket(const struct pcap_pkthdr* header, const u_char* pck);
		EthernetPacket(const Frame& frame);
		QJsonObject toObject() const override;
	};

	class Ipv4Packet :public EthernetPacket
	{
	public:
		typedef struct ip_address
		{
			u_char byte1;
			u_char byte2;
			u_char byte3;
			u_char byte4;
		};
		struct ipHeader
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
		}ipHeader;

		Ipv4Packet(const EthernetPacket eth);
		QJsonObject toObject() const override;
		int getTransportLayerProtorn() const;
	};

	class Ipv6Packet :public EthernetPacket
	{
	public:
		typedef struct ipv6_address
		{
			u_short byte[8];
		};
		struct ipv6Header
		{
			u_short vtf[2];
			u_short pay_length;
			u_char next_header;
			u_char hop_limit;
			ipv6_address    saddr;        // Source address
			ipv6_address    daddr;        // Destination address
		}ipv6Header;

		Ipv6Packet(const EthernetPacket &eth);
		QJsonObject toObject() const override;
	};

	class TcpPacket :public Ipv4Packet
	{
	public:
		struct tcp_header
		{
			u_short SourPort;
			u_short DestPort;
			u_int SequNum;
			u_int AcknowledgeNum;
			u_short HeaderLenAndFlag;
			u_short WindowSize;
			u_short CheckSum;
			u_short urgentPointer;
		}tcp_header;
		
		TcpPacket(const Ipv4Packet& ipp);
		QJsonObject toObject() const override;
	};

	class UdpPacket :public Ipv4Packet
	{
	public:
		struct udp_header
		{
			unsigned short SourPort; 
			unsigned short DestPort;
			unsigned short Length; 
			unsigned short CheckSum;
		}udp_header;

		UdpPacket(const Ipv4Packet& ipp);
		QJsonObject toObject() const override;
	};

	class HttpPacket :public TcpPacket
	{
	public:
		vector<string> httpData;
		HttpPacket(const TcpPacket& tcpp);
		QJsonObject toObject() const override;
	};

	class DnsPacket :public UdpPacket
	{
	private:
		string DnsData;
		struct dns_header
		{
			u_short flags;
			u_char question;
			u_char answer;
			u_char authority_rrs;
			u_char additional_rrs;
		}dns_header;
	public:
		DnsPacket(const UdpPacket& udpp);
		QJsonObject toObject() const override;
	};
}