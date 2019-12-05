#include "packetCapturer.h"

packetCapturer::packetCapturer() 
{
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &all_devs, ErrorBuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", ErrorBuf);
		exit(1);
	}
	/* ´òÓ¡ÁÐ±í */
	int i = 0;
	for (auto d = all_devs; d != NULL; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
	}

	if ((handler = pcap_open_live((&all_devs[0])->name,    // name of the device
		65536,            // portion of the packet to capture. 
					    // 65536 grants that the whole packet will be captured on all the MACs.
		1,                // promiscuous mode (nonzero means promiscuous)
		1000,            // read timeout
		ErrorBuf            // error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* Free the device list */
		pcap_freealldevs(all_devs);
	}
}

void packetCapturer::packet_handler(const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	stringstream stro;

	struct tm* ltime;
	time_t local_tv_sec;
	char timestr[16];

	ip_header* ih;
	udp_header* uh;
	u_int ip_len;
	u_short sport, dport;

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
	

	/* print timestamp and length of the packet */
	cout << timestr << " length: " << header->len << endl;

	stro << timestr << " length: " << header->len;

	ih = (ip_header*)(pkt_data +
		14); //length of ethernet header

	/* retireve the position of the udp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header*)((u_char*)ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = uh->sport;
	dport = uh->dport;

	/* print ip addresses and udp ports */
	char* a = new char[1000];
	sprintf(a,"%d.%d.%d.%d::%d -> %d.%d.%d.%d::%d\n",
		ih->saddr.byte1,
		ih->saddr.byte2,
		ih->saddr.byte3,
		ih->saddr.byte4,
		sport,
		ih->daddr.byte1,
		ih->daddr.byte2,
		ih->daddr.byte3,
		ih->daddr.byte4,
		dport);

	stro<<"   " <<a;

	vs.push_back(stro.str());
}

void packetCapturer::run()
{
	struct pcap_pkthdr pkhdr;
	while (true)
	{
		const unsigned char* packet_data = pcap_next(handler, &pkhdr);
		if (packet_data == NULL)
		{
			pcap_close(handler);
			break;
		}
		packet_handler(&pkhdr, packet_data);
	}
	
}

