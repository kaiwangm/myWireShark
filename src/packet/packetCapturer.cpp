/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 01:53:00
 * @LastEditTime : 2019-12-24 01:25:05
 * @LastEditors  : Kai Wang
 * @Description: 实现了了数据包捕获类的相关函数
 * @FilePath: \src\packet\packetCapture.cpp
 */

#include "packetCapturer.h"

packetCapturer::packetCapturer(packetManger *man)
    : manger(man)
{
    char ErrorBuf[PCAP_ERRBUF_SIZE] = { 0 };

    pcap_if_t *all_devs;
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &all_devs, ErrorBuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", ErrorBuf);    // 捕获程序初始化失败，输出错误信息
        ErrorFlag = 1;
        return;
    }

    for (; all_devs != NULL; all_devs = all_devs->next) { // 将网卡指针放入数组管理
        networkDevice.push_back(all_devs);
    }

    ErrorFlag = 0;
}

/**
* @description: 设置当前网卡
* @param int i 网卡序号
*/
void packetCapturer::setHandle(int i)
{
    char ErrorBuf[PCAP_ERRBUF_SIZE] = { 0 };
    handler = pcap_open_live(networkDevice[i]->name,    // 网卡名称
                             65536,                     // 最大端口号
                             1,                         // 最小端口号
                             1000,                      // 超时时间
                             ErrorBuf                   // 错误信息输出数组
                            );
    cout << ErrorBuf << endl;
}

/**
 * @description: 获取错误代码
 * @return: int 错误代码
 */
int packetCapturer::getInitFlag()
{
    return ErrorFlag;
}

void packetCapturer::packet_handler(const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    Frame frame(header, pkt_data);
    manger->pushToUnresolvePacket(frame);
}

/**
 * @description: 线程运行函数
 */
void packetCapturer::run()
{
    struct pcap_pkthdr pkhdr;
    while (true) {
        const unsigned char *packet_data = pcap_next(handler, &pkhdr);
        if (packet_data == NULL) {
            //pcap_close(handler);
            continue;
        }
        packet_handler(&pkhdr, packet_data);
    }

}

