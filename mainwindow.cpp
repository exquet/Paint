#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , colourPen(Qt::black)
    , penThickness(3)
    , isFill(false)
    , isShapeMode(false)
    , shapeSize(50)
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

    //back
    QIcon backIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/back.png");
    ui->menuback->setIcon(backIcon);
    QIcon recoverIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/recover.png");
    ui->actionreturn->setIcon(recoverIcon);

    //pen(fill)
    QIcon penIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/pen.png");
    ui->menufill->setIcon(penIcon);
    ui->actionpen->setIcon(penIcon);
    QIcon fillIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/fill.png");
    ui->actionFill->setIcon(fillIcon);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){ // отслеживание первого нажатия
        m_lastPoint = event->pos();
        images.push_back(m_image);

        if(isFill){ fillingPlace(m_image, m_lastPoint, colourPen); }
        else if(isShapeMode){
            QPainter painter(&m_image);
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            // верхний левый угол ограничивающего прямоугольника вычисляем так, чтобы круг был центром в event->pos()
            painter.drawEllipse(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){ // отслеживание изменения положия при нажатом ЛКМ
        QPainter painter(&m_image);
        if(isShapeMode){
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawEllipse(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
        }
        else{
            // перо: черный цвет, толщина линии 3 пикселя, сплошная линия с закругленными концами
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(m_lastPoint, event->pos()); // отрисовка линии от начального положения до текущего
            m_lastPoint = event->pos(); // обновление положения курсора
        }
        update();
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


void MainWindow::on_actionreturn_triggered()
{
    if(!images.isEmpty()){ 
        m_image = images.last();
        update();
        images.pop_back();
    }
    else{
        QMessageBox::information(this, tr("Error!"), tr("There is no action to return"));
    }
}


void MainWindow::on_actionFill_triggered()
{
    QIcon fillIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/fill.png");
    ui->menufill->setIcon(fillIcon);
    isFill = true;
    isShapeMode = false;
}


void MainWindow::on_actionpen_triggered()
{
    QIcon penIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/pen.png");
    ui->menufill->setIcon(penIcon);
    isFill = false;
    isShapeMode = false;
}

void MainWindow::fillingPlace(QImage &image, const QPoint &pos, const QColor &color){
    QColor targetColor = image.pixelColor(pos);
    if(color == targetColor){return;}

    QStack<QPoint> stack;
    stack.push(pos);

    while(!stack.isEmpty()){
        QPoint pt = stack.pop(); // извлекает точку из stack

        // проверяем, что точка находится внутри границ изображения
        if (pt.x() < 0 || pt.x() >= image.width() || pt.y() < 0 || pt.y() >= image.height()){continue;}

        QColor currencyColor = image.pixelColor(pt);
        if(currencyColor == targetColor){
            image.setPixelColor(pt, color); // замена цвета

            // добавление соседних пикселей в стек
            stack.push(QPoint(pt.x()+1, pt.y()));
            stack.push(QPoint(pt.x()-1, pt.y()));
            stack.push(QPoint(pt.x(), pt.y()+1));
            stack.push(QPoint(pt.x(), pt.y()-1));
        }
        update();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (event->size().width() > m_image.width() || event->size().height() > m_image.height())
    {
        QImage newImage(event->size(), QImage::Format_RGB32);
        newImage.fill(Qt::white);
        QPainter painter(&newImage);
        painter.drawImage(QPoint(0, 0), m_image);
        m_image = newImage;

        for(QImage &img : images){
            img = img.scaled(event->size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
    }
    QWidget::resizeEvent(event);
}



void MainWindow::on_actioncircle_triggered()
{
    bool ok;
    int size = QInputDialog::getInt(this, tr("Выберите размер фигуры"), tr("Размер:"), shapeSize, 1, 2500, 1, &ok);
    if(ok){
        shapeSize = size;
        isShapeMode = true;
    }
}

