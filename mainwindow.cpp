#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::resize(800, 600);
    m_image = QImage(800, 600, QImage::Format_RGB32);
    m_image.fill(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_lastPoint = event->pos();
    }
}
