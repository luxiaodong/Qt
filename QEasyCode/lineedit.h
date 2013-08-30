#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QDialog>
#include <QString>

namespace Ui {
    class LineEdit;
}

class LineEdit : public QDialog
{
    Q_OBJECT

public:
    explicit LineEdit(QWidget *parent = 0);
    ~LineEdit();

    void setTitle(QString);
    void setText(QString);
    QString lineText();

private:
    Ui::LineEdit *ui;
};

#endif // LINEEDIT_H
