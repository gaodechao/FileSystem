#include "UserLogin.h"

UserLogin::UserLogin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.B_login, SIGNAL(clicked()), this, SLOT(B_login_clicked()));
	connect(ui.B_cancle, SIGNAL(clicked()), this, SLOT(B_cancle_clicked()));
}

void UserLogin::Logout()
{
	//remove user's states
	userID = ACCOUNT_NUM;
	memset(userName, 0, sizeof(userName));
	memset(userName, 0, 6 + USER_NAME_LENGTH);
	//Mount();
}

UserLogin::~UserLogin()
{

}

Msg UserLogin::Login(const char * user, const char * password)
{
	Msg msg;
	//parameters check
	if (user == NULL || password == NULL)
	{
		msg.content = "Error: User name or password illegal!\n\n";
		msg.flag = false;
		return msg;
	}
	if (strlen(user) > USER_NAME_LENGTH || strlen(password) > USER_PASSWORD_LENGTH)
	{
		msg.content = "Error: User name or password illegal!\n";
		msg.flag = false;
		return msg;
	}
	//search the user in accouting file
	for (int i = 0; i < ACCOUNT_NUM; i++)
	{
		cout << i << "  " << users.userName[i] << "  " << users.password[i] << endl;
		if (strcmp(users.userName[i], user) == 0)
		{
			//find the user and check password
			if (strcmp(users.password[i], password) == 0)
			{
				//Login successfully
				msg.flag = true;
				msg.content = "OK";
				userID = users.userID[i];
				//make user's name, root user is special
				memset(userName, 0, USER_NAME_LENGTH + 6);
				if (userID == 0)
				{
					strcat(userName, "root");
					strcat(userName, users.userName[i]);
					strcat(userName, "$");
				}
				
				else
				{
					strcat(userName, users.userName[i]);
					strcat(userName, "#");
				}
				return msg;
			}
			else
			{
				//Password wrong
				msg.content = "Login failed: Wrong password.\n";
				msg.flag = false;
				return msg;
			}
		}
	}


	//User not found
	msg.content = "Login failed: User not found.\n";
	msg.flag = false;
	return msg;
}

void UserLogin::B_cancle_clicked()
{
	this->close();
}

void UserLogin::B_login_clicked() {

	for (int j = 0; j < ACCOUNT_NUM; j++) {
		cout << j << "  " << users.userName[j] << "  " << users.password[j] << endl;
	}

	Logout();

	for (int j = 0; j < ACCOUNT_NUM; j++) {
		cout << j << "  " << users.userName[j] << "  " << users.password[j] << endl;
	}
	QString username = ui.L_user->text();
	QByteArray ba1 = username.toLatin1();
	
	char *user = ba1.data();

	QString password = ui.L_password->text();
	QByteArray ba2 = password.toLatin1();
	char *pass = ba2.data();

	Msg msg=Login(user,pass);
	if (!msg.flag) {
		QMessageBox::warning(NULL, QStringLiteral("Login"),QString::fromLocal8Bit(msg.content.c_str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else {
		QMessageBox::warning(this, QStringLiteral("Login"), QStringLiteral("µÇÂ¼³É¹¦"));
		emit send_username(username);
		this->close();
	}
}
