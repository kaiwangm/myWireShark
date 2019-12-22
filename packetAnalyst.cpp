#include "packetAnalyst.h"

void packetAnalyst::run()
{
	
	while (true)
	{
		if (manger->unresolveSize() > 0)
		{
			Frame orgin = manger->popFromUnresolvePacket();

			Packet result = analysisPacket(orgin);
			manger->pushToCompletionPackage(result);
		}
	}
}

Packet packetAnalyst::analysisPacket(Frame& orgin)
{
	Packet result;
	string orgData;
	for (auto i : orgin.org_data)
	{
		string hx = tool::dec2hex(i);
		if (hx.size() == 1)
		{
			orgData += "0";
			orgData += hx;
		}
		else
		{
			orgData += hx;

		}
		
	}
	result.org_data = orgData;

	string asciiData;
	for (auto i : orgin.org_data)
	{
		if (i >= ' ' && i<='~')
		{
			asciiData += i;
		}
		else
		{
			asciiData += "-";
		}
	}
	result.ascii_data = asciiData;

	result.captureInformation["time"] = orgin.Interface.time.c_str();
	result.captureInformation["length"] = orgin.Interface.length;

	
	EthernetPacket eth(orgin);
	result.linkLayerInformation = eth.toObject();
	result.portocol = "Ethernet II";

	result.extractInfo = "Ethernet II Packet";
	if (eth.FrameHeader.type[0] == 0x08 && eth.FrameHeader.type[1] == 0x00)
	{
		Ipv4Packet ipp(eth);
		result.IPLayerInformation = ipp.toObject();
		result.portocol = "ipv4";
		result.extractInfo = "Unknown transport layer protocol";
		if (ipp.getTransportLayerProtorn() == 6)
		{
			TcpPacket tcp(ipp);
			result.transportLayerInformation = tcp.toObject();
			result.portocol = "tcp";
			result.extractInfo = to_string(result.transportLayerInformation["sourcePort"].toInt());
			result.extractInfo += " -> ";
			result.extractInfo += to_string(result.transportLayerInformation["destPort"].toInt());
			result.extractInfo += " Flags:";
			result.extractInfo += result.transportLayerInformation["Flags"].toString().toStdString();
			result.extractInfo += " Win:";
			result.extractInfo += to_string(result.transportLayerInformation["Windows Size value"].toInt());

			int offset = tcp.offset;
			if (offset+4 <= tcp.org_data.size()
				&& (tcp.org_data[offset]=='H'
				&& tcp.org_data[offset+1] == 'T'
				&& tcp.org_data[offset+2] == 'T'
				&& tcp.org_data[offset+3] == 'P'
				|| tcp.org_data[offset] == 'P'
				&& tcp.org_data[offset + 1] == 'O'
				&& tcp.org_data[offset + 2] == 'S'
				&& tcp.org_data[offset + 3] == 'T'
				|| tcp.org_data[offset] == 'G'
				&& tcp.org_data[offset + 1] == 'E'
				&& tcp.org_data[offset + 2] == 'T'))
			{
				HttpPacket httpp(tcp);
				result.applicationLayerInformation = httpp.toObject();
				result.portocol = "http";
				result.extractInfo = httpp.httpData[0];
				return result;
			}
		}
		else if(ipp.getTransportLayerProtorn() == 17)
		{
			UdpPacket udp(ipp);
			result.transportLayerInformation = udp.toObject();
			result.portocol = "udp";
			result.extractInfo = to_string(result.transportLayerInformation["sourcePort"].toInt());
			result.extractInfo += " -> ";
			result.extractInfo += to_string(result.transportLayerInformation["destPort"].toInt());
			result.extractInfo += " Lengrh: ";
			result.extractInfo += to_string(result.transportLayerInformation["length"].toInt());
			if (result.transportLayerInformation["destPort"].toInt() == 53 ||
				result.transportLayerInformation["sourcePort"].toInt() == 53)
			{
				DnsPacket dnsp(udp);
				result.applicationLayerInformation = dnsp.toObject();
				result.portocol = "dns";
			}
		}
		else
		{
			return result;
		}
	}
	else if (eth.FrameHeader.type[0] == 0x86 && eth.FrameHeader.type[1] == 0xdd)
	{
		Ipv6Packet ipp(eth);
		result.IPLayerInformation = ipp.toObject();
		result.portocol = "ipv6";
		result.extractInfo = "Unknown transport layer protocol";
	}

	return result;
}

packetAnalyst::packetAnalyst(packetManger* man)
{
	manger = man;
}

packetAnalyst::~packetAnalyst()
{
}
