#ifndef QPNGSCALEDIALOG_H
#define QPNGSCALEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QPngScaleDialog;
}

class QPngScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QPngScaleDialog(QWidget *parent = 0);
    ~QPngScaleDialog();

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
    Ui::QPngScaleDialog *ui;

private:
    QStringList m_fileList;
};

#endif // QPNGSCALEDIALOG_H
