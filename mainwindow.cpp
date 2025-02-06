#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::resize(800, 600);
    m_image = QImage(800, 600, QImage::Format_RGB32); // создание холста
    m_image.fill(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){ // отслеживание первого нажатия
        m_lastPoint = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){ // отслеживание изменения положия при нажатом ЛКМ
        QPainter painter(&m_image);

        // перо: черный цвет, толщина линии 3 пикселя, сплошная линия с закругленными концами
        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(m_lastPoint, event->pos()); // отрисовка линии от начального положения до текущего
        m_lastPoint = event->pos(); // обновление положения курсора
        update(); // обновление холста
    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(0, 0, m_image); // отрисовка изображения
}

void MainWindow::on_actionClear_triggered()
{
    m_image.fill(Qt::white);
    update();
}

