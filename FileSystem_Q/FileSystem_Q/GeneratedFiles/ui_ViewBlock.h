/********************************************************************************
** Form generated from reading UI file 'ViewBlock.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWBLOCK_H
#define UI_VIEWBLOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewBlock
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QListView *listView;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QTableView *inodeView;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QTableView *blockView;

    void setupUi(QWidget *ViewBlock)
    {
        if (ViewBlock->objectName().isEmpty())
            ViewBlock->setObjectName(QStringLiteral("ViewBlock"));
        ViewBlock->resize(955, 818);
        gridLayout = new QGridLayout(ViewBlock);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ViewBlock);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        listView = new QListView(ViewBlock);
        listView->setObjectName(QStringLiteral("listView"));

        horizontalLayout->addWidget(listView);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(ViewBlock);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        inodeView = new QTableView(ViewBlock);
        inodeView->setObjectName(QStringLiteral("inodeView"));

        horizontalLayout_2->addWidget(inodeView);


        verticalLayout->addLayout(horizontalLayout_2);

        label_3 = new QLabel(ViewBlock);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        blockView = new QTableView(ViewBlock);
        blockView->setObjectName(QStringLiteral("blockView"));

        horizontalLayout_3->addWidget(blockView);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 8);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 14);
        verticalLayout->setStretch(4, 1);
        verticalLayout->setStretch(5, 16);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(ViewBlock);

        QMetaObject::connectSlotsByName(ViewBlock);
    } // setupUi

    void retranslateUi(QWidget *ViewBlock)
    {
        ViewBlock->setWindowTitle(QApplication::translate("ViewBlock", "ViewBlock", nullptr));
        label->setText(QApplication::translate("ViewBlock", "<html><head/><body><p>\350\266\205\347\272\247\345\235\227 \357\274\232</p></body></html>", nullptr));
        label_2->setText(QApplication::translate("ViewBlock", "<html><head/><body><p>Inode\347\273\223\347\202\271\357\274\232</p></body></html>", nullptr));
        label_3->setText(QApplication::translate("ViewBlock", "<html><head/><body><p>Blocked\347\273\223\347\202\271\357\274\232</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewBlock: public Ui_ViewBlock {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWBLOCK_H
