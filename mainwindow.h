#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "myspinnaker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    MySpinnaker *camera;
    cv::Mat imgFromFlir;
};

#endif // MAINWINDOW_H
