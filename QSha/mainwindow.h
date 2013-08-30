#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "server/qserver.h"
#include "client/qclient.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QString strings(int type);

public slots:
    void on_actionServer_triggered();
    void on_actionStart_triggered();
    void on_actionClient_triggered();
    void on_actionQuit_triggered();
    void displayMessage(QString msg);
    void talking();
private:
    QServer* m_server;
    QClient* m_client;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
