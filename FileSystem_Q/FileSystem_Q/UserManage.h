#pragma once

#include <QWidget>
#include<QDebug>
#include "ui_UserManage.h"
#include<QStandardItemModel>
#include<QMessageBox>
#include "stdafx.h"
#include <QItemDelegate>
class UserManage : public QWidget
{
	Q_OBJECT

public:
	UserManage(QWidget *parent = Q_NULLPTR);
	~UserManage();
	void init();
	void init_tableView();
private:
	Ui::UserManage ui;

	QStandardItemModel * tablemodel;

private slots:
	void button_clicked();
	void delete_user(const QModelIndex &index);

};
