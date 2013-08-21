#ifndef QSIZESELECT_H
#define QSIZESELECT_H

#include <QDialog>
#include <QSize>

namespace Ui {
    class QSizeSelect;
}

class QSizeSelect : public QDialog
{
    Q_OBJECT

public:
    explicit QSizeSelect(QWidget *parent = 0);
    ~QSizeSelect();

    QSize selectSize();
public slots:
    void click1024();
    void click768();
    void click480();
    void click320();
    void clickCustom();

private:
    Ui::QSizeSelect *ui;

private:
    QSize m_size;
};

#endif // QSIZESELECT_H
