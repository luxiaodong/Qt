#ifndef QIPHONETOIPADDIALOG_H
#define QIPHONETOIPADDIALOG_H

#include <QDialog>

namespace Ui {
class QIPhoneToIPadDialog;
}

class QIPhoneToIPadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QIPhoneToIPadDialog(QWidget *parent = 0);
    ~QIPhoneToIPadDialog();

public slots:
    void clickResourcePath();
    void clickOK();

private:
    void traverseDir(QString rootDir);

private:
    void convert(QString filePath);
    void convertUI(QString srcPath, QString tagPath);
    void convertXml(QString srcPath, QString tagPath);
    void convertPlist(QString srcPath, QString tagPath);
    void convertPng(QString srcPath, QString tagPath);

private:
    QString suffix(QString filePath);
    QString targetPath(QString srcPath);
    bool createDir(QString filePath);
private:
    QString m_resourceRootPath;
    QStringList m_allFiles;
    QStringList m_allDirs;
    
private:
    Ui::QIPhoneToIPadDialog *ui;
};

#endif // QIPHONETOIPADDIALOG_H
