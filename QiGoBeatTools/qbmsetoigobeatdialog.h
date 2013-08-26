#ifndef QBMSETOIGOBEATDIALOG_H
#define QBMSETOIGOBEATDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QBmseToIGoBeatDialog;
}

class QBmseToIGoBeatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QBmseToIGoBeatDialog(QWidget *parent = 0);
    ~QBmseToIGoBeatDialog();

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
    Ui::QBmseToIGoBeatDialog *ui;

private:
    QStringList m_fileList;
};

#endif // QBMSETOIGOBEATDIALOG_H
