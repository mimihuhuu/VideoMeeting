/********************************************************************************
** Form generated from reading UI file 'CLogDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOGDIALOG_H
#define UI_CLOGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CLogDialogClass
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget_left;
    QVBoxLayout *LeftList_Vlayout;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_logo;
    QLabel *label_name;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *BtnSetting;
    QSpacerItem *verticalSpacer_6;
    QLabel *label;
    QLineEdit *lineEdit_roomID;
    QLabel *label_8;
    QLineEdit *lineEdit_username;
    QLabel *label_7;
    QLineEdit *lineEdit_pwd;
    QSpacerItem *verticalSpacer_5;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer_4;
    QCheckBox *checkBox_2;
    QPushButton *btnJion;
    QSpacerItem *horizontalSpacer_4;
    QWidget *widget_right;
    QVBoxLayout *BigVideoWidget_Vlayout;
    QHBoxLayout *BigVideoWidget_Top;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnMin;
    QPushButton *btnClose;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *CLogDialogClass)
    {
        if (CLogDialogClass->objectName().isEmpty())
            CLogDialogClass->setObjectName(QString::fromUtf8("CLogDialogClass"));
        CLogDialogClass->resize(724, 620);
        horizontalLayout = new QHBoxLayout(CLogDialogClass);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_left = new QWidget(CLogDialogClass);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        LeftList_Vlayout = new QVBoxLayout(widget_left);
        LeftList_Vlayout->setSpacing(6);
        LeftList_Vlayout->setContentsMargins(11, 11, 11, 11);
        LeftList_Vlayout->setObjectName(QString::fromUtf8("LeftList_Vlayout"));
        LeftList_Vlayout->setContentsMargins(9, 0, 9, 9);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_logo = new QLabel(widget_left);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));

        horizontalLayout_4->addWidget(label_logo);

        label_name = new QLabel(widget_left);
        label_name->setObjectName(QString::fromUtf8("label_name"));

        horizontalLayout_4->addWidget(label_name);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        BtnSetting = new QPushButton(widget_left);
        BtnSetting->setObjectName(QString::fromUtf8("BtnSetting"));

        horizontalLayout_4->addWidget(BtnSetting);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_6 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_6);

        label = new QLabel(widget_left);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        lineEdit_roomID = new QLineEdit(widget_left);
        lineEdit_roomID->setObjectName(QString::fromUtf8("lineEdit_roomID"));

        verticalLayout_2->addWidget(lineEdit_roomID);

        label_8 = new QLabel(widget_left);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout_2->addWidget(label_8);

        lineEdit_username = new QLineEdit(widget_left);
        lineEdit_username->setObjectName(QString::fromUtf8("lineEdit_username"));

        verticalLayout_2->addWidget(lineEdit_username);

        label_7 = new QLabel(widget_left);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_2->addWidget(label_7);

        lineEdit_pwd = new QLineEdit(widget_left);
        lineEdit_pwd->setObjectName(QString::fromUtf8("lineEdit_pwd"));

        verticalLayout_2->addWidget(lineEdit_pwd);

        verticalSpacer_5 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_5);

        checkBox = new QCheckBox(widget_left);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setChecked(true);

        verticalLayout_2->addWidget(checkBox);

        verticalSpacer_4 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_4);

        checkBox_2 = new QCheckBox(widget_left);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setChecked(true);

        verticalLayout_2->addWidget(checkBox_2);

        btnJion = new QPushButton(widget_left);
        btnJion->setObjectName(QString::fromUtf8("btnJion"));

        verticalLayout_2->addWidget(btnJion);


        horizontalLayout_5->addLayout(verticalLayout_2);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        LeftList_Vlayout->addLayout(horizontalLayout_5);


        horizontalLayout->addWidget(widget_left);

        widget_right = new QWidget(CLogDialogClass);
        widget_right->setObjectName(QString::fromUtf8("widget_right"));
        BigVideoWidget_Vlayout = new QVBoxLayout(widget_right);
        BigVideoWidget_Vlayout->setSpacing(0);
        BigVideoWidget_Vlayout->setContentsMargins(11, 11, 11, 11);
        BigVideoWidget_Vlayout->setObjectName(QString::fromUtf8("BigVideoWidget_Vlayout"));
        BigVideoWidget_Vlayout->setContentsMargins(0, 0, 0, 0);
        BigVideoWidget_Top = new QHBoxLayout();
        BigVideoWidget_Top->setSpacing(6);
        BigVideoWidget_Top->setObjectName(QString::fromUtf8("BigVideoWidget_Top"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        BigVideoWidget_Top->addItem(horizontalSpacer);

        btnMin = new QPushButton(widget_right);
        btnMin->setObjectName(QString::fromUtf8("btnMin"));

        BigVideoWidget_Top->addWidget(btnMin);

        btnClose = new QPushButton(widget_right);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        BigVideoWidget_Top->addWidget(btnClose);


        BigVideoWidget_Vlayout->addLayout(BigVideoWidget_Top);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        BigVideoWidget_Vlayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_right);


        retranslateUi(CLogDialogClass);

        QMetaObject::connectSlotsByName(CLogDialogClass);
    } // setupUi

    void retranslateUi(QDialog *CLogDialogClass)
    {
        CLogDialogClass->setWindowTitle(QCoreApplication::translate("CLogDialogClass", "CLogDialog", nullptr));
        label_logo->setText(QCoreApplication::translate("CLogDialogClass", "TextLabel", nullptr));
        label_name->setText(QCoreApplication::translate("CLogDialogClass", "TextLabel", nullptr));
        BtnSetting->setText(QCoreApplication::translate("CLogDialogClass", "PushButton", nullptr));
        label->setText(QCoreApplication::translate("CLogDialogClass", "\346\210\277\351\227\264\345\217\267", nullptr));
        label_8->setText(QCoreApplication::translate("CLogDialogClass", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_7->setText(QCoreApplication::translate("CLogDialogClass", "\345\257\206\347\240\201", nullptr));
        checkBox->setText(QCoreApplication::translate("CLogDialogClass", "\345\274\200\345\220\257\346\221\204\345\203\217\345\244\264", nullptr));
        checkBox_2->setText(QCoreApplication::translate("CLogDialogClass", "\345\274\200\345\220\257\351\272\246\345\205\213\351\243\216", nullptr));
        btnJion->setText(QCoreApplication::translate("CLogDialogClass", "\345\212\240\345\205\245\344\274\232\350\256\256", nullptr));
        btnMin->setText(QCoreApplication::translate("CLogDialogClass", "PushButton", nullptr));
        btnClose->setText(QCoreApplication::translate("CLogDialogClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CLogDialogClass: public Ui_CLogDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOGDIALOG_H
