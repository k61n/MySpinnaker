#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTime timer;
    timer.start();
    camera = new MySpinnaker();
    qDebug() << "Camera initialised in" << timer.elapsed() << "ms";

    //delete camera;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    try{
        QTime timer;

        timer.start();
        cv::Mat test_image = camera->getImage();
        qDebug() << "Image acquired in" << timer.elapsed() << "ms";

        if (!test_image.data)
        {
            QMessageBox msg;
            msg.setText("Could not load image");
            msg.exec();
        }
        timer.restart();
        cv::imwrite("test_shot.jpg", test_image);
        qDebug() << "Image saved in" << timer.elapsed() << "ms";
        cv::namedWindow("test image", cv::WINDOW_NORMAL);
        cv::imshow("test image", test_image);
    }
    catch (cv::Exception &e){
        qDebug() << "Error: " << e.what();
    }
}
