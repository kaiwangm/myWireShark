#include "Frame.h"
using namespace networkPacket;

string tool::dec2hex(int i) //将int转成16进制字符串
{
	stringstream ioss; //定义字符串流
	string s_temp; //存放转化后字符
	ioss << setiosflags(ios::uppercase) << hex << i; //以十六制(大写)形式输出
	//ioss << resetiosflags(ios::uppercase) << hex << i; //以十六制(小写)形式输出//取消大写的设置
	ioss >> s_temp;
	return s_temp;
}


//无符号整型16位  
u_short tool::bswap_16(const u_short &x)
{
	return (((u_short)(x) & 0x00ff) << 8) | \
		(((u_short)(x) & 0xff00) >> 8);
}

//无符号整型32位
u_int tool::bswap_32(const u_int &x)
{
	return (((u_int)(x) & 0xff000000) >> 24) | \
		(((u_int)(x) & 0x00ff0000) >> 8) | \
		(((u_int)(x) & 0x0000ff00) << 8) | \
		(((u_int)(x) & 0x000000ff) << 24);
}

u_char tool::hex2dec(char a, char b)
{
	u_char x, y;

	if (a >= '0' && a <= '9')
	{
		x = a - '0';
	}
	else
	{
		x = a - 'A' +10;
	}

	if (b >= '0' && b <= '9')
	{
		y = b - '0';
	}
	else
	{
		y = b - 'A' +10;
	}
	u_char res = (x << 4) + y;
	return res;
}

vector<string> tool::split(const string& str, const string& pattern)
{
	vector<string> res;
	if (str == "")
		return res;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + pattern;
	size_t pos = strs.find(pattern);

	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 2, strs.size());
		pos = strs.find(pattern);
		
		if (temp == "")
		{
			res.push_back(strs);
			return res;
		}
	}

	return res;
}

Frame::Frame(QString data, QString time, int length)
{
	Interface.time = time.toStdString();
	Interface.length = length;
	for (int i = 0; i < data.size(); i = i + 2)
	{
		u_char tem = tool::hex2dec(data[i].toLatin1(), data[i + 1].toLatin1());
		org_data.push_back(tem);
	}

}

Frame::Frame(const struct pcap_pkthdr* header, const u_char* pck) :
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

QJsonObject Frame::toObject() const
{
	return QJsonObject();
}

EthernetPacket::EthernetPacket(const struct pcap_pkthdr* header, const u_char* pck)
	:Frame(header, pck)
{
	memcpy(FrameHeader.destination, pck, 6 * (sizeof u_char));
	memcpy(FrameHeader.source, pck + 6, 6 * (sizeof u_char));
	memcpy(FrameHeader.type, pck + 12, 2 * (sizeof u_char));

	offset = 14;
}

EthernetPacket::EthernetPacket(const Frame& frame)
	:Frame(frame)
{
	memcpy(&FrameHeader, org_data.data(), 14 * (sizeof u_char));
	offset = 14;
}

QJsonObject EthernetPacket::toObject() const
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

Ipv4Packet::Ipv4Packet(const EthernetPacket eth) :
	EthernetPacket(eth)
{
	memcpy(&ipHeader, &org_data[offset], (sizeof ipHeader));
	offset += (sizeof ipHeader);
}

QJsonObject Ipv4Packet::toObject() const
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

int Ipv4Packet::getTransportLayerProtorn() const
{
	return ipHeader.proto;
}

Ipv6Packet::Ipv6Packet(const EthernetPacket &eth) :
	EthernetPacket(eth)
{
	memcpy(&ipv6Header, &org_data[offset], (sizeof ipv6Header));
	offset += (sizeof ipv6Header);
}

QJsonObject Ipv6Packet::toObject() const
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

TcpPacket::TcpPacket(const Ipv4Packet& ipp) :
	Ipv4Packet(ipp)
{
	memcpy(&tcp_header, &org_data[offset], (sizeof tcp_header));


	offset += (sizeof tcp_header);
}

QJsonObject TcpPacket::toObject() const
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
	if (flags == 0x0010)
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

UdpPacket::UdpPacket(const Ipv4Packet& ipp) :
	Ipv4Packet(ipp)
{
	memcpy(&udp_header, &org_data[offset], (sizeof udp_header));

	offset += (sizeof udp_header);
}

QJsonObject UdpPacket::toObject() const
{
	QJsonObject udp;

	udp["sourcePort"] = tool::bswap_16(udp_header.SourPort);
	udp["destPort"] = tool::bswap_16(udp_header.DestPort);
	udp["length"] = tool::bswap_16(udp_header.Length);
	return udp;
}

HttpPacket::HttpPacket(const TcpPacket& tcpp)
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

QJsonObject HttpPacket::toObject() const
{
	QJsonObject http;

	for (int i = 0; i < httpData.size(); ++i)
	{
		http[QString::number(i)] = QString(httpData[i].c_str());
	}

	return http;
}

DnsPacket::DnsPacket(const UdpPacket& udpp)
	:UdpPacket(udpp)
{
	memcpy(&dns_header, &org_data[offset], (sizeof dns_header));
	offset += (sizeof dns_header);

	for (int i = offset; i < org_data.size(); ++i)
	{
		if (org_data[i] >= ' ' && org_data[i] <= '~')
		{
			DnsData += org_data[i];
		}
		else if (org_data[i] == 10)
		{
			DnsData += "\n";
		}
		else
		{
			DnsData += "-";
		}
	}
}

QJsonObject DnsPacket::toObject() const
{
	QJsonObject dns;

	dns["flags"] = QString("Flags:0x") + QString(tool::dec2hex(tool::bswap_32(dns_header.flags)).c_str());
	dns["Question"] = QString("Question: ") + QString::number(dns_header.question);
	dns["Answer"] = QString("Answer: ") + QString::number(dns_header.answer);
	dns["Authority Rrs"] = QString("Authority Rrs: ") + QString::number(dns_header.authority_rrs);
	dns["Additional Rrs"] = QString("Additional Rrs: ") + QString::number(dns_header.additional_rrs);
	dns["_Data"] = QString("Data: ") + QString(DnsData.c_str());

	return dns;
}