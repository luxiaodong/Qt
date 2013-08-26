#ifndef QPOINTTRANSLATEDIALOG_H
#define QPOINTTRANSLATEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class QPointTranslateDialog;
}

class QPointTranslateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QPointTranslateDialog(QWidget *parent = 0);
    ~QPointTranslateDialog();

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
    Ui::QPointTranslateDialog *ui;

private:
    QStringList m_fileList;
};

#endif // QPOINTTRANSLATEDIALOG_H
