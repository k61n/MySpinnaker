#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "myspinnaker.h"
//#include "opencv2/core.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateScene(cv::Mat cvMat);

signals:
    void on_stream_start_request();
    void on_stream_stop_request();

public slots:
    void on_received(cv::Mat cvMat);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    MySpinnaker *camera;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
