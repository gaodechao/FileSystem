#pragma once

#include <QtWidgets/QMainWindow>
#include<QFileSystemModel>
#include<QStandardItemModel>
#include<QStringList>
#include<QMap>
#include<QDebug>
#include<QIcon>
#include<QString>
#include<QMessageBox>
#include <QPersistentModelIndex>
#include "ui_FileSystem_Q.h"
#include <string>
#include "ViewBlock.h"
#include<QInputDialog>
#include <iterator>

#include "stdafx.h"
#include "FileDialog.h"
#include "UserManage.h"
#include "UserLogin.h"
#include "PassWord.h"

using namespace std;
class FileSystem_Q : public QMainWindow
{
	Q_OBJECT

public:

	copydir cpdir(char *dirname);
	void pastedir(copydir bp);
	void makequeue(copydir *&d);


	FileSystem_Q(QWidget *parent = Q_NULLPTR);
	void init_File();
	void init_connect();
	QString getCurrentItem();
	QString getParentItem();
	QString getParentItem(const QModelIndex &index);
	void init_tableView();
	void init_View_data();
	void init_tableView_style();
	void init_fileMenu();
	void file_Ergodic(QStandardItem *item,directory dir);
	void upData_ab_dir();
	void open_Inode(int inodeID);
	void open_Item(const QModelIndex &index);
	void open_file(char *filename);
	bool eventFilter(QObject* obj, QEvent *evt);
	bool OpenfileManage(QString filename);
	void show_Inode(QModelIndex &index);


private:

	Ui::FileSystem_QClass ui;
	//窗体
	ViewBlock *viewBlock;
	UserManage *user_manage;
	UserLogin *login;
	PassWord *passwd;
	bool copyflag = false;
	bool copydirflag = false;
	QModelIndex inode_index;
	QString copyfile;
	inode copyInode;

	copydir copyDir;

	QStandardItemModel *treemodel;//
	QStandardItemModel *tablemodel;//
	QStandardItemModel *inodeModel;
	QMenu *tablemenu;
	QMap<QModelIndex, int> file_index_inode;
	QMap<QString, QIcon> m_publicIconMap;//< 存放公共图标

	//存放打开文件夹的目录
	QStringList OpenFileList;
	inode current_inode_table;
	QString current_filename;

	void creatItemMenu();

	//function

private slots:

	void receiveUser(string username);
	void onTableClicked(const QModelIndex &);
	void AccessChange(const QModelIndex &);

	void openItem_tree();
	void openItem_table();

	void copy_table();
	void cut_table();
	void build_folder_table();
	void build_file_table();
	void rename_table();
	void delete_table();
	void View_Property();
	void PasswordChange();
	void manage();
	void newLogin();//重新登录
	void Exit();//退出
	void View_Block();//查看磁盘快使用情况
	void format();//格式化
	void stick_table();//粘贴

	//窗体通信
	void CloseFileDialog(QString filename);
	void receive_username(QString username);
};


