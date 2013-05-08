#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QAxObject>
#include <QAxWidget>
#include <QAxSelect>
#include <QFile>
#include <QFileDialog>
#include <QString>

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
    void clickOK();
    void clickBrowse1();
    void clickBrowse2();

private:
    void test();
    bool readSalary();
    bool writeSalary();

    QMap<QString, QString> m_map;
    QMap<QString, int> m_mapHelp;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
