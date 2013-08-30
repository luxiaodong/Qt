#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QFileDialog>
#include <QMainWindow>

#include "qhdwikiread.h"
#include "qprotocoldata.h"
#include "qcocos2dxcode.h"
#include "qunitycode.h"
#include "qphpcode.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void test();

    void createCocos2dxCode();
    void createPHPCode();
    void createUnityCode();

    void setCodePath();
public:
    QHDWikiRead m_hdWikiRead;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
