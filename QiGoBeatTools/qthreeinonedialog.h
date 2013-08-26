#ifndef QTHREEINONEDIALOG_H
#define QTHREEINONEDIALOG_H

#include <QDialog>
#include <QString>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QThreeInOneDialog;
}

class QThreeInOneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QThreeInOneDialog(QWidget *parent = 0);
    ~QThreeInOneDialog();

protected:
    void dropEvent( QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public slots:
    void clickOK();
    void clickClear();
    void clickQuit();
    void selectAll(int state);

private:
    void createModel();

private:
    Ui::QThreeInOneDialog *ui;

private:
    QString m_dirPath;
};

#endif // QTHREEINONEDIALOG_H
