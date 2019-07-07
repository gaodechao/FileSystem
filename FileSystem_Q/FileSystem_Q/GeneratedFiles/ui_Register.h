/********************************************************************************
** Form generated from reading UI file 'Register.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

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

class Ui_Register
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *R_username;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *R_password;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *B_Register;
    QPushButton *B_return;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QStringLiteral("Register"));
        Register->resize(449, 348);
        gridLayout = new QGridLayout(Register);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_3 = new QLabel(Register);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(Register);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        R_username = new QLineEdit(Register);
        R_username->setObjectName(QStringLiteral("R_username"));

        horizontalLayout->addWidget(R_username);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(Register);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        R_password = new QLineEdit(Register);
        R_password->setObjectName(QStringLiteral("R_password"));

        horizontalLayout_2->addWidget(R_password);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 3);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        B_Register = new QPushButton(Register);
        B_Register->setObjectName(QStringLiteral("B_Register"));

        horizontalLayout_3->addWidget(B_Register);

        B_return = new QPushButton(Register);
        B_return->setObjectName(QStringLiteral("B_return"));

        horizontalLayout_3->addWidget(B_return);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 1);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QApplication::translate("Register", "Register", nullptr));
        label_3->setText(QApplication::translate("Register", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#ff5500;\">\346\226\207\344\273\266\347\263\273\347\273\237\346\263\250\345\206\214</span></p><p align=\"center\"><br/></p></body></html>", nullptr));
        label->setText(QApplication::translate("Register", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">\347\224\250\346\210\267\345\220\215</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("Register", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">\345\257\206\347\240\201</span></p></body></html>", nullptr));
        B_Register->setText(QApplication::translate("Register", "\346\263\250\345\206\214", nullptr));
        B_return->setText(QApplication::translate("Register", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
