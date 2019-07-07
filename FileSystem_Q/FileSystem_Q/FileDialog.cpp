#include "FileDialog.h"

FileDialog::FileDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
FileDialog::FileDialog(QString content,char *filename,inode &currentInode,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->fileinode = currentInode;
	write_flag = true;
	string tag = Access_1_Check(currentInode, "Write");
	ui.L_mode->setText(QStringLiteral("写模式"));
	if (tag != "OK") {
		write_flag = false;
		ui.textEdit->setReadOnly(true);
		ui.L_mode->setText(QStringLiteral("只读模式"));
	}
	content.replace("\u0000", "");
	this->filename = QString::fromLocal8Bit(filename);
	ui.L_filename->setText(QString::fromLocal8Bit(filename));
	ui.textEdit->setText(content);
	show_size();
	connect(ui.B_cancle, SIGNAL(clicked()), this, SLOT(cancle()));
	connect(ui.B_save, SIGNAL(clicked()), this, SLOT(save()));
	connect(ui.B_save2, SIGNAL(clicked()), this, SLOT(save2()));
	connect(ui.textEdit, SIGNAL(textChanged()), this, SLOT(count()));
}

FileDialog::~FileDialog()
{
	QString str = ui.textEdit->toPlainText();
}
void FileDialog::closeEvent(QCloseEvent *)
{
	emit ExitWin(filename);
}
void FileDialog::show_size()
{
	QString str = ui.textEdit->toPlainText();
	//设置字号
	QFont ft;
	ft.setPointSize(12);
	ui.L_size->setFont(ft);
	//设置颜色
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::red);
	ui.L_size->setPalette(pa);
	ui.L_size->setText(QString::number(str.size(), 10));
}
void FileDialog::save() {
	if (!write_flag) {
		this->close();
	}
	QByteArray ba1 = this->filename.toLatin1();
	char *filename = ba1.data();
	//将原文件删除
	Msg msg=DeleteFile(filename);
	if (!msg.flag) {
		QMessageBox::warning(NULL, QStringLiteral("错误"), QString::fromLocal8Bit("当前文件不可写"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}

	//创建新文件
	CreateFile(filename);
	this->fileinode = OpenFile(filename);
	cout << "save" << endl;
	QString str = ui.textEdit->toPlainText();
	qDebug() << str << endl;
	str.replace("\u0000", "");
	qDebug() << "content:" << str << endl;
	QByteArray ba = str.toLatin1(); // must
	char *content = ba.data();
	cout << "content:" << content << endl;
	
	msg=Write(this->fileinode, content);
	emit ExitWin(filename);
	if (!msg.flag) {
		QMessageBox::about(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(msg.content.c_str()));
		return;
	}
}
void FileDialog::save2()
{
	if (!write_flag) {
		this->close();
	}
	
	QByteArray ba1 = this->filename.toLatin1();
	char *filename = ba1.data();
	this->fileinode = OpenFile(filename);
	cout << "save" << endl;
	QString str = ui.textEdit->toPlainText();
	qDebug() << str << endl;
	str.replace("\u0000", "");
	qDebug() << "content:" << str << endl;
	QByteArray ba = str.toLatin1(); // must
	char *content = ba.data();
	cout << "content:" << content << endl;
	Msg msg = Write(this->fileinode, content);
	emit ExitWin(filename);
	if (!msg.flag) {
		QMessageBox::about(this, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit(msg.content.c_str()));
		return;
	}
}
void FileDialog::cancle() {

	emit ExitWin(filename);
	this->close();
	
}
void FileDialog::count() {
	show_size();
}