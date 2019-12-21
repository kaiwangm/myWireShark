#include "FileManger.h"

void FileManger::save(QString path)
{
	auto allPacket = manger->getAll();
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	QJsonArray ary;
	for (auto i = allPacket.begin(); i != allPacket.end(); ++i)
	{
		QJsonObject obj;
		obj["data"] = QString(i->org_data.c_str());
		obj["time"] = i->captureInformation["time"];
		obj["length"] = i->captureInformation["length"];
		ary.push_back(obj);
	}

	QJsonDocument jsonDoc;
	jsonDoc.setArray(ary);

	file.write(jsonDoc.toJson());
	file.close();
}

void FileManger::open(QString path)
{
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	QByteArray allData = file.readAll();
	file.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
	QJsonArray ary = jsonDoc.array();
	
	QVariantList list = ary.toVariantList();
	for (auto i = list.begin(); i != list.end(); ++i)
	{
		QJsonObject tem = i->toJsonObject();
		Frame fr(tem["data"].toString(), tem["time"].toString(), tem["length"].toInt());
		manger->pushToUnresolvePacket(fr);
	}
}

FileManger::FileManger(packetManger* man)
	:manger(man)
{
}

FileManger::~FileManger()
{
}
