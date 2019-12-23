/*
 * @Author: JunWei Lin
 * @Date: 2019-12-10 01:56:53
 * @LastEditTime : 2019-12-23 23:59:39
 * @LastEditors  : Kai Wang
 * @Description: 本文件定义数据包管理类
 * @FilePath: \src\controller\packetManger.h
 */

#pragma once
#include <QObject>
#include <queue>
#include <mutex>
#include <packet/Frame.h>

using namespace networkPacket;
class packetManger : public QObject
{
    Q_OBJECT

public:

    packetManger();
    ~packetManger();
    /**
    * @description: 从外部获取分析完成的数据包数组
    * @return: 返回分析完成数据包数组引用
    */
    std::vector<networkPacket::Packet> getAll();

    /**
    * @description: 使用下标来获取分析完成的数据包数组中的包
    * @param int i 下标
    * @return: 下标i对应的包引用
    */
    Packet& operator [](const int &i);

    /**
    * @description:从未分析完成队列中取出头部包
    * @return: 头部包
    */
    Frame popFromUnresolvePacket();

    /**
    * @description: 将一个包放入待解析队列
    * @param Frame frame 待放入队列包的引用
    */
    void pushToUnresolvePacket(Frame &frame);

    /**
    * @description: 将一个包放入解析完成数组
    * @param Packet pkg 待放入数组的包
    */
    void pushToCompletionPackage(Packet &pkg);

    /**
     * @description: 返回解析完成数组的大小
     * @return: 解析完成数组的大小
     */
    int completionSize() const;

    /**
    * @description: 返回解析完成数组的大小
    * @return: 解析完成数组的大小
    */
    int unresolveSize();

signals:
    /**
     * @description: 刷新GUI信号
     */
    void sigUpdate();

private:
    std::mutex queue_mutex; //��֤�̰߳�ȫ
    std::mutex vector_mutex; //��֤�̰߳�ȫ
    std::queue<networkPacket::Frame> unresolvePacket;
    std::vector<networkPacket::Packet> parsingCompletionPackage;

};
