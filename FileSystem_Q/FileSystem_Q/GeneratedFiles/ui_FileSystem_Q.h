/********************************************************************************
** Form generated from reading UI file 'FileSystem_Q.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILESYSTEM_Q_H
#define UI_FILESYSTEM_Q_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileSystem_QClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QTableView *inode_table;
    QTableView *tableView;
    QLabel *L_Wellcom;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FileSystem_QClass)
    {
        if (FileSystem_QClass->objectName().isEmpty())
            FileSystem_QClass->setObjectName(QStringLiteral("FileSystem_QClass"));
        FileSystem_QClass->resize(1043, 941);
        centralWidget = new QWidget(FileSystem_QClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        horizontalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_3->addWidget(pushButton_3);

        horizontalLayout_3->setStretch(0, 5);
        horizontalLayout_3->setStretch(1, 1);

        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 3);

        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout->addWidget(treeView);

        inode_table = new QTableView(centralWidget);
        inode_table->setObjectName(QStringLiteral("inode_table"));

        verticalLayout->addWidget(inode_table);

        verticalLayout->setStretch(0, 3);
        verticalLayout->setStretch(1, 4);

        horizontalLayout->addLayout(verticalLayout);

        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setMouseTracking(false);
        tableView->setLineWidth(24);
        tableView->setMidLineWidth(6);

        horizontalLayout->addWidget(tableView);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 5);

        verticalLayout_2->addLayout(horizontalLayout);

        L_Wellcom = new QLabel(centralWidget);
        L_Wellcom->setObjectName(QStringLiteral("L_Wellcom"));

        verticalLayout_2->addWidget(L_Wellcom);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

        FileSystem_QClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FileSystem_QClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1043, 26));
        FileSystem_QClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FileSystem_QClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FileSystem_QClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FileSystem_QClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FileSystem_QClass->setStatusBar(statusBar);

        retranslateUi(FileSystem_QClass);

        QMetaObject::connectSlotsByName(FileSystem_QClass);
    } // setupUi

    void retranslateUi(QMainWindow *FileSystem_QClass)
    {
        FileSystem_QClass->setWindowTitle(QApplication::translate("FileSystem_QClass", "FileSystem_Q", nullptr));
        pushButton->setText(QApplication::translate("FileSystem_QClass", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        pushButton_2->setText(QApplication::translate("FileSystem_QClass", "\350\277\224\345\233\236\344\270\273\351\241\265", nullptr));
        pushButton_3->setText(QApplication::translate("FileSystem_QClass", "\350\267\263\350\275\254", nullptr));
        L_Wellcom->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FileSystem_QClass: public Ui_FileSystem_QClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILESYSTEM_Q_H
