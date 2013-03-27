#ifndef QRENAMEDIALOG_H
#define QRENAMEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
class QRenameDialog;
}

class QRenameDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QRenameDialog(QWidget *parent = 0);
    ~QRenameDialog();

protected:
    void dropEvent( QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public slots:
    void clickOK();
    void clickClear();
    void clickQuit();
    void clickBrowse();
    void selectAll(int state);

private:
    void traverseFiles(QString dirPath);

private:
    void createModel();
    bool rename(QString filePath);

private:
    QStringList m_fileList;
    QString m_fileSuffix;
    
private:
    Ui::QRenameDialog *ui;
};

#endif // QRENAMEDIALOG_H
