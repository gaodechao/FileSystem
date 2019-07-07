#include "ViewBlock.h"

ViewBlock::ViewBlock(QWidget *parent)
	: QWidget(parent)
{
	cout << "ViewBlock1 setup UI ViewBlock" << endl;
	
	ui.setupUi(this);
	init();
	
}

void ViewBlock::init()
{
	cout<<"init" << endl;
	sfModel = new QStringListModel(ui.listView);
	inodeModel = new QStandardItemModel(ui.inodeView);
	blockModel = new QStandardItemModel(ui.blockView);
	init_SF();
	init_Inode();
	init_Block();
}

void ViewBlock::init_SF()
{
	ui.listView->setEditTriggers(0);
	QStringList strlist;
	strlist.append(QStringLiteral("inode总数:		     ")+QString::number(superBlock.s_num_inode,10));
	strlist.append(QStringLiteral("空闲inode总数:		 ") + QString::number(superBlock.s_num_finode, 10));
	strlist.append(QStringLiteral("inode 大小 :			 ") + QString::number(superBlock.s_size_inode, 10));
	strlist.append(QStringLiteral("block总数:			 ") + QString::number(superBlock.s_num_block, 10));
	strlist.append(QStringLiteral("空闲block总数:		 ") + QString::number(superBlock.s_num_fblock, 10));
	strlist.append(QStringLiteral("block大小:		     ") + QString::number(superBlock.s_size_block, 10));
	strlist.append(QStringLiteral("超级栈数目:			 ") + QString::number(superBlock.special_free, 10));
	sfModel->setStringList(strlist);
	ui.listView->setModel(sfModel);
}

void ViewBlock::init_Inode()
{
	cout << "init_Inode" << endl;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			inodeModel->setItem(i,j, new QStandardItem(QString::number(i*16+j+1, 10)));
			if (inode_bitmap[i * 16 + j] == 1) {
				inodeModel->item(i,j)->setBackground(QBrush(QColor(255, 0, 0)));
			}
			else {
				inodeModel->item(i, j)->setBackground(QBrush(QColor(0, 255, 0)));
			}
			inodeModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
	ui.inodeView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.inodeView->setModel(inodeModel);
	ui.inodeView->verticalHeader()->hide();
	ui.inodeView->horizontalHeader()->hide();
	ui.inodeView->setEditTriggers(0);
	cout << "finished" << endl;
}

void ViewBlock::init_Block()
{
	if (!read_blockmap()) {
		QMessageBox::about(this, QStringLiteral("错误"), QStringLiteral("Block初始化未成功"));
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 32; j++) {
			blockModel->setItem(i, j, new QStandardItem(QString::number(i * 32 + j + 1, 10)));

			if (bm[i][j] == 1) {
				blockModel->item(i, j)->setBackground(QBrush(QColor(255, 0, 0)));

			}
			else {
				blockModel->item(i, j)->setBackground(QBrush(QColor(0, 255, 0)));
			}
			blockModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
	ui.blockView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.blockView->setModel(blockModel);
	ui.blockView->verticalHeader()->hide();
	ui.blockView->horizontalHeader()->hide();
	ui.blockView->setEditTriggers(0);
}

bool ViewBlock::read_blockmap()
{
	FILE* fbm = fopen("./blockmap.txt", "r+");
	if (fbm == NULL)
	{
		return false;
	}
	fread(bm, sizeof(int) * 512, 1, fbm);
	fclose(fbm);
	return true;
}

ViewBlock::~ViewBlock()
{
}
