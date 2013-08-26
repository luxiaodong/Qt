#ifndef QPACKAGESONGSDIALOG_H
#define QPACKAGESONGSDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QPackageSongsDialog;
}

class QPackageSongsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QPackageSongsDialog(QWidget *parent = 0);
    ~QPackageSongsDialog();

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
    Ui::QPackageSongsDialog *ui;

private:
    QStringList m_dirPathlist;
};

#endif // QPACKAGESONGSDIALOG_H
