/********************************************************************************
** Form generated from reading UI file 'UserManage.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANAGE_H
#define UI_USERMANAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserManage
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *L_user;
    QLineEdit *L_password;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *B_add;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QTableView *tableView;

    void setupUi(QWidget *UserManage)
    {
        if (UserManage->objectName().isEmpty())
            UserManage->setObjectName(QStringLiteral("UserManage"));
        UserManage->resize(484, 386);
        gridLayout_2 = new QGridLayout(UserManage);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(UserManage);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(UserManage);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(UserManage);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        L_user = new QLineEdit(UserManage);
        L_user->setObjectName(QStringLiteral("L_user"));

        gridLayout->addWidget(L_user, 1, 0, 1, 1);

        L_password = new QLineEdit(UserManage);
        L_password->setObjectName(QStringLiteral("L_password"));

        gridLayout->addWidget(L_password, 1, 1, 1, 1);

        comboBox = new QComboBox(UserManage);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 1, 1, 1);

        B_add = new QPushButton(UserManage);
        B_add->setObjectName(QStringLiteral("B_add"));

        gridLayout->addWidget(B_add, 2, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_4 = new QLabel(UserManage);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout->addWidget(label_4);

        tableView = new QTableView(UserManage);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout->addWidget(tableView);


        verticalLayout_2->addLayout(verticalLayout);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(UserManage);

        QMetaObject::connectSlotsByName(UserManage);
    } // setupUi

    void retranslateUi(QWidget *UserManage)
    {
        UserManage->setWindowTitle(QApplication::translate("UserManage", "UserManage", nullptr));
        label->setText(QApplication::translate("UserManage", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\347\224\250\346\210\267\345\220\215</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("UserManage", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\345\257\206\347\240\201</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("UserManage", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\347\273\204\345\217\267</span></p></body></html>", nullptr));
        comboBox->setItemText(0, QApplication::translate("UserManage", "1", nullptr));
        comboBox->setItemText(1, QApplication::translate("UserManage", "2", nullptr));
        comboBox->setItemText(2, QApplication::translate("UserManage", "3", nullptr));
        comboBox->setItemText(3, QApplication::translate("UserManage", "4", nullptr));
        comboBox->setItemText(4, QApplication::translate("UserManage", "5", nullptr));
        comboBox->setItemText(5, QApplication::translate("UserManage", "6", nullptr));
        comboBox->setItemText(6, QApplication::translate("UserManage", "7", nullptr));
        comboBox->setItemText(7, QApplication::translate("UserManage", "8", nullptr));
        comboBox->setItemText(8, QApplication::translate("UserManage", "9", nullptr));

        B_add->setText(QApplication::translate("UserManage", "\346\267\273\345\212\240", nullptr));
        label_4->setText(QApplication::translate("UserManage", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600; text-decoration: underline;\">\347\224\250\346\210\267\344\277\241\346\201\257\357\274\232</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserManage: public Ui_UserManage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANAGE_H
