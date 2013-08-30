#ifndef QLAMP_H
#define QLAMP_H

#include <QMainWindow>
#include <QPainter>
#include <QRect>
#include <QList>
#include <QPoint>
#include <QMouseEvent>
#include "../../kernel/qpermutegroup.h"
#include "../../kernel/qquaterniongroup.h"
#include "../../kernel/qcongruencegroup.h"
#include "../../kernel/qabstractdihedralgroup.h"
#include "../../model/qbinarymodel.h"
#include "qlamplogic.h"

namespace Ui {
    class Qlamp;
}

class Qlamp : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qlamp(QWidget *parent = 0);
    ~Qlamp();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent( QMouseEvent * e );

//for test code
    QString printTrace(GMatrix m);
    void printPermuteGroup();
    void printGroupMatrix();
private:
    Ui::Qlamp *ui;
    QLampLogic m_logic;
};

#endif // QLAMP_H
