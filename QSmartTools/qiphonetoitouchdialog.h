#ifndef QIPHONETOITOUCHDIALOG_H
#define QIPHONETOITOUCHDIALOG_H

#include <QDialog>
#include <QString>
#include <QDir>
#include <QStringList>

namespace Ui {
class QIPhoneToITouchDialog;
}

class QIPhoneToITouchDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QIPhoneToITouchDialog(QWidget *parent = 0);
    ~QIPhoneToITouchDialog();

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
    void replaceAnimationPath(QString fileName);

private:
    QString suffix(QString filePath);
    QString targetPath(QString srcPath);
    bool createDir(QString filePath);
private:
    QString m_resourceRootPath;
    QStringList m_allFiles;
    QStringList m_allDirs;
    
private:
    Ui::QIPhoneToITouchDialog *ui;
};

#endif // QIPHONETOITOUCHDIALOG_H
