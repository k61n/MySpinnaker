#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFuture>
#include <QtConcurrent>
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("cv::Mat");

    camera = new MySpinnaker();

    connect(camera, &MySpinnaker::on_capture, this, &MainWindow::on_received);
    connect(this, &MainWindow::on_stream_start_request, camera, &MySpinnaker::on_start_streaming);
    connect(this, &MainWindow::on_stream_stop_request, camera, &MySpinnaker::on_stop_streaming);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(QRectF(QPointF(0, 0), QPointF(600, 400)));
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete camera;
    delete ui;
}

void MainWindow::updateScene(cv::Mat cvMat)
{
    QImage img = QImage((uchar*)cvMat.data, cvMat.cols, cvMat.rows, cvMat.step, QImage::Format_RGB888);
    img = img.scaledToWidth(scene->sceneRect().width());
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img.rgbSwapped()));
}

void MainWindow::on_received(cv::Mat cvMat)
{
    updateScene(cvMat);
}

void MainWindow::on_pushButton_clicked()
{
    cv::Mat test_image = camera->getImage();
    updateScene(test_image);
}

void MainWindow::on_pushButton_2_clicked()
{
    emit on_stream_start_request();
    QFuture<void> test = QtConcurrent::run(this->camera, &MySpinnaker::stream);
}

void MainWindow::on_pushButton_3_clicked()
{
    emit on_stream_stop_request();
}
