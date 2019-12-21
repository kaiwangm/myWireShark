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
		std::vector<u_char> org_data;
		struct Interface
		{
			std::string time;
			int length;
		} Interface;
		Frame(QString data, QString time, int length)
		{
			Interface.time = time.toStdString();
			Interface.length = length;
			for (int i = 0; i < data.size(); i = i + 2)
			{
				u_char tem = tool::hex2dec(data[i].toLatin1(), data[i + 1].toLatin1());
				org_data.push_back(tem);
			}
			
		}
		Frame(const struct pcap_pkthdr* header, const u_char* pck) :
			org_data(pck, pck + header->len)
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
		struct FrameHeader
		{
			u_char destination[6];
			u_char source[6];
			u_char type[2];
		} FrameHeader;

		int offset;
		EthernetPacket(const struct pcap_pkthdr* header, const u_char* pck)
			:Frame(header, pck)
		{
			memcpy(FrameHeader.destination, pck, 6 * (sizeof u_char));
			memcpy(FrameHeader.source, pck + 6, 6 * (sizeof u_char));
			memcpy(FrameHeader.type, pck + 12, 2 * (sizeof u_char));

			offset = 14;
		}

		EthernetPacket(const Frame& frame)
			:Frame(frame)
		{
			memcpy(&FrameHeader, org_data.data(), 14 * (sizeof u_char));
			offset = 14;
		}

		QJsonObject toObject()
		{
			QJsonObject eth;


			string destination;
			string source;
			for (int i = 0; i < 6; ++i)
			{

				destination += tool::dec2hex(FrameHeader.destination[i]);
				source += tool::dec2hex(FrameHeader.source[i]);
				if (i != 5)
				{
					destination += ":";
					source += ":";
				}
			}

			eth["destination"] = QString(destination.c_str());
			eth["source"] = QString(source.c_str());
			if (FrameHeader.type[0] == 0x08 && FrameHeader.type[1] == 0x00)
			{
				eth["type"] = QString("ipv4");
			}
			else if (FrameHeader.type[0] == 0x86 && FrameHeader.type[1] == 0xdd)
			{
				eth["type"] = QString("ipv6");
			}
			else
			{
				eth["type"] = QString("null");
			}

			return eth;
		}

	};




	/* UDP header*/
	typedef struct udp_header
	{
		u_short sport;            // Source port
		u_short dport;            // Destination port
		u_short len;            // Datagram length
		u_short crc;            // Checksum
	}udp_header;

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

		Ipv4Packet(const EthernetPacket eth) :
			EthernetPacket(eth)
		{
			memcpy(&ipHeader, &org_data[offset], (sizeof ipHeader));
			offset += (sizeof ipHeader);
		}

		QJsonObject toObject()
		{
			QJsonObject ipv4;


			string destination;
			string source;


			destination += to_string(ipHeader.daddr.byte1) + ".";
			destination += to_string(ipHeader.daddr.byte2) + ".";
			destination += to_string(ipHeader.daddr.byte3) + ".";
			destination += to_string(ipHeader.daddr.byte4);
			source += to_string(ipHeader.saddr.byte1) + ".";
			source += to_string(ipHeader.saddr.byte2) + ".";
			source += to_string(ipHeader.saddr.byte3) + ".";
			source += to_string(ipHeader.saddr.byte4);


			ipv4["destination"] = QString(destination.c_str());
			ipv4["source"] = QString(source.c_str());

			return ipv4;
		}
		int getTransportLayerProtorn()
		{
			return ipHeader.proto;
		}
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

		Ipv6Packet(const EthernetPacket eth) :
			EthernetPacket(eth)
		{
			memcpy(&ipv6Header, &org_data[offset], (sizeof ipv6Header));
			offset += (sizeof ipv6Header);
		}

		QJsonObject toObject()
		{
			QJsonObject ipv6;


			string destination;
			string source;


			for (int i = 0; i < 8; i = i + 1)
			{
				destination += tool::dec2hex(ipv6Header.daddr.byte[i]);


				source += tool::dec2hex(ipv6Header.saddr.byte[i]);


				if (i != 7)
				{
					destination += ":";
					source += ":";
				}
			}


			ipv6["destination"] = QString(destination.c_str());
			ipv6["source"] = QString(source.c_str());

			return ipv6;
		}
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


		TcpPacket(const Ipv4Packet& ipp) :
			Ipv4Packet(ipp)
		{
			memcpy(&tcp_header, &org_data[offset], (sizeof tcp_header));


			offset += (sizeof tcp_header);
		}

		QJsonObject toObject()
		{
			QJsonObject tcp;

			tcp["sourcePort"] = tool::bswap_16(tcp_header.SourPort);
			tcp["destPort"] = tool::bswap_16(tcp_header.DestPort);
			tcp["SEQ"] = (int)tool::bswap_32(tcp_header.SequNum);
			tcp["ACK"] = (int)tool::bswap_32(tcp_header.AcknowledgeNum);
			tcp["Windows Size value"] = tool::bswap_16(tcp_header.WindowSize);
			tcp["Checksum"] = QString("0x") + QString(tool::dec2hex(tool::bswap_16(tcp_header.CheckSum)).c_str());
			tcp["Urgent Pointer"] = tool::bswap_16(tcp_header.urgentPointer);

			auto flags = (tool::bswap_16(tcp_header.HeaderLenAndFlag) & 0x0FFF);
			if ( flags == 0x0010)
			{
				tcp["Flags"] = "(ACK)[0x010]";
			}
			else if (flags == 0x0018)
			{
				tcp["Flags"] = "(PSH, ACK)[0x018]";
			}
			else if (flags == 0x0011)
			{
				tcp["Flags"] = "(FIN, ACK)[0x011]";
			}
			else if (flags == 0x0004)
			{
				tcp["Flags"] = "(RST)[0x001]";
			}
			return tcp;
		}
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


		UdpPacket(const Ipv4Packet& ipp) :
			Ipv4Packet(ipp)
		{
			memcpy(&udp_header, &org_data[offset], (sizeof udp_header));

			offset += (sizeof udp_header);
		}

		QJsonObject toObject()
		{
			QJsonObject udp;

			udp["sourcePort"] = tool::bswap_16(udp_header.SourPort);
			udp["destPort"] = tool::bswap_16(udp_header.DestPort);
			return udp;
		}
	};

	class HttpPacket :public TcpPacket
	{
	public:
		vector<string> httpData;
		HttpPacket(const TcpPacket& tcpp)
			:TcpPacket(tcpp)
		{
			string httpStr;
			for (int i = offset; i < org_data.size(); ++i)
			{
				if (org_data[i] >= ' ' && org_data[i] <= '~')
				{
					httpStr += org_data[i];
				}
				else if (org_data[i] == 10)
				{
					httpStr += "\n";
				}
				else if (org_data[i] == 13)
				{
					httpStr += "\r";
				}
				else
				{
					httpStr += "-";
				}
			}

			httpData = tool::split(httpStr, "\r\n");
		}

		QJsonObject toObject()
		{
			QJsonObject http;

			for (int i = 0; i < httpData.size(); ++i)
			{
				http[QString::number(i)] = QString(httpData[i].c_str());
			}
			
			return http;
		}
	};
}