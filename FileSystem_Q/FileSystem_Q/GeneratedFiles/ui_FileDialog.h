/********************************************************************************
** Form generated from reading UI file 'FileDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEDIALOG_H
#define UI_FILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *L_filename;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QPushButton *B_save;
    QPushButton *B_save2;
    QPushButton *B_cancle;
    QVBoxLayout *verticalLayout_3;
    QLabel *L_mode;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_3;
    QLabel *L_size;

    void setupUi(QWidget *FileDialog)
    {
        if (FileDialog->objectName().isEmpty())
            FileDialog->setObjectName(QStringLiteral("FileDialog"));
        FileDialog->resize(1015, 477);
        gridLayout = new QGridLayout(FileDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(FileDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        L_filename = new QLabel(FileDialog);
        L_filename->setObjectName(QStringLiteral("L_filename"));

        horizontalLayout->addWidget(L_filename);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 5);

        verticalLayout->addLayout(horizontalLayout);

        textEdit = new QTextEdit(FileDialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout->addWidget(textEdit);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        B_save = new QPushButton(FileDialog);
        B_save->setObjectName(QStringLiteral("B_save"));

        verticalLayout_2->addWidget(B_save);

        B_save2 = new QPushButton(FileDialog);
        B_save2->setObjectName(QStringLiteral("B_save2"));

        verticalLayout_2->addWidget(B_save2);

        B_cancle = new QPushButton(FileDialog);
        B_cancle->setObjectName(QStringLiteral("B_cancle"));

        verticalLayout_2->addWidget(B_cancle);


        verticalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        L_mode = new QLabel(FileDialog);
        L_mode->setObjectName(QStringLiteral("L_mode"));

        verticalLayout_3->addWidget(L_mode);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        label_3 = new QLabel(FileDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        L_size = new QLabel(FileDialog);
        L_size->setObjectName(QStringLiteral("L_size"));

        verticalLayout_3->addWidget(L_size);

        verticalLayout_3->setStretch(5, 1);
        verticalLayout_3->setStretch(6, 6);

        verticalLayout_4->addLayout(verticalLayout_3);

        verticalLayout_4->setStretch(0, 1);
        verticalLayout_4->setStretch(1, 4);

        gridLayout->addLayout(verticalLayout_4, 0, 1, 1, 1);


        retranslateUi(FileDialog);

        QMetaObject::connectSlotsByName(FileDialog);
    } // setupUi

    void retranslateUi(QWidget *FileDialog)
    {
        FileDialog->setWindowTitle(QApplication::translate("FileDialog", "FileDialog", nullptr));
        label->setText(QApplication::translate("FileDialog", "\345\275\223\345\211\215\346\226\207\344\273\266\357\274\232", nullptr));
        L_filename->setText(QString());
        B_save->setText(QApplication::translate("FileDialog", "\344\277\235\345\255\230", nullptr));
        B_save2->setText(QApplication::translate("FileDialog", "\350\277\275\345\212\240\344\277\235\345\255\230", nullptr));
        B_cancle->setText(QApplication::translate("FileDialog", "\351\200\200\345\207\272", nullptr));
        L_mode->setText(QString());
        label_3->setText(QApplication::translate("FileDialog", "\345\255\227\350\212\202\347\273\237\350\256\241\357\274\232", nullptr));
        L_size->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FileDialog: public Ui_FileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEDIALOG_H
