/********************************************************************************
** Form generated from reading UI file 'UserLogin.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERLOGIN_H
#define UI_USERLOGIN_H

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

class Ui_UserLogin
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLineEdit *L_user;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLineEdit *L_password;
    QHBoxLayout *horizontalLayout;
    QPushButton *B_login;
    QPushButton *B_cancle;

    void setupUi(QWidget *UserLogin)
    {
        if (UserLogin->objectName().isEmpty())
            UserLogin->setObjectName(QStringLiteral("UserLogin"));
        UserLogin->resize(334, 230);
        gridLayout = new QGridLayout(UserLogin);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(UserLogin);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        L_user = new QLineEdit(UserLogin);
        L_user->setObjectName(QStringLiteral("L_user"));

        verticalLayout->addWidget(L_user);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_3 = new QLabel(UserLogin);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        L_password = new QLineEdit(UserLogin);
        L_password->setObjectName(QStringLiteral("L_password"));

        verticalLayout_2->addWidget(L_password);


        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        B_login = new QPushButton(UserLogin);
        B_login->setObjectName(QStringLiteral("B_login"));

        horizontalLayout->addWidget(B_login);

        B_cancle = new QPushButton(UserLogin);
        B_cancle->setObjectName(QStringLiteral("B_cancle"));

        horizontalLayout->addWidget(B_cancle);


        verticalLayout_4->addLayout(horizontalLayout);

        verticalLayout_4->setStretch(0, 2);
        verticalLayout_4->setStretch(1, 1);

        gridLayout->addLayout(verticalLayout_4, 0, 0, 1, 1);


        retranslateUi(UserLogin);

        QMetaObject::connectSlotsByName(UserLogin);
    } // setupUi

    void retranslateUi(QWidget *UserLogin)
    {
        UserLogin->setWindowTitle(QApplication::translate("UserLogin", "UserLogin", nullptr));
        label_2->setText(QApplication::translate("UserLogin", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\347\224\250\346\210\267\345\220\215:</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("UserLogin", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\345\257\206\347\240\201:</span></p></body></html>", nullptr));
        B_login->setText(QApplication::translate("UserLogin", "\347\231\273\345\275\225", nullptr));
        B_cancle->setText(QApplication::translate("UserLogin", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserLogin: public Ui_UserLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERLOGIN_H
