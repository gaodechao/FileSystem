#pragma once

#include <QWidget>
#include "ui_ViewBlock.h"
#include<iostream>
#include<QMessageBox>

#include<QStandardItemModel>
#include<QStringListModel>
#include<QStringList>

#include "stdafx.h"
using namespace std;

class ViewBlock : public QWidget
{
	Q_OBJECT

public:
	ViewBlock(QWidget *parent = Q_NULLPTR);
	void init();
	void init_SF();
	void init_Inode();
	void init_Block();
	bool read_blockmap();

	~ViewBlock();

private:
	Ui::ViewBlock ui;

	QStringListModel *sfModel;
	QStandardItemModel *inodeModel;
	QStandardItemModel *blockModel;
};
