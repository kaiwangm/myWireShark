/*
 * @Author: Kai Wang
 * @Date: 2019-12-10 19:23:32
 * @LastEditTime : 2019-12-23 23:39:55
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \src\controller\FileManger.h
 */

#pragma once
#include <QObject>
#include <QFile>
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <QVariantMap>
#include <QVariantList>
#include <controller/packetManger.h>

using namespace std;
class FileManger : public QObject
{
    Q_OBJECT
private:
    packetManger *manger;
public:
    FileManger(packetManger *man);
    ~FileManger();
    /**
     * @description: 保存数据包到文件
     * @param QString path 文件路径
     */
    void save(QString path);
    /**
    * @description: 从文件打开数据包
    * @param QString path 文件路径
    */
    void open(QString path);

};
