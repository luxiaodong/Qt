#ifndef QPUZZLEDRAGONDIALOG_H
#define QPUZZLEDRAGONDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
class QPuzzleDragonDialog;
}

class QPuzzleDragonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QPuzzleDragonDialog(QWidget *parent = 0);
    ~QPuzzleDragonDialog();

public slots:
    void clickSplit();
    void clickCatchPng();

private:
    void splitMonster();
    void parseBin000(QString filePath);
    void parseBin001(QString filePath);
    void parseBin002(QString filePath);
    void parseBin003(QString filePath);

    void catchSinglePng(QString filePath);

private:
    QList<int> m_data1_offset;
    QList<int> m_data1_length;
    QList<int> m_data2_offset;
    QList<int> m_data2_length;
    QList<int> m_data3_offset;
    QList<int> m_data3_length;
    
private:
    Ui::QPuzzleDragonDialog *ui;
};

#endif // QPUZZLEDRAGONDIALOG_H
