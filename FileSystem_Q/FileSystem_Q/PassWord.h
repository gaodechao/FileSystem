#pragma once

#include <QWidget>
#include "ui_PassWord.h"
#include "stdafx.h"
#include <QMessageBox>

class PassWord : public QWidget
{
	Q_OBJECT

public:
	PassWord(QWidget *parent = Q_NULLPTR);
	~PassWord();

private:
	Ui::PassWord ui;
private slots:
	void B_check_clicked();
	void B_cancle_clicked();
};
