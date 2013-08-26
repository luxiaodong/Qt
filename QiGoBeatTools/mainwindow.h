#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void clickPointTranslate();
    void clickPackageSongs();
    void clickAddHd();
    void clickScalePng();
    void clickAddSlideNodeExtraNumber();
    void clickBsmToIGoBeat();
    void clickEncryption();
    void click3In1();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
