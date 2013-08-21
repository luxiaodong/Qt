#ifndef QENTERNAME_H
#define QENTERNAME_H

#include <QDialog>

namespace Ui {
    class QEnterName;
}

class QEnterName : public QDialog
{
    Q_OBJECT

public:
    explicit QEnterName(QWidget *parent = 0);
    ~QEnterName();

    void setNodeName(const QString &name);
    QString nodeName();

private:
    Ui::QEnterName *ui;
};

#endif // QENTERNAME_H
