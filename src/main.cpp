/*
 * @Author: Kai Wang
 * @Date: 2019-12-07 11:21:21
 * @LastEditTime : 2019-12-08 01:10:39
 * @LastEditors  : Kai Wang
 * @Description: 定义了软甲入口函数
 * @FilePath: \src\main.cpp
 */

#include <QtWidgets/QApplication>
#include <ui/myWireShark.h>
#include <packet/packetCapturer.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	myWireShark w;
	w.show();
	return a.exec();
}
