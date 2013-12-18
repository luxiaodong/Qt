#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void clickConvertDialogue();
    void clickConvertBezierPoly();
    void clickCityRoadMap();
    void clickCityPosition();
    void clickCubePoly();
    void clickTracePoint();
    void clickXiaoqianCollect();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
