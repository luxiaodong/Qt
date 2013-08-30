/********************************************************************************
** Form generated from reading UI file 'qlamp.ui'
**
** Created: Sun Sep 12 13:20:05 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLAMP_H
#define UI_QLAMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qlamp
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTableWidget *table;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qlamp)
    {
        if (Qlamp->objectName().isEmpty())
            Qlamp->setObjectName(QString::fromUtf8("Qlamp"));
        Qlamp->resize(425, 345);
        centralWidget = new QWidget(Qlamp);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        table = new QTableWidget(centralWidget);
        table->setObjectName(QString::fromUtf8("table"));

        horizontalLayout->addWidget(table);

        Qlamp->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qlamp);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 425, 17));
        Qlamp->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qlamp);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Qlamp->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Qlamp);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Qlamp->setStatusBar(statusBar);

        retranslateUi(Qlamp);

        QMetaObject::connectSlotsByName(Qlamp);
    } // setupUi

    void retranslateUi(QMainWindow *Qlamp)
    {
        Qlamp->setWindowTitle(QApplication::translate("Qlamp", "Qlamp", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Qlamp: public Ui_Qlamp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLAMP_H
