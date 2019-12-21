#pragma once

#include <QObject>
#include <queue>
#include <mutex>
#include "Frame.h"
using namespace networkPacket;
class packetManger : public QObject
{
	Q_OBJECT

private:
	std::mutex queue_mutex; //保证线程安全
	std::mutex vector_mutex; //保证线程安全
	std::queue<networkPacket::Frame> unresolvePacket;
	std::vector<networkPacket::Packet> parsingCompletionPackage;
public:
	packetManger();
	~packetManger();
	std::vector<networkPacket::Packet> getAll();
	void pushToUnresolvePacket(Frame &frame);
	Frame popFromUnresolvePacket();

	void pushToCompletionPackage(Packet& pkg);
	Packet operator [](const int i) const;
	int completionSize() const;
	int unresolveSize() const;
};
