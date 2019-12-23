/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 01:52:00
 * @LastEditTime : 2019-12-24 01:25:03
 * @LastEditors  : JunWei Lin
 * @Description: 实现了数据包分析类的相关函数
 * @FilePath: \src\packet\packetAnalyst.cpp
 */

#include "packetAnalyst.h"

/**
 * @description:  线程运行函数
 */
void packetAnalyst::run()
{
    while (true) {
        if (manger->unresolveSize() > 0) {
            Frame orgin = manger->popFromUnresolvePacket();
            Packet result = analysisPacket(orgin);
            manger->pushToCompletionPackage(result);
        }
    }
}

/**
* @description: 包分析函数
* @param Frame &orgin 原始数据包
* @return: Packet 分析完成的包
*/
Packet packetAnalyst::analysisPacket(Frame &orgin)
{
    Packet result;
    string orgData;

    // 将原始数据转换为16进制
    for (auto i : orgin.org_data) {
        string hx = tool::dec2hex(i);
        if (hx.size() == 1) {
            orgData += "0";
            orgData += hx;
        } else {
            orgData += hx;
        }
    }
    result.org_data = orgData;

    // 将原始数据转换为ASCII码
    string asciiData;
    for (auto i : orgin.org_data) {
        if (i >= ' ' && i <= '~') {
            asciiData += i;
        } else {
            asciiData += "-";
        }
    }
    result.ascii_data = asciiData;
    result.captureInformation["time"] = orgin.Interface.time.c_str();
    result.captureInformation["length"] = orgin.Interface.length;


    // 解析链路层协议
    EthernetPacket eth(orgin);
    result.linkLayerInformation = eth.toObject();
    result.portocol = "Ethernet II";
    result.extractInfo = "Ethernet II Packet";

    if (eth.FrameHeader.type[0] == 0x08 && eth.FrameHeader.type[1] == 0x00) {

        // 解析网络层协议
        Ipv4Packet ipp(eth);
        result.IPLayerInformation = ipp.toObject();
        result.portocol = "ipv4";
        result.extractInfo = "Unknown transport layer protocol";
        if (ipp.getTransportLayerProtorn() == 6) {

            // 解析传输层TCP协议
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
            if (offset + 4 <= tcp.org_data.size()
                    && (tcp.org_data[offset] == 'H'
                        && tcp.org_data[offset + 1] == 'T'
                        && tcp.org_data[offset + 2] == 'T'
                        && tcp.org_data[offset + 3] == 'P'
                        || tcp.org_data[offset] == 'P'
                        && tcp.org_data[offset + 1] == 'O'
                        && tcp.org_data[offset + 2] == 'S'
                        && tcp.org_data[offset + 3] == 'T'
                        || tcp.org_data[offset] == 'G'
                        && tcp.org_data[offset + 1] == 'E'
                        && tcp.org_data[offset + 2] == 'T')) {

                // 解析传输层HTTP协议
                HttpPacket httpp(tcp);
                result.applicationLayerInformation = httpp.toObject();
                result.portocol = "http";
                result.extractInfo = httpp.httpData[0];
                return result;
            }
        } else if (ipp.getTransportLayerProtorn() == 17) {

            // 解析传输层UDP协议
            UdpPacket udp(ipp);
            result.transportLayerInformation = udp.toObject();
            result.portocol = "udp";
            result.extractInfo = to_string(result.transportLayerInformation["sourcePort"].toInt());
            result.extractInfo += " -> ";
            result.extractInfo += to_string(result.transportLayerInformation["destPort"].toInt());
            result.extractInfo += " Lengrh: ";
            result.extractInfo += to_string(result.transportLayerInformation["length"].toInt());
            if (result.transportLayerInformation["destPort"].toInt() == 53 ||
                    result.transportLayerInformation["sourcePort"].toInt() == 53) {

                // 解析传输层DNS协议
                DnsPacket dnsp(udp);
                result.applicationLayerInformation = dnsp.toObject();
                result.portocol = "dns";
            }
        } else {
            return result;
        }
    } else if (eth.FrameHeader.type[0] == 0x86 && eth.FrameHeader.type[1] == 0xdd) {

        // 解析传输层Ipv6协议
        Ipv6Packet ipp(eth);
        result.IPLayerInformation = ipp.toObject();
        result.portocol = "ipv6";
        result.extractInfo = "Unknown transport layer protocol";
    }

    return result;
}

packetAnalyst::packetAnalyst(packetManger *man)
{
    manger = man;
}

packetAnalyst::~packetAnalyst()
{
}
