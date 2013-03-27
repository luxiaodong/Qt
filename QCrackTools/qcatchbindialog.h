#ifndef QCATCHBINDIALOG_H
#define QCATCHBINDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class QCatchBinDialog;
}

class QCatchBinDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QCatchBinDialog(QWidget *parent = 0);
    ~QCatchBinDialog();

public slots:
    void clickBrowse();
    void clickCatch();
    
private:
    Ui::QCatchBinDialog *ui;
};

#endif // QCATCHBINDIALOG_H
