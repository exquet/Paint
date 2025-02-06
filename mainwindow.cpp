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

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        QPainter painter(&m_image);

        // Настраиваем перо: черный цвет, толщина линии 3 пикселя, сплошная линия с закругленными концами
        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        // Рисуем линию от последней сохраненной точки до текущей позиции курсора
        painter.drawLine(m_lastPoint, event->pos());
        // Обновляем m_lastPoint для непрерывного рисования
        m_lastPoint = event->pos();
        // Запрашиваем перерисовку окна, чтобы новые линии отобразились
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    // Рисуем наше изображение (холст) в верхнем левом углу окна (0,0)
    painter.drawImage(0, 0, m_image);
}
