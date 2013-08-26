#ifndef QENCRYPTIONDIALOG_H
#define QENCRYPTIONDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QEncryptionDialog;
}

class QEncryptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QEncryptionDialog(QWidget *parent = 0);
    ~QEncryptionDialog();

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
    Ui::QEncryptionDialog *ui;

private:
    QStringList m_fileList;
};

#endif // QENCRYPTIONDIALOG_H
