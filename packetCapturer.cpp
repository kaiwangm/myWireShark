#include "packetCapturer.h"

packetCapturer::packetCapturer(packetManger* man)
	:manger(man)
{
	char ErrorBuf[PCAP_ERRBUF_SIZE] = { 0 };
	portocolMap[6] = "TCP";
	portocolMap[2] = "IGMP";
	portocolMap[17] = "UDP";

	pcap_if_t* all_devs;
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &all_devs, ErrorBuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", ErrorBuf);
		ErrorFlag = 1;
		return;
	}

	for (; all_devs != NULL; all_devs = all_devs->next)
	{
		cout << all_devs->name << endl;
		networkDevice.push_back(all_devs);
	}

	ErrorFlag = 0;	
}

void packetCapturer::setHandle(int i)
{
	char ErrorBuf[PCAP_ERRBUF_SIZE] = { 0 };
	handler = pcap_open_live(networkDevice[i]->name,    // name of the device
		65536,            // portion of the packet to capture. 
						// 65536 grants that the whole packet will be captured on all the MACs.
		1,                // promiscuous mode (nonzero means promiscuous)
		1000,            // read timeout
		ErrorBuf            // error buffer
	);
	cout << ErrorBuf << endl;
}

int packetCapturer::getInitFlag()
{
	return ErrorFlag;
}

void packetCapturer::packet_handler(const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	Frame frame(header, pkt_data);
	manger->pushToUnresolvePacket(frame);
}

void packetCapturer::run()
{
	struct pcap_pkthdr pkhdr;
	while (true)
	{
		const unsigned char* packet_data = pcap_next(handler, &pkhdr);
		if (packet_data == NULL)
		{
			//pcap_close(handler);
			continue;
		}
		packet_handler(&pkhdr, packet_data);
	}
	
}

