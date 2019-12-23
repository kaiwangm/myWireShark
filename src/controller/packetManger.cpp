/*
 * @Author: JunWei Lin
 * @Date: 2019-12-10 01:56:54
 * @LastEditTime : 2019-12-23 23:58:03
 * @LastEditors  : JunWei Lin
 * @Description: 本文件实现了数据包管理类的相关函数
 * @FilePath: \src\controller\packetManger.cpp
 */
#include "packetManger.h"

packetManger::packetManger()
{
}

packetManger::~packetManger()
{
}

/**
* @description: 从外部获取分析完成的数据包数组
* @return: 返回分析完成数据包数组引用
*/
std::vector<networkPacket::Packet> packetManger::getAll()
{
    return parsingCompletionPackage;
}

/**
* @description: 将一个包放入待解析队列
* @param Frame frame 待放入队列包的引用
*/
void packetManger::pushToUnresolvePacket(Frame &frame)
{
    std::lock_guard<std::mutex> guard(queue_mutex);
    unresolvePacket.push(frame);
    sigUpdate();
}

/**
* @description:从未分析完成队列中取出头部包
* @return: 头部包
*/
Frame packetManger::popFromUnresolvePacket()
{
    std::lock_guard<std::mutex> guard(queue_mutex);
    Frame re = unresolvePacket.front();
    unresolvePacket.pop();
    sigUpdate();
    return re;
}

/**
* @description: 将一个包放入解析完成数组
* @param Packet pkg 待放入数组的包
*/
void packetManger::pushToCompletionPackage(Packet &pkg)
{
    std::lock_guard<std::mutex> guard(vector_mutex);
    parsingCompletionPackage.push_back(pkg);

}

/**
* @description: 使用下标来获取分析完成的数据包数组中的包
* @param int i 下标
* @return: 下标i对应的包引用
*/
Packet& packetManger::operator[](const int& i)
{
    return parsingCompletionPackage[i];
}

/**
* @description: 返回解析完成数组的大小
* @return: 解析完成数组的大小
*/
int packetManger::completionSize() const
{
    return parsingCompletionPackage.size();
}

/**
* @description: 返回解析完成数组的大小
* @return: 解析完成数组的大小
*/
int packetManger::unresolveSize()
{
    return unresolvePacket.size();
}

