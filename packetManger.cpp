#include "packetManger.h"

packetManger::packetManger()

{
}

packetManger::~packetManger()
{
}

std::vector<networkPacket::Packet> packetManger::getAll()
{
	return parsingCompletionPackage;
}

void packetManger::pushToUnresolvePacket(Frame& frame)
{
	std::lock_guard<std::mutex> guard(queue_mutex);
	unresolvePacket.push(frame);
}

Frame packetManger::popFromUnresolvePacket()
{
	std::lock_guard<std::mutex> guard(queue_mutex);
	Frame re = unresolvePacket.front();
	unresolvePacket.pop();
	
	return re;
}

void packetManger::pushToCompletionPackage(Packet& pkg)
{
	std::lock_guard<std::mutex> guard(vector_mutex);
	parsingCompletionPackage.push_back(pkg);
}

Packet packetManger::operator[](const int i) const
{
	return parsingCompletionPackage[i];
}

int packetManger::completionSize() const
{
	return parsingCompletionPackage.size();
}

int packetManger::unresolveSize() const
{
	return unresolvePacket.size();
}

