#include "UserManage.h"

UserManage::UserManage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
	connect(ui.B_add, SIGNAL(clicked()), this, SLOT(button_clicked()));
	connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(delete_user(const QModelIndex &)));
}

UserManage::~UserManage()
{

	
}

void UserManage::init()
{
	tablemodel = new QStandardItemModel(ui.tableView);
	ui.tableView->setEditTriggers(0);
	init_tableView();
}

void UserManage::init_tableView()
{
	tablemodel->clear();
	QStringList strHeader;
	strHeader << QStringLiteral("用户ID")<< QStringLiteral("用户名")<< QStringLiteral( "组名")<<QStringLiteral("密码")<<QStringLiteral("操作");
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tablemodel->setHorizontalHeaderLabels(strHeader);
	for (int i = 0; i < ACCOUNT_NUM; i++) {
		if (users.userName[i][0] != 0)
		{
			tablemodel->setItem(i, 0, new QStandardItem(QString::number(users.userID[i], 10)));
			tablemodel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit(users.userName[i])));
			tablemodel->setItem(i, 2, new QStandardItem(QString::number(users.groupID[i], 10)));
			tablemodel->setItem(i, 3, new QStandardItem(QString::fromLocal8Bit(users.password[i])));
			tablemodel->setItem(i, 4, new QStandardItem(QStringLiteral("删除")));
			printf("%d\t%s\t%s\t%d\n", users.userID[i], users.userName[i],users.password[i], users.groupID[i]);
		}
		else break;
	}
	ui.tableView->setModel(tablemodel);
}
void UserManage::delete_user(const QModelIndex & index)
{

	if (index.isValid()) {
		QString cellText = index.data().toString();
		QString str = QString::fromLocal8Bit("删除");
		if (cellText == str) {
			int tmp = tablemodel->index(index.row(), 0).data().toInt();
			for (int j = tmp; j < ACCOUNT_NUM - 1; j++) {
				strcpy(users.userName[j], users.userName[j + 1]);
				strcpy(users.password[j], users.password[j + 1]);
				users.groupID[j] = users.groupID[j + 1];
			}
			fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
			fwrite(&users, sizeof(userPsw), 1, fd);
			init_tableView();
		}
	}

}
void UserManage::button_clicked() {
	int i;
	for (i = 0; i < ACCOUNT_NUM; i++) {
		if (users.userName[i][0] == 0) break;
	}
	if (i == ACCOUNT_NUM) {
		return;
	}

	QString username = ui.L_user->text();
	QByteArray ba1 = username.toLatin1();
	char *user = ba1.data();
	QString password = ui.L_password->text();
	QByteArray ba2 = password.toLatin1();
	char *pass = ba2.data();
	if (strlen(user) > USER_NAME_LENGTH ||strlen(user)<=0) {
		return;
	}
	if (strlen(pass) > USER_PASSWORD_LENGTH || strlen(pass) <= 0) {
		return;
	}
	int t = ui.comboBox->currentText().toInt();
	cout << t << endl;
	for (i = 0; i < ACCOUNT_NUM; i++) {
		if (strcmp(users.userName[i], user) == 0) {
			return;
		}
		if (users.userName[i][0] == 0) {
			strcpy(users.userName[i], user);
			strcpy(users.password[i], pass);
			if (t > 0) {
				users.groupID[i] = t;
				for (int j = 0; j < ACCOUNT_NUM; j++) {
					cout << j << "  " << users.userName[j] << "  " << users.password[j] << endl;
				}
				fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
				fwrite(&users, sizeof(userPsw), 1, fd);
				init_tableView();
				return;	
			}
			else {
				strcpy(users.userName[i], 0);
				strcpy(users.password[i], 0);
			}
			break;
		}
	}
	init_tableView();
}