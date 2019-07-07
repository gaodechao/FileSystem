#include "FileSystem_Q.h"
#include<time.h>


FileSystem_Q::FileSystem_Q(QWidget *parent)
	: QMainWindow(parent)
{

	
	ui.setupUi(this);
	
	m_publicIconMap[QStringLiteral("Diss")] = QIcon(QStringLiteral("./Resources/Icon/diss.png"));
	m_publicIconMap[QStringLiteral("Folder")] = QIcon(QStringLiteral("./Resources/Icon/folder.png"));
	m_publicIconMap[QStringLiteral("File")] = QIcon(QStringLiteral("./Resources/Icon/file.png"));
	m_publicIconMap[QStringLiteral("check")] = QIcon(QStringLiteral("./Resources/Icon/check.png"));
	m_publicIconMap[QStringLiteral("check_not")] = QIcon(QStringLiteral("./Resources/Icon/check_not.png"));
	init_fileMenu();
	creatItemMenu();
	init_File();
	init_tableView();
	init_connect();
	Sleep(2);
	login = new UserLogin();
	login->show();
	connect(login, SIGNAL(send_username(QString)), this, SLOT(receive_username(QString)));
}

void FileSystem_Q::receiveUser(string username) {

	time_t t = time(0);
	struct tm *p;
	p = gmtime(&t);
	char s[100];
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
	ui.L_Wellcom->setText(QString::fromLocal8Bit(("��ӭ," + username + "  " + s).c_str()));
	init_View_data();
}

void FileSystem_Q::init_connect()
{
	connect(ui.treeView, SIGNAL(doubleClicked(const QModelIndex)), SLOT(openItem_tree()));
	connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
	connect(ui.inode_table, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(AccessChange(const QModelIndex &)));
}

QString FileSystem_Q::getCurrentItem()
{
	QModelIndex index = ui.treeView->currentIndex();
	return  ui.treeView->model()->data(index).toString();
}

QString FileSystem_Q::getParentItem()
{
	QModelIndex index = ui.treeView->currentIndex();
	return index.parent().data().toString();
}

QString FileSystem_Q::getParentItem(const QModelIndex & index)
{
	return index.parent().data().toString();
}

void FileSystem_Q::init_tableView()
{
	init_tableView_style();
	tablemodel = new QStandardItemModel(ui.tableView);
	init_View_data();
	ui.tableView->setModel(tablemodel);
}

void FileSystem_Q::init_View_data()
{
	tablemodel->clear();
	ui.lineEdit->setText(QStringLiteral("%1").arg(Ab_dir().c_str()));
	tablemodel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("�ļ���") << QStringLiteral("����") << QStringLiteral("��С") << QStringLiteral("����ʱ��"));
	for (int i = 0; i < DIRECTORY_NUM; i++) {
		if (strlen(currentDirectory.fileName[i]) > 0)
		{
			inode tmp_inode;
			fseek(fd, INODE_START + currentDirectory.inodeID[i] * INODE_SIZE, SEEK_SET);
			fread(&tmp_inode, sizeof(inode), 1, fd);
			//show_inode1(tmp_inode, "tem_inode");
			QList<QStandardItem *> items;
			if (tmp_inode.di_mode == 0) {
				QStandardItem *item1 = new QStandardItem(m_publicIconMap[QStringLiteral("Folder")], QString::fromLocal8Bit(currentDirectory.fileName[i]));
				items.append(item1);

				QStandardItem *item2 = new QStandardItem(QStringLiteral("�ļ���"));
				items.append(item2);
			}
			else if (tmp_inode.di_mode == 1) {
				QStandardItem *item1 = new QStandardItem(m_publicIconMap[QStringLiteral("File")], QString::fromLocal8Bit(currentDirectory.fileName[i]));
				items.append(item1);
				QStandardItem *item2 = new QStandardItem(QStringLiteral("�ļ�"));
				items.append(item2);
			}
			QStandardItem *item3 = new QStandardItem(QStringLiteral("%1").arg(tmp_inode.di_size));
			items.append(item3);
			QStandardItem *item4 = new QStandardItem(QStringLiteral("%1").arg(tmp_inode.time));
			items.append(item4);
			tablemodel->appendRow(items);
		}
	}
}

void FileSystem_Q::init_tableView_style()
{
	ui.tableView->verticalHeader()->hide();
	ui.tableView->setShowGrid(false);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ģʽΪѡ����
	ui.tableView->setFocusPolicy(Qt::NoFocus);
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::QHeaderView::Stretch);
	ui.tableView->installEventFilter(this);
}

void FileSystem_Q::init_fileMenu()
{
	QMenu *fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("�ļ�"));

	QAction *Action_build = new QAction(QString::fromLocal8Bit("�½��ļ���"));
	fileMenu->addAction(Action_build);
	connect(Action_build, SIGNAL(triggered()), this, SLOT(build_folder_table()));

	QAction *Action_build_file = new QAction(QString::fromLocal8Bit("�½��ļ�"));
	fileMenu->addAction(Action_build_file);
	connect(Action_build_file, SIGNAL(triggered()), this, SLOT(build_file_table()));

	QAction *Action_format = new QAction(QStringLiteral("��ʽ��"));
	fileMenu->addAction(Action_format);
	connect(Action_format, SIGNAL(triggered()), this, SLOT(format()));

	QAction * Action_newLogin = new QAction(QStringLiteral("��¼���û�"));
	fileMenu->addAction(Action_newLogin);
	connect(Action_newLogin, SIGNAL(triggered()),this,SLOT(newLogin()));

	QAction *  passwd= new QAction(QStringLiteral("��������"));
	fileMenu->addAction(passwd);
	connect(passwd, SIGNAL(triggered()), this, SLOT(PasswordChange()));

	QAction * User_manager = new QAction(QStringLiteral("�û�����"));
	fileMenu->addAction(User_manager);
	connect(User_manager, SIGNAL(triggered()), this, SLOT(manage()));

	QAction *Action_Exit = new QAction(QStringLiteral("�˳���¼"));
	fileMenu->addAction(Action_Exit);
	connect(Action_Exit, SIGNAL(triggered()), this, SLOT(Exit()));

	QMenu *ViewMenu = menuBar()->addMenu(QString::fromLocal8Bit("�鿴"));
	QAction *action_view_block = new QAction(QStringLiteral("�鿴�ռ�ʹ�����"));
	ViewMenu->addAction(action_view_block);
	connect(action_view_block, SIGNAL(triggered()), this, SLOT(View_Block()));

}

void FileSystem_Q::file_Ergodic(QStandardItem *item,directory dir)
{
	for (int i = 2; i < DIRECTORY_NUM; i++) {
		if (strlen(dir.fileName[i])>0)
		{
			cout << dir.fileName[i] << "  " << dir.inodeID[i] << "   " ;
			inode tmp_inode;
			fseek(fd, INODE_START + dir.inodeID[i] * INODE_SIZE, SEEK_SET);
			fread(&tmp_inode, sizeof(inode), 1, fd);
			cout << tmp_inode.di_mode << endl;
			if (tmp_inode.di_mode == 0) {
				QStandardItem *item_child_folder = new QStandardItem(m_publicIconMap[QStringLiteral("Folder")], QString::fromLocal8Bit(string(dir.fileName[i]).c_str()));
				item->appendRow(item_child_folder);
				file_index_inode[item_child_folder->index()] = dir.inodeID[i];
				directory new_current_dir;
				fseek(fd, DATA_START + tmp_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
				fread(&new_current_dir, sizeof(directory), 1, fd);
				file_Ergodic(item_child_folder, new_current_dir);
			}
			else if (tmp_inode.di_mode == 1) {
				QStandardItem *item_child_file = new QStandardItem(m_publicIconMap[QStringLiteral("File")], QString::fromLocal8Bit(string(dir.fileName[i]).c_str()));
				item->appendRow(item_child_file);
				file_index_inode[item_child_file->index()] = dir.inodeID[i];
			}
		}
	}
}

void FileSystem_Q::upData_ab_dir()
{
	//��õ�ǰ����
	QModelIndex index = ui.treeView->currentIndex();
	QString str = ui.treeView->model()->data(index).toString();
	stack<QString> tmp_ab_dir;
	tmp_ab_dir.push(str);
	while (index.parent().isValid()) {
		index = index.parent();
		str= ui.treeView->model()->data(index).toString();
		tmp_ab_dir.push(str);
	}
	cout << endl;
	dir_pointer = 0;
	while (!tmp_ab_dir.empty()) {
		QString flodername = tmp_ab_dir.top();
		tmp_ab_dir.pop();
		QByteArray ba = flodername.toLatin1(); // must
		char *filename = ba.data();
		for (int i = 0; i < 14; i++) {
			ab_dir[dir_pointer][i] = filename[i];
		}
		dir_pointer++;
	}
	cout << endl;
}

void FileSystem_Q::open_Inode(int inodeID)
{
	inode tmp_inode;
	fseek(fd, INODE_START + inodeID * INODE_SIZE, SEEK_SET);
	fread(&tmp_inode, sizeof(inode), 1, fd);
	if (tmp_inode.di_mode == 0) {
		directory new_current_dir;
		fseek(fd, DATA_START + tmp_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fread(&new_current_dir, sizeof(directory), 1, fd);
		currentDirectory = new_current_dir;
		upData_ab_dir();
	}
	else
	{
		QByteArray ba = getCurrentItem().toLatin1(); // must
		char *filename = ba.data();
		open_file(filename);
	}
}

void FileSystem_Q::open_Item(const QModelIndex & index)
{
	if (index.isValid()) {
		QString cellText = index.data().toString();
		QByteArray ba = cellText.toLatin1(); // must
		char *filename = ba.data();
		QString type = tablemodel->index(ui.tableView->currentIndex().row(), 1).data().toString();
;
		if (type==QString::fromLocal8Bit("�ļ���")) {
			Msg tag = OpenDir(filename);
			if (tag.flag) {
				init_View_data();
				init_File();
				return;
			}
			else {
				QMessageBox::warning(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(tag.content.c_str()));
			}
		}
		else {
			cout << "���ļ�" << filename << endl;
			open_file(filename);
		}
	}
}

void FileSystem_Q::open_file(char * filename)
{
	inode currentInode;
	currentInode=OpenFile(filename);
	Msg msg= PrintFile(currentInode);
	if (!msg.flag) {
		QMessageBox::warning(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(msg.content.c_str()));
		return;
	}
	QString str = QString::fromStdString(msg.content);
	FileDialog *file_dialog =new FileDialog(str,filename,currentInode);
	connect(file_dialog, SIGNAL(ExitWin(QString)), this, SLOT(CloseFileDialog(QString)));
	file_dialog->show();
	init_View_data();
}

bool FileSystem_Q::eventFilter(QObject * obj, QEvent * evt)
{
	if (obj == ui.tableView  &&  evt->type() == QEvent::ContextMenu)
	{
		if (ui.tableView->currentIndex().isValid() == true)
		{
			tablemenu->exec(cursor().pos());           //�ڵ�ǰ���λ�������в˵�menu����
		}
	}
	return QWidget::eventFilter(obj, evt);
}

bool FileSystem_Q::OpenfileManage(QString filename)
{
	QString ab_str_file = QString::fromStdString(Ab_dir()) + filename;
	if (OpenFileList.contains(ab_str_file)) {
		QMessageBox::about(this, QStringLiteral("��ʾ"), QStringLiteral("��ǰ�ļ��Դ�"));
	}
	return false;
}

void FileSystem_Q::show_Inode(QModelIndex & index)
{
	inodeModel->clear();
	if (index.isValid()) {
		QString cellText = index.data().toString();
		this->current_filename = cellText;
		QByteArray ba = cellText.toLatin1(); // must
		char *filename = ba.data();
		int inodeID = get_current_Inode(filename);
		cout << "clicked   " << filename << endl;
		cout << "inodeID" << inodeID << endl;
		inode tmp_inode;
		fseek(fd, INODE_START + inodeID * INODE_SIZE, SEEK_SET);
		fread(&tmp_inode, sizeof(inode), 1, fd);
		this->current_inode_table = tmp_inode;
		inodeModel->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit(filename) << QStringLiteral("�����Ϣ"));
		inodeModel->setItem(0, 0, new QStandardItem(QStringLiteral("�����")));
		inodeModel->setItem(0, 1, new QStandardItem(QString::number(inodeID, 10)));
		inodeModel->setItem(1, 0, new QStandardItem(QStringLiteral("�ļ�����")));
		if (tmp_inode.di_mode == 0) {
			inodeModel->setItem(1, 1, new QStandardItem(QStringLiteral("�ļ���")));
		}
		else {
			inodeModel->setItem(1, 1, new QStandardItem(QStringLiteral("�ļ�")));
		}
		inodeModel->setItem(2, 0, new QStandardItem(QStringLiteral("Ȩ�޿���")));

		inodeModel->setItem(3, 0, new QStandardItem(QStringLiteral("�ɶ�")));
		if (ACCESS_check("OWN_R", tmp_inode.permission)) {
			inodeModel->setItem(3, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(3, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(4, 0, new QStandardItem(QStringLiteral("��д")));
		
		if (ACCESS_check("OWN_W", tmp_inode.permission)) {
			inodeModel->setItem(4, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(4, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(5, 0, new QStandardItem(QStringLiteral("�ɲ���")));
		if (ACCESS_check("OWN_E", tmp_inode.permission)) {
			inodeModel->setItem(5, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(5, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}

		inodeModel->setItem(6, 0, new QStandardItem(QStringLiteral("��ɶ�")));
		if (ACCESS_check("GRP_R", tmp_inode.permission)) {
			inodeModel->setItem(6, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(6, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(7, 0, new QStandardItem(QStringLiteral("���д")));
		if (ACCESS_check("GRP_W", tmp_inode.permission)) {
			inodeModel->setItem(7, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
			
		}
		else {
			inodeModel->setItem(7, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
			
		}
		inodeModel->setItem(8, 0, new QStandardItem(QStringLiteral("��ɲ���")));
		if (ACCESS_check("GRP_E", tmp_inode.permission)) {
			inodeModel->setItem(8, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		} 
		else{
			inodeModel->setItem(8, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(9, 0, new QStandardItem(QStringLiteral("�����˿ɶ�")));
		if (ACCESS_check("ELSE_R", tmp_inode.permission)) {
			inodeModel->setItem(9, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(9, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(10, 0, new QStandardItem(QStringLiteral("�����˿�д")));
		
		if (ACCESS_check("ELSE_W", tmp_inode.permission)) {
			inodeModel->setItem(10, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(10, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(11, 0, new QStandardItem(QStringLiteral("�����˿ɲ���")));
		if (ACCESS_check("ELSE_E", tmp_inode.permission)) {
			inodeModel->setItem(11, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
		}
		else {
			inodeModel->setItem(11, 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
		}
		inodeModel->setItem(12, 0, new QStandardItem(QStringLiteral("������")));
		inodeModel->setItem(12, 1, new QStandardItem(QString::fromLocal8Bit(users.userName[tmp_inode.di_uid])));
		inodeModel->setItem(13, 0, new QStandardItem(QStringLiteral("������")));
		inodeModel->setItem(13, 1, new QStandardItem(QString::number(tmp_inode.di_grp, 10)));
		inodeModel->setItem(14, 0, new QStandardItem(QStringLiteral("�ļ���С")));
		inodeModel->setItem(14, 1, new QStandardItem(QString::number(tmp_inode.di_size, 10)));
		inodeModel->setItem(15, 0, new QStandardItem(QStringLiteral("����ʱ��")));
		inodeModel->setItem(15, 1, new QStandardItem(QString::fromLocal8Bit(tmp_inode.time)));
	}
	ui.inode_table->setFocusPolicy(Qt::NoFocus);
	ui.inode_table->installEventFilter(this);
	ui.inode_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.inode_table->setModel(inodeModel);
	ui.inode_table->horizontalHeader()->setSectionResizeMode(QHeaderView::QHeaderView::Stretch);
}

void FileSystem_Q::init_File() {

	ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	treemodel = new QStandardItemModel(ui.treeView);
	treemodel->clear();

	treemodel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("�ļ���"));
	ui.treeView->setEditTriggers(0);
	ui.treeView->setModel(treemodel);
	file_index_inode.clear();

	QStandardItem *itemDiss = new QStandardItem(m_publicIconMap[QStringLiteral("Diss")], QStringLiteral("root"));
	treemodel->appendRow(itemDiss);
	fseek(fd, DATA_START, SEEK_SET);
	fread(&rootDirectory, sizeof(directory), 1, fd);
	file_Ergodic(itemDiss,rootDirectory);//�ļ�ϵͳ�ı���
	
}

void FileSystem_Q::creatItemMenu()
{
	tablemenu = new QMenu(this);
	tablemenu->addAction(QStringLiteral("���ļ�"), this, SLOT(openItem_table()));
	tablemenu->addAction(QStringLiteral("����"), this, SLOT(copy_table()));
	tablemenu->addAction(QStringLiteral("����"), this, SLOT(cut_table()));
	tablemenu->addAction(QStringLiteral("ճ��"), this, SLOT(stick_table()));
	tablemenu->addAction(QStringLiteral("�½��ļ���"), this, SLOT(build_folder_table()));
	tablemenu->addAction(QStringLiteral("�½��ļ�"), this, SLOT(build_file_table()));
	tablemenu->addAction(QStringLiteral("������"), this, SLOT(rename_table()));
	tablemenu->addAction(QStringLiteral("ɾ��"), this, SLOT(delete_table()));
	tablemenu->addAction(QStringLiteral("����"), this, SLOT(View_Property()));
}

void FileSystem_Q::onTableClicked(const QModelIndex &rowindex)
{
	QModelIndex index = tablemodel->index(rowindex.row(), 0);//ѡ���е�һ�е�����
	open_Item(index);
}

void FileSystem_Q::AccessChange(const QModelIndex & rowindex)
{
	QModelIndex index = inodeModel->index(rowindex.row(), 0);//ѡ���е�һ�е�����
	QString rowdata = rowindex.data().toString();
	QByteArray ba1 = this->current_filename.toLatin1();
	char *op_filename = ba1.data();
	if (index.isValid()) {
		QString firststr=index.data().toString();
		qDebug() << firststr << endl;
		int filetype = 0;
		if (firststr == QString::fromLocal8Bit("�ɶ�") || firststr == QString::fromLocal8Bit("��д")||firststr==QString::fromLocal8Bit("�ɲ���")
			||firststr == QString::fromLocal8Bit("��ɶ�")||firststr == QString::fromLocal8Bit("���д")|| firststr == QString::fromLocal8Bit("��ɲ���")
			|| firststr == QString::fromLocal8Bit("�����˿ɶ�")||firststr == QString::fromLocal8Bit("�����˿�д")||firststr==QString::fromLocal8Bit("�����˿ɲ���")) {
			
			qDebug() << rowdata;
			if (rowdata == QString::fromLocal8Bit("Y")) {
				inodeModel->setItem(rowindex.row(), 1, new QStandardItem(m_publicIconMap[QStringLiteral("check_not")], QStringLiteral("N")));
			}
			else {
				inodeModel->setItem(rowindex.row(), 1, new QStandardItem(m_publicIconMap[QStringLiteral("check")], QStringLiteral("Y")));
			}

			int temp = 0;
			for (int i = 0,j=3; i < 8; i++,j++) {
				QString tag = inodeModel->index(j, 1).data().toString();
				qDebug()<<"i="<<i<<"  " << "j=" << j << "  " << tag << endl;
				if (tag == QString::fromLocal8Bit("Y")) {
					temp += 1 << (8 - i);
				}
			}
			QString tag = inodeModel->index(11, 1).data().toString();
			if(tag==QString::fromLocal8Bit("Y"))
				temp += 1;
			int tt = 0;
			if (this->current_inode_table.di_mode == 0) {
				tt = 1;
			}
			string tag1 = Access_1_Check(current_inode_table, "Change premission");
			if (tag1 != "OK") {
				QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit(tag1.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			}
			Msg msg = Chmod(op_filename, tt, temp);
			if (!msg.flag) {
				QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit(msg.content.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			}

		}
		if (firststr == QString::fromLocal8Bit("������")) {
			QInputDialog dia(this);
			dia.setWindowTitle("build folder:");
			dia.setLabelText(QStringLiteral("����������������"));
			dia.setInputMode(QInputDialog::TextInput);//��ѡ������DoubleInput  TextInput
			if (dia.exec() == QInputDialog::Accepted)
			{
				QString input = dia.textValue();
				QByteArray ba = input.toLatin1(); // must
				char *username = ba.data();
				if (strlen(username) > USER_NAME_LENGTH || strlen(username) <= 0) {
					QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit("�����û���̫����Ϊ��"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
					return;
				}
				int tt = 0;
				if (this->current_inode_table.di_mode == 0) {
					tt = 1;
				}
				string tag1 = Access_1_Check(current_inode_table, "Change owner");
				if (tag1 != "OK") {
					QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit(tag1.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				}
				Msg msg=Chown(op_filename,tt,username);
				if (!msg.flag) {
					QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit(msg.content.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
					return;
				}
			}
		}
		if (firststr == QString::fromLocal8Bit("������")) {
			QInputDialog dia(this);
			dia.setWindowTitle("build folder:");
			dia.setLabelText(QStringLiteral("������������"));
			dia.setInputMode(QInputDialog::IntInput);//��ѡ������DoubleInput  TextInput
			if (dia.exec() == QInputDialog::Accepted)
			{
				int input = dia.textValue().toInt();
				cout << input << endl;
				if (input<=0 || input>=10) {
					QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit("��ǰ�鲻����"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				}
				int tt = 0;
				if (this->current_inode_table.di_mode == 0) {
					tt = 1;
				}
				string tag1 = Access_1_Check(current_inode_table, "Change group");
				if (tag1 != "OK") {
					QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit(tag1.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				}
				Msg msg = Chgrp(op_filename, tt, input);
				if (!msg.flag) {
					QMessageBox::warning(NULL, QStringLiteral("����"), QString::fromLocal8Bit(msg.content.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				}
			}
		}
		show_Inode(inode_index);
	}
}


void FileSystem_Q::openItem_tree()
{
	int inodeID = file_index_inode[ui.treeView->currentIndex()];
	open_Inode(inodeID);
	
	init_View_data();
}

void FileSystem_Q::openItem_table()
{
	QModelIndex index = tablemodel->index(ui.tableView->currentIndex().row(), 0);//ѡ���е�һ�е�����
	open_Item(index);
}

void FileSystem_Q::copy_table()
{	
	QModelIndex index = tablemodel->index(ui.tableView->currentIndex().row(), 0);//ѡ���е�һ�е�����
	if (index.isValid()) {
		QString str = index.data().toString();
		qDebug() << str << endl;
		QByteArray ba = str.toLatin1(); // must
		char *copy = ba.data();
		int inodeID = get_current_Inode(copy);
		fseek(fd, INODE_START + inodeID * INODE_SIZE, SEEK_SET);
		fread(&this->copyInode, sizeof(inode), 1, fd);
		if (copyInode.di_mode == 0) {
			copyDir = cpdir(copy);	
			copydirflag = true;
			copyflag = false;
		}
		else {
			this->copyfile = str;
			copyflag = true;
			copydirflag = false;
		}
	}
}
void FileSystem_Q::cut_table()
{
	QModelIndex index = tablemodel->index(ui.tableView->currentIndex().row(), 0);//ѡ���е�һ�е�����
	if (index.isValid()) {
		QString str = index.data().toString();
		qDebug() << str << endl;
		QByteArray ba = str.toLatin1(); // must
		char *copy = ba.data();
		int inodeID = get_current_Inode(copy);
		fseek(fd, INODE_START + inodeID * INODE_SIZE, SEEK_SET);
		fread(&this->copyInode, sizeof(inode), 1, fd);
		if (copyInode.di_mode == 0) {
			copyDir = cpdir(copy);
			copydirflag = true;
			copyflag = false;
			RemoveDir(copy);

		}
		else {
			this->copyfile = str;
			copyflag = true;
			copydirflag = false;
			DeleteFile(copy);
		}
	}
	init_View_data();

}

void FileSystem_Q::build_folder_table()
{
	QInputDialog dia(this);
	dia.setWindowTitle("build folder:");
	dia.setLabelText(QStringLiteral("���ļ�������"));
	dia.setInputMode(QInputDialog::TextInput);//��ѡ������DoubleInput  TextInput
	if (dia.exec() == QInputDialog::Accepted)
	{
		QString newname = dia.textValue();
		QByteArray ba = newname.toLatin1(); // must
		char *foldername = ba.data();
		QStandardItem *item_child_folder = new QStandardItem(m_publicIconMap[QStringLiteral("Folder")], QStringLiteral("%1").arg(foldername));
		Msg msg=MakeDir(foldername);
		if (!msg.flag) {
			QMessageBox::warning(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(msg.content.c_str()));
			return;

		}
		init_View_data();
		init_File();
	}
}

void FileSystem_Q::build_file_table()
{
	QInputDialog dia(this);
	dia.setWindowTitle("build folder:");
	dia.setLabelText(QStringLiteral("���ļ�����"));
	dia.setInputMode(QInputDialog::TextInput);//��ѡ������DoubleInput  TextInput
	if (dia.exec() == QInputDialog::Accepted)
	{

		QString newname = dia.textValue();
		QByteArray ba = newname.toLatin1(); // must
		char *filename = ba.data();
		QStandardItem *item_child_folder = new QStandardItem(m_publicIconMap[QStringLiteral("Folder")], QStringLiteral("%1").arg(filename));
		Msg tag=CreateFile(filename);
		if (tag.flag) {
			init_View_data();
			init_File();
		}
		else {
			QMessageBox::warning(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(tag.content.c_str()));
		}
	}

}

void FileSystem_Q::rename_table()
{
	QModelIndex index1 = tablemodel->index(ui.tableView->currentIndex().row(), 0);//ѡ���е�һ�е�����
	qDebug() << index1.data().toString();
	QString str = QString::fromLocal8Bit("�ļ���");
	QModelIndex index2 = tablemodel->index(ui.tableView->currentIndex().row(), 1);//ѡ���е�һ�е�����
	qDebug() << index2.data().toString();
	if (index2.data().toString() == str) {
		QString cellText = index1.data().toString();
		QByteArray ba = cellText.toLatin1(); // must
		char *oldname = ba.data();
		QInputDialog dia(this);
		dia.setWindowTitle(QString::fromLocal8Bit("������"));
		dia.setLabelText(QString::fromLocal8Bit("ԭ�ļ���:") + QString::fromLocal8Bit(oldname));
		dia.setInputMode(QInputDialog::TextInput);//��ѡ������DoubleInput  TextInput
		if (dia.exec() == QInputDialog::Accepted)
		{
			QString str = dia.textValue();
			QByteArray ba = str.toLatin1(); // must
			char *newname = ba.data();
			if (strcmp(oldname, newname) != 0) {
				Rename(oldname, newname, 1);
			}
		}
	}
	else {
		QString cellText = index1.data().toString();
		QByteArray ba = cellText.toLatin1(); // must
		char *oldfilename = ba.data();
		QInputDialog dia(this);
		dia.setWindowTitle(QString::fromLocal8Bit("������"));
		dia.setLabelText(QString::fromLocal8Bit("ԭ�ļ�:")+QString::fromLocal8Bit(oldfilename));
		dia.setInputMode(QInputDialog::TextInput);//��ѡ������DoubleInput  TextInput
		if (dia.exec() == QInputDialog::Accepted)
		{
			QString str=dia.textValue();
			QByteArray ba = str.toLatin1(); // must
			char *newfilename = ba.data();
			if (strcmp(oldfilename, newfilename) != 0) {
				Rename(oldfilename, newfilename, 0);
			}
		}
	}
	init_View_data();
	init_File();
}

void FileSystem_Q::delete_table()
{
	QModelIndex index1 = tablemodel->index(ui.tableView->currentIndex().row(), 0);//ѡ���е�һ�е�����
	qDebug() << index1.data().toString();
	QString str = QString::fromLocal8Bit("�ļ���");
	QModelIndex index2 = tablemodel->index(ui.tableView->currentIndex().row(), 1);//ѡ���е�һ�е�����
	qDebug() << index2.data().toString();
	if (index2.data().toString()==str) {
		QString cellText = index1.data().toString();
		QByteArray ba = cellText.toLatin1(); // must
		char *filename = ba.data();
		Msg msg= RemoveDir(filename);
		if (!msg.flag) {
			QMessageBox::warning(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(msg.content.c_str()));
			return;
		}
	}
	else {
		QString cellText = index1.data().toString();
		QByteArray ba = cellText.toLatin1(); // must
		char *filename = ba.data();
		Msg msg=DeleteFile(filename);
		if (!msg.flag) {
			QMessageBox::warning(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(msg.content.c_str()));
			return;
		}
	}
	init_View_data();
	init_File();
}

void FileSystem_Q::View_Property()
{
	inodeModel = new QStandardItemModel(ui.inode_table);
	QModelIndex index = tablemodel->index(ui.tableView->currentIndex().row(), 0);//ѡ���е�һ�е�����
	inode_index = index;
	show_Inode(index);
}

void FileSystem_Q::PasswordChange()
{
	passwd = new PassWord();
	passwd->show();
}

void FileSystem_Q::manage()
{
	if (userID != 0) {
		QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("ֻ�й���Աadmin �ſ����޸�Ȩ��"));
		return;
	}
	user_manage = new UserManage();
	user_manage->show();
}

void FileSystem_Q::newLogin()
{
	login = new UserLogin();
	connect(login, SIGNAL(send_username(QString)), this, SLOT(receive_username(QString)));
	login->show();
}

void FileSystem_Q::Exit()
{
	this->close();

}

void FileSystem_Q::View_Block()
{
	viewBlock = new ViewBlock();
	viewBlock->show();
}

void FileSystem_Q::format()
{
	Format();
	inti_main();
	init_File();
	init_View_data();
}

void FileSystem_Q::stick_table()
{
	if (copyflag) {
		string ab_path = Ab_arm_dir();
		ab_path.push_back('#');
		QByteArray ba1 = this->copyfile.toLatin1(); // must
		char *copy = ba1.data();
		QString str = QString::fromStdString(ab_path);
		QByteArray ba2 = str.toLatin1(); // must
		
		char *aimpath = ba2.data();
		Copy(copy, copyInode, aimpath);
	}
	if (copydirflag) {
		
		pastedir(copyDir);
	}
	init_View_data();
}

void FileSystem_Q::CloseFileDialog(QString filename)
{
	qDebug() << "close  " << filename << endl;
	init_View_data();
}

void FileSystem_Q::receive_username(QString username)
{
	time_t t = time(0);
	struct tm *p;
	p = gmtime(&t);
	char s[100];
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
	ui.L_Wellcom->setText("Welcome    " + username + "\n" + QStringLiteral("��¼ʱ��:")+QString::fromLocal8Bit(s));
	init_View_data();
}
 copydir FileSystem_Q::cpdir(char *dirname) {
	int pos;
	inode tmp_dir_inode;
	int tmp_dir_ino;
	do {
		for (pos = 2; pos < DIRECTORY_NUM; pos++)
		{
			if (strcmp(currentDirectory.fileName[pos], dirname) == 0) {

				break;
			}
		}
		tmp_dir_ino = currentDirectory.inodeID[pos];
		fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_dir_inode, sizeof(inode), 1, fd);
	} while (tmp_dir_inode.di_mode == 1);

	copydir* d = new  copydir;
	d->copynode = tmp_dir_inode;
	memset(d->fileName, 0, sizeof(d->fileName));
	strncpy(d->fileName, dirname, strlen(dirname));
	d->type = 0;
	makequeue(d);
	return *d;
}

void FileSystem_Q::pastedir(copydir bp)
{
	//���ڡ���ǰ��Ŀ¼��current_directory����ΪҪճ����Ŀ¼��ճ���㷨��
	 //���ڵ�ǰĿ¼����һ���ļ��а��Ǿ� bp==back _up ���ݵ���˼
	MakeDir(bp.fileName);
	OpenDir(bp.fileName);//�������ļ���֮�󣬽�����ļ���,�����ļ��к�current_directory�ᷢ���л�
	//����copyqueue
	
	for (int i=0; i<bp.copyqueue.size(); i++) {
		copydir temp =*( bp.copyqueue[i]);
		if (temp.type==1) {//��һ���ļ�
			string str = Ab_arm_dir();
			str.push_back('#');
			QString strr = QString::fromStdString(str);
			QByteArray ba1 = strr.toLatin1();
			char *filename = ba1.data();
			Copy(temp.fileName, temp.copynode,filename);//����������Ϊ Ab_arm_adr()+'#'
			
		}
		else {//�����ļ���ǰ��Ҫ����Ŀ¼�ļ�
			pastedir(temp);
		}
	}
	OpenDir("..");//����ԭ�����ļ���
}

void FileSystem_Q::makequeue(copydir *&d)
{

	//������ǰĿ¼�����������vector
	if (!d->type) {//Ŀ¼���Ҫmakequeue
		OpenDir(d->fileName);//��������Ŀ¼
		int pos;
		for (pos = 2; pos < DIRECTORY_NUM; pos++) {
			if (strlen(currentDirectory.fileName[pos]) > 0) {
				inode tnode;
				int temp = currentDirectory.inodeID[pos];//�����˵�ǰ���ļ����inode����
				fseek(fd, INODE_START + temp * INODE_SIZE, SEEK_SET);
				fread(&tnode, sizeof(inode), 1, fd);//������ǰ��i���
				copydir *t = new copydir;
				t->copynode = tnode;
				memset(t->fileName, 0, sizeof(t->fileName));
				strncpy(t->fileName, currentDirectory.fileName[pos], strlen(currentDirectory.fileName[pos]));
				if (tnode.di_mode) {
					t->type = 1;
				}
				else {//����һ��Ŀ¼���Ҫ����makequeue
					t->type = 0;
					makequeue(t);
				}
				(d->copyqueue).push_back(t);//���ýڵ�ѹ��queue

			}
			else break;
		}

		OpenDir("..");//���ص�ǰĿ¼���Ҫ�ı�Ŀ¼��.
	}
	else {//�ļ������makequeue
		return;
	}

}
