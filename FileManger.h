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
#include "packetManger.h"
using namespace std;
class FileManger : public QObject
{
	Q_OBJECT
private:
	packetManger* manger;
public:
	void save(QString path);
	void open(QString path);
	FileManger(packetManger* man);
	~FileManger();
};
