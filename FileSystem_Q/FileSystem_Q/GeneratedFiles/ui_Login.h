/********************************************************************************
** Form generated from reading UI file 'Login.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *T_user;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *T_password;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *B_login;
    QPushButton *B_cancle;
    QPushButton *B_regist;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QStringLiteral("Login"));
        Login->resize(361, 179);
        gridLayout = new QGridLayout(Login);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(Login);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        T_user = new QLineEdit(Login);
        T_user->setObjectName(QStringLiteral("T_user"));

        horizontalLayout->addWidget(T_user);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        T_password = new QLineEdit(Login);
        T_password->setObjectName(QStringLiteral("T_password"));

        horizontalLayout_2->addWidget(T_password);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 3);

        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        B_login = new QPushButton(Login);
        B_login->setObjectName(QStringLiteral("B_login"));

        horizontalLayout_3->addWidget(B_login);

        B_cancle = new QPushButton(Login);
        B_cancle->setObjectName(QStringLiteral("B_cancle"));

        horizontalLayout_3->addWidget(B_cancle);

        B_regist = new QPushButton(Login);
        B_regist->setObjectName(QStringLiteral("B_regist"));

        horizontalLayout_3->addWidget(B_regist);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalLayout_2->setStretch(0, 3);
        verticalLayout_2->setStretch(1, 1);

        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Login", nullptr));
        label->setText(QApplication::translate("Login", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\347\224\250\346\210\267\345\220\215\357\274\232</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("Login", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\345\257\206\347\240\201\357\274\232</span></p></body></html>", nullptr));
        B_login->setText(QApplication::translate("Login", "\347\231\273\345\275\225", nullptr));
        B_cancle->setText(QApplication::translate("Login", "\346\263\250\345\206\214", nullptr));
        B_regist->setText(QApplication::translate("Login", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
