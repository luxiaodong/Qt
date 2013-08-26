#ifndef QADDHDDIALOG_H
#define QADDHDDIALOG_H

#include <QDialog>
#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QAddHdDialog;
}

class QAddHdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QAddHdDialog(QWidget *parent = 0);
    ~QAddHdDialog();

protected:
    void dropEvent( QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public slots:
    void clickOK();
    void clickClear();
    void clickQuit();
    void selectAll(int state);

private:
    void traverseFiles(QString dirPath);

private:
    void createModel();

private:
    Ui::QAddHdDialog *ui;

private:
    QStringList m_fileList;
};

#endif // QADDHDDIALOG_H
