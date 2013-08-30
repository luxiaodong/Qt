#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QHostInfo>
#include <QTcpSocket>

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QString userName,QWidget *parent = 0);
    ~LoginDialog();
    QString userName();
    QString serverIP();

public slots:
    void accept();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
