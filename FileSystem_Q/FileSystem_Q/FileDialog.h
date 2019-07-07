#pragma once

#include <QWidget>
#include <QString>
#include "ui_FileDialog.h"
#include<iostream>
#include<QDebug>
#include "stdafx.h"
#include <QMessageBox>
using namespace std;
class FileDialog : public QWidget
{
	Q_OBJECT

public:
	FileDialog(QWidget *parent = Q_NULLPTR);
	FileDialog(QString content,char *filename,inode &currentInode,QWidget *parent = Q_NULLPTR);
	~FileDialog();

	//重写关闭事件
	void closeEvent(QCloseEvent *);
	void show_size();
signals:
	void ExitWin(QString filename);

private:
	Ui::FileDialog ui;
	inode fileinode;
	bool write_flag;
	QString filename;


private slots:
	void save();
	void save2();
	void cancle();
	void count();
};
