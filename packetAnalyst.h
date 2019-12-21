#pragma once
#include <Qthread>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "packetManger.h"

using namespace std;
class packetAnalyst : public QThread
{
	Q_OBJECT

private:
	packetManger* manger;
	void run() override;
	Packet analysisPacket(Frame& orgin);
public:
	packetAnalyst(packetManger* man);
	~packetAnalyst();
};
