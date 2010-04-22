/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Apr 22 13:00:08 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLab_1_1;
    QAction *actionLab_1_2;
    QAction *actionLab_2_1;
    QAction *actionLab_2_2;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuLab;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        actionLab_1_1 = new QAction(MainWindow);
        actionLab_1_1->setObjectName(QString::fromUtf8("actionLab_1_1"));
        actionLab_1_2 = new QAction(MainWindow);
        actionLab_1_2->setObjectName(QString::fromUtf8("actionLab_1_2"));
        actionLab_2_1 = new QAction(MainWindow);
        actionLab_2_1->setObjectName(QString::fromUtf8("actionLab_2_1"));
        actionLab_2_2 = new QAction(MainWindow);
        actionLab_2_2->setObjectName(QString::fromUtf8("actionLab_2_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        menuLab = new QMenu(menuBar);
        menuLab->setObjectName(QString::fromUtf8("menuLab"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuLab->menuAction());
        menuLab->addAction(actionLab_1_1);
        menuLab->addAction(actionLab_1_2);
        menuLab->addSeparator();
        menuLab->addAction(actionLab_2_1);
        menuLab->addAction(actionLab_2_2);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionLab_1_1->setText(QApplication::translate("MainWindow", "Lab 1.1", 0, QApplication::UnicodeUTF8));
        actionLab_1_2->setText(QApplication::translate("MainWindow", "Lab 1.2", 0, QApplication::UnicodeUTF8));
        actionLab_2_1->setText(QApplication::translate("MainWindow", "Lab 2.1", 0, QApplication::UnicodeUTF8));
        actionLab_2_2->setText(QApplication::translate("MainWindow", "Lab 2.2", 0, QApplication::UnicodeUTF8));
        menuLab->setTitle(QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
