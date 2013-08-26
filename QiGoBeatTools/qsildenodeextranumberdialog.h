#ifndef QSILDENODEEXTRANUMBERDIALOG_H
#define QSILDENODEEXTRANUMBERDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QSildeNodeExtraNumberDialog;
}

class QSildeNodeExtraNumberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QSildeNodeExtraNumberDialog(QWidget *parent = 0);
    ~QSildeNodeExtraNumberDialog();

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
    Ui::QSildeNodeExtraNumberDialog *ui;

private:
    QStringList m_fileList;
};

#endif // QSILDENODEEXTRANUMBERDIALOG_H
