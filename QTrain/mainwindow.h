#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qsql.h>
#include <QStringList>
#include <qdebug.h>
#include <QSqlDatabase>
#include <QTextCodec>
#include <QSqlTableModel>
#include <QMouseEvent>
#include <QList>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent* e);
    void updateTrace(int x, int y);
    int direction(int x1, int y1, int x2, int y2);
private:
    QList<int>     m_moveTrace;

private:
    Ui::MainWindow *ui;
    QSqlTableModel *m_model;
};

#endif // MAINWINDOW_H
