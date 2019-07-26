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
    scene->setSceneRect(QRectF(QPointF(0, 0), QPointF(900, 600)));
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete camera;
    delete ui;
}

void MainWindow::on_received(cv::Mat cvMat)
{
    QImage img = QImage(cvMat.data, cvMat.cols, cvMat.rows, static_cast<int>(cvMat.step), QImage::Format_RGB888);
    if (ui->checkZoom->isChecked()) {
        img = img.copy(QRect(2286, 1524, 900, 600));
    }
    else {
        img = img.scaled(900, 600, Qt::KeepAspectRatio);
    }
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img.rgbSwapped()));
}

void MainWindow::on_pushButton_clicked()
{
    cv::Mat test_image = camera->getImage();
    this->on_received(test_image);
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

void MainWindow::on_pushButton_4_clicked()
{
    cv::Mat cvMat = camera->getImage();
    QImage img = QImage(cvMat.data, cvMat.cols, cvMat.rows, static_cast<int>(cvMat.step), QImage::Format_RGB888);
    img.save("wafer.png");
    qDebug() << "Image saved";
}
