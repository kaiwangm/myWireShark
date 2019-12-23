/*
 * @Author: Kai Wang
 * @Date: 2019-12-12 12:26:19
 * @LastEditTime : 2019-12-24 01:07:29
 * @LastEditors  : JinHua Xv
 * @Description: 本文件实现了网卡选择界面类
 * @FilePath: \src\ui\devChoice.cpp
 */

#include "devChoice.h"


devChoice::devChoice(packetCapturer *pac): currentDev(0)
{
    ui.setupUi(this);

    QTreeWidget *tre = ui.treeWidget_dev_2;

    for (int i = 0; i < pac->networkDevice.size(); ++i) {
        QStringList infoList;                                           // 将网卡信息遍历展示

        infoList.append(QString(pac->networkDevice[i]->name));
        infoList.append(QString(pac->networkDevice[i]->description));
        tre->addTopLevelItem(new QTreeWidgetItem(infoList));
    }
}

/**
 * @description: 设置当前网卡序号
 */
void devChoice::setCurrentDev()
{
    currentDev = ui.treeWidget_dev_2->currentIndex().row();
}

/**
* @description: 获取当前选择的网卡
* @return: int 当前选择的网卡编号
*/
int devChoice::getCurrentDev()
{
    return currentDev;
}

devChoice::~devChoice()
{
}