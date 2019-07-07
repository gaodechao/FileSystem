#include "PassWord.h"

PassWord::PassWord(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.B_check, SIGNAL(clicked()), this, SLOT(B_check_clicked()));
	connect(ui.B_cancle, SIGNAL(clicked()), this, SLOT(B_cancle_clicked()));
}

PassWord::~PassWord()
{

}
void PassWord::B_cancle_clicked() {
	int choose = QMessageBox::question(this, QStringLiteral("退出"),
		QString(QStringLiteral("确认退出程序?")),
		QMessageBox::Yes | QMessageBox::No);

	if (choose == QMessageBox::No)
	{
		return;
	}
	else if (choose == QMessageBox::Yes)
	{
		this->close();
	}
	
}
void PassWord::B_check_clicked() {

	cout << "B_cancle_clicked" << endl;
	QString oldpassword = ui.L_oldPass->text();
	QString newpassword = ui.L_newPass->text();
	QByteArray ba1 = oldpassword.toLatin1();
	QByteArray ba2 = newpassword.toLatin1();
	char *oldpass = ba1.data();
	char *newpass = ba2.data();
	if (strlen(newpass) > USER_PASSWORD_LENGTH) {
		QMessageBox::warning(NULL, QStringLiteral("Login"), QString::fromLocal8Bit("新密码太长，超过14个字节"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else {
		Msg msg = Passwd(oldpass, newpass);
		if (msg.flag) {
			QMessageBox::about(this, QStringLiteral("修改密码"), QStringLiteral("修改成功"));
		}
		else {
			QMessageBox::about(this, QStringLiteral("修改密码"), QString::fromLocal8Bit(msg.content.c_str()));
		}
	}
}
