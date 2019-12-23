/*
 * @Author: Kai Wang
 * @Date: 2019-12-07 20:40:32
 * @LastEditTime : 2019-12-24 00:33:57
 * @LastEditors  : Kai Wang
 * @Description: 本文件定义了常见的数据包类，并定义了一些网络数据处理工具函数
 * @FilePath: \src\packet\Frame.h
 */

#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <pcap.h>
#include <cstdint>
#include <cstring>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <sstream>
#include <iomanip>

using namespace std;

namespace tool
{
/**
* @description: 十进制转十六进制
* @param int i 十进制整数
* @return: string 十六进制字符串
*/
string dec2hex(int i);

/**
* @description: 十六进制转十进制
* @param char a, char b 十六进制的两位
* @return: u_char 十进制整数
*/
u_char hex2dec(char a, char b);

/**
* @description: 16位大小端转换
* @param u_short &x  16位数
* @return: u_short   转换后的16位数
*/
u_short bswap_16(const u_short &x);

/**
* @description: 32位大小端转换
* @param u_int &x  32位数
* @return: u_int   转换后的32位数
*/
u_int bswap_32(const u_int &x);

/**
* @description: 切分字符串
* @param string &str 源字符串, string &pattern 分隔符
* @return: vector<string> 切分后字符串数组
*/
vector<string> split(const string &str, const string &pattern);

}

namespace networkPacket
{
/**
 * @description: 数据包的类定义
 */
class Packet
{
public:
    std::string extractInfo;                    // 基础描述信息
    std::string org_data;                       // 原始数据
    std::string ascii_data;                     // 转换为ASCII的字符串
    std::string portocol;                       // 协议名
    QJsonObject captureInformation;             // 捕获状态信息
    QJsonObject linkLayerInformation;           // 链接层解析结果
    QJsonObject IPLayerInformation;             // 网络层解析结果
    QJsonObject transportLayerInformation;      // 传输层解析结果
    QJsonObject applicationLayerInformation;    // 应用层解析结果
};

/**
 * @description: 帧基类的定义
 */
class Frame
{
public:
    struct Interface {
        std::string time;           // Packet receive time
        int length;                 // Packet length
    } Interface;
    std::vector<u_char> org_data;
    Frame(QString data, QString time, int length);
    Frame(const struct pcap_pkthdr *header, const u_char *pck);
    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    virtual QJsonObject toObject() const;
};

/**
 * @description: 以太帧类的定义
 */
class EthernetPacket : public Frame
{
public:
    struct FrameHeader {            // Ethernet II Header
        u_char destination[6];      // Destination address
        u_char source[6];           // Source address
        u_char type[2];             // Network Layer protocol type
    } FrameHeader;

    int offset;                     //解析偏移量
    EthernetPacket(const struct pcap_pkthdr *header, const u_char *pck);
    EthernetPacket(const Frame &frame);

    /**
    * @description: 将对象转为Json对象
    * @return: Json对象
    */
    QJsonObject toObject() const override;
};

/**
 * @description: ipv4包类的定义，继承于以太帧
 */
class Ipv4Packet : public EthernetPacket
{
public:
    typedef struct ip_address {     // ipv4 address
        u_char byte1;
        u_char byte2;
        u_char byte3;
        u_char byte4;
    };
    struct ipHeader {               // Ipv4 Header
        u_char    ver_ihl;          // Version (4 bits) + Internet header length (4 bits)
        u_char    tos;              // Type of service
        u_short tlen;               // Total length
        u_short identification;     // Identification
        u_short flags_fo;           // Flags (3 bits) + Fragment offset (13 bits)
        u_char    ttl;              // Time to live
        u_char    proto;            // Protocol
        u_short crc;                // Header checksum
        ip_address    saddr;        // Source address
        ip_address    daddr;        // Destination address
    } ipHeader;

    Ipv4Packet(const EthernetPacket eth);

    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    QJsonObject toObject() const override;

    /**
     * @description: 获取下一层协议种类
     * @return: int 协议种类
     */
    int getTransportLayerProtorn() const;
};

/**
 * @description: ipv6包类的定义，继承于以太帧
 */
class Ipv6Packet : public EthernetPacket
{
public:
    typedef struct ipv6_address {       // ipv6 address
        u_short byte[8];
    };
    struct ipv6Header {                 // Ipv6 Header
        u_short vtf[2];
        u_short pay_length;             // Pay Length
        u_char next_header;             // Next Header pointer
        u_char hop_limit;               // hop Limit
        ipv6_address    saddr;          // Source address
        ipv6_address    daddr;          // Destination address
    } ipv6Header;

    Ipv6Packet(const EthernetPacket &eth);

    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    QJsonObject toObject() const override;
};

/**
 * @description: Tcp包类的定义，继承于ipv4类
 */
class TcpPacket : public Ipv4Packet
{
public:
    struct tcp_header {                 // TCP Header
        u_short SourPort;               // Source address
        u_short DestPort;               // Destination address
        u_int SequNum;                  // Sequence number
        u_int AcknowledgeNum;           // Acknowledge number
        u_short HeaderLenAndFlag;       // Header Length and Flag
        u_short WindowSize;             // Window size
        u_short CheckSum;               // Check Sum
        u_short urgentPointer;          // Urgent pointer
    } tcp_header;

    TcpPacket(const Ipv4Packet &ipp);

    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    QJsonObject toObject() const override;
};

/**
 * @description: Udp包类的定义，继承于ipv4类
 */
class UdpPacket : public Ipv4Packet
{
public:
    struct udp_header {                 // UDP Header
        unsigned short SourPort;        // Source address
        unsigned short DestPort;        // Destination address
        unsigned short Length;          // udp length
        unsigned short CheckSum;        // Check Sum
    } udp_header;

    UdpPacket(const Ipv4Packet &ipp);

    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    QJsonObject toObject() const override;
};

/**
 * @description: Http包类的定义，继承于Tcp类
 */
class HttpPacket : public TcpPacket
{
public:
    vector<string> httpData;            // HTTP Data
    HttpPacket(const TcpPacket &tcpp);

    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    QJsonObject toObject() const override;
};

/**
 * @description: Dns包类的定义，继承于Udp类
 */
class DnsPacket : public UdpPacket
{
private:
    string DnsData;
    struct dns_header {                 // DNS Header
        u_short flags;                  // DNS Flags
        u_char question;                // Question number
        u_char answer;                  // Answer number
        u_char authority_rrs;           // Authority Rrs
        u_char additional_rrs;          // Additional Rrs
    } dns_header;
public:
    DnsPacket(const UdpPacket &udpp);

    /**
     * @description: 将对象转为Json对象
     * @return: Json对象
     */
    QJsonObject toObject() const override;
};
}