#pragma once

#include<QMessageBox>
#include <QWidget>
#include "ui_UserLogin.h"

#include "stdafx.h"

class UserLogin : public QWidget
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	void Logout();
	~UserLogin();
	Msg Login(const char* user, const char* password);

private:
	Ui::UserLogin ui;
private slots:
	void B_login_clicked();
	void B_cancle_clicked();

public: signals:

	void send_username(QString username);
};
