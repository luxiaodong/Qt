#ifndef QBINCOMPAREDIALOG_H
#define QBINCOMPAREDIALOG_H

#include <QDialog>

namespace Ui {
class QBinCompareDialog;
}

class QBinCompareDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QBinCompareDialog(QWidget *parent = 0);
    ~QBinCompareDialog();

public slots:
    void clickBrowse1();
    void clickBrowse2();
    void clickCompare();
    
private:
    Ui::QBinCompareDialog *ui;
};

#endif // QBINCOMPAREDIALOG_H
