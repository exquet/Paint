#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , colourPen(Qt::black)
    , penThickness(3)
{
    ui->setupUi(this);

    MainWindow::resize(800, 600);
    m_image = QImage(800, 600, QImage::Format_RGB32); // создание холста
    m_image.fill(Qt::white);

    //file
    QIcon fileIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/file.png");
    ui->menuFile->setIcon(fileIcon);
    QIcon saveIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/save.png");
    ui->actionSave->setIcon(saveIcon);
    QIcon deleteIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/delete.png");
    ui->actionClear->setIcon(deleteIcon);

    //colour
    QIcon colourIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/colorValue.png");
    ui->menuColour->setIcon(colourIcon);
    QIcon blackIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/black.png");
    ui->actionblack->setIcon(blackIcon);
    QIcon whiteIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/white.png");
    ui->actionwhite->setIcon(whiteIcon);
    QIcon redIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/red.png");
    ui->actionred->setIcon(redIcon);
    QIcon greenIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/green.png");
    ui->actiongreen->setIcon(greenIcon);
    QIcon blueIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/blue.png");
    ui->actionblue->setIcon(blueIcon);

    //thickness
    QIcon thicknessIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/thickness.png");
    ui->menuThickness->setIcon(thicknessIcon);

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
        painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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


void MainWindow::on_actionSave_triggered()
{
    QString filters = "PNG (*.png);;JPEG (*.jpg *.jpeg);;All Files (*)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить изображение"), "image", filters);
    m_image.save(fileName);
}


void MainWindow::on_actionblack_triggered()
{
    colourPen = (Qt::black);
}


void MainWindow::on_actionwhite_triggered()
{
    colourPen = (Qt::white);
}


void MainWindow::on_actionred_triggered()
{
    colourPen = (Qt::red);
}


void MainWindow::on_actionblue_triggered()
{
    colourPen = (Qt::blue);
}


void MainWindow::on_actiongreen_triggered()
{
    colourPen = (Qt::green);
}


void MainWindow::on_action1px_triggered()
{
    penThickness = 1;
}


void MainWindow::on_action2px_triggered()
{
    penThickness = 2;
}


void MainWindow::on_action3px_triggered()
{
    penThickness = 3;
}


void MainWindow::on_action4px_triggered()
{
    penThickness = 4;
}


void MainWindow::on_action5px_triggered()
{
    penThickness = 5;
}


void MainWindow::on_actioncustom_triggered()
{
    bool ok;
    int number = QInputDialog::getInt(this, tr("Enter a number"), tr("number: "), 0, -2147483647, 2147483647, 1, &ok);

    if(ok){
        penThickness = number;
    }
}

