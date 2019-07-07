/********************************************************************************
** Form generated from reading UI file 'PassWord.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORD_H
#define UI_PASSWORD_H

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

class Ui_PassWord
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *L_oldPass;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLineEdit *L_newPass;
    QHBoxLayout *horizontalLayout;
    QPushButton *B_check;
    QPushButton *B_cancle;

    void setupUi(QWidget *PassWord)
    {
        if (PassWord->objectName().isEmpty())
            PassWord->setObjectName(QStringLiteral("PassWord"));
        PassWord->resize(265, 224);
        gridLayout = new QGridLayout(PassWord);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(PassWord);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        L_oldPass = new QLineEdit(PassWord);
        L_oldPass->setObjectName(QStringLiteral("L_oldPass"));

        verticalLayout->addWidget(L_oldPass);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(PassWord);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        L_newPass = new QLineEdit(PassWord);
        L_newPass->setObjectName(QStringLiteral("L_newPass"));

        verticalLayout_2->addWidget(L_newPass);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        B_check = new QPushButton(PassWord);
        B_check->setObjectName(QStringLiteral("B_check"));

        horizontalLayout->addWidget(B_check);

        B_cancle = new QPushButton(PassWord);
        B_cancle->setObjectName(QStringLiteral("B_cancle"));

        horizontalLayout->addWidget(B_cancle);


        verticalLayout_3->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);


        retranslateUi(PassWord);

        QMetaObject::connectSlotsByName(PassWord);
    } // setupUi

    void retranslateUi(QWidget *PassWord)
    {
        PassWord->setWindowTitle(QApplication::translate("PassWord", "PassWord", nullptr));
        label->setText(QApplication::translate("PassWord", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\345\216\237\345\257\206\347\240\201\357\274\232</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("PassWord", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:600;\">\346\226\260\345\257\206\347\240\201\357\274\232</span></p></body></html>", nullptr));
        B_check->setText(QApplication::translate("PassWord", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
        B_cancle->setText(QApplication::translate("PassWord", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PassWord: public Ui_PassWord {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORD_H
