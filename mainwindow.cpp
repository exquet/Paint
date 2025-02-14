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

    //shapes
    QIcon shapesIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/shapes.png");
    ui->menushapes->setIcon(shapesIcon);
    QIcon circleIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/circle.png");
    ui->actioncircle->setIcon(circleIcon);
    QIcon squareIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/square.png");
    ui->actionsquare->setIcon(squareIcon);
    QIcon textIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/text.png");
    ui->actiontext->setIcon(textIcon);
    QIcon polygonIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/polygon.png");
    ui->actionpolygon->setIcon(polygonIcon);

    //window
    QIcon logoIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/logo.png");
    MainWindow::setWindowIcon(logoIcon);

    setMouseTracking(true);
    if (centralWidget()){
        centralWidget()->setMouseTracking(true);
    }

    connect(ui->actioncircle, &QAction::triggered, this, &MainWindow::on_actionShapes);
    connect(ui->actionsquare, &QAction::triggered, this, &MainWindow::on_actionShapes);
    connect(ui->actiontext, &QAction::triggered, this, &MainWindow::on_actionShapes);

    setFocusPolicy(Qt::StrongFocus);

    ui->enter_label->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){ // отслеживание первого нажатия
        m_lastPoint = event->pos();
        images.push_back(m_image);
        QPainter painter(&m_image);
        painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        if(isFill){ fillingPlace(m_image, m_lastPoint, colourPen); }
        if(isShapeMode && ui->actioncircle->isChecked()) {
            // верхний левый угол ограничивающего прямоугольника вычисляем так, чтобы круг был центром в event->pos()
            painter.drawEllipse(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
            update();
        }
        if(isShapeMode && ui->actionsquare->isChecked()) {
            painter.drawRect(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
            update();
        }
        if(isShapeMode && ui->actiontext->isChecked()) {
            QFont font = painter.font();
            font.setPointSize(shapeSize); // size
            painter.setFont(font);
            painter.drawText(event->pos().x(), event->pos().y(), shapeText);
            update();
        }
        if(isShapeMode && ui->actionpolygon->isChecked()) {
            points.push_back(event->pos());

            if(points.size() >= 2) {
                ui->enter_label->show();
            }
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    mousePos = event->pos();

    if(event->buttons() == Qt::LeftButton){ // отслеживание изменения положия при нажатом ЛКМ
        QPainter painter(&m_image);
        if(isShapeMode && ui->actioncircle->isChecked()) {
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawEllipse(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
        }
        else if(isShapeMode && ui->actionsquare->isChecked()) {
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawRect(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
        }
        else if(isShapeMode && ui->actiontext->isChecked()) {
            QFont font = painter.font();
            font.setPointSize(shapeSize);
            painter.setFont(font);
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawText(event->pos().x(), event->pos().y(), shapeText);
        }
        else if(isShapeMode && ui->actionpolygon->isChecked()){}
        else if(isFill) {}

        else{
            // перо: черный цвет, толщина линии 3 пикселя, сплошная линия с закругленными концами
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(m_lastPoint, event->pos()); // отрисовка линии от начального положения до текущего
            m_lastPoint = event->pos(); // обновление положения курсора
        }
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(0, 0, m_image); // отрисовка изображения

    QPen previewPen(Qt::gray);
    previewPen.setStyle(Qt::SolidLine);
    previewPen.setWidth(1);
    painter.setPen(previewPen);
    painter.setBrush(Qt::NoBrush);

    if(isShapeMode && ui->actioncircle->isChecked()){
        painter.drawEllipse(mousePos.x() - shapeSize/2, mousePos.y() - shapeSize/2, shapeSize, shapeSize);
    }
    if(isShapeMode && ui->actionsquare->isChecked()){
        painter.drawRect(mousePos.x() - shapeSize/2, mousePos.y() - shapeSize/2, shapeSize, shapeSize);
    }
    if(isShapeMode && ui->actiontext->isChecked()){
        QFont font = painter.font();
        font.setPointSize(shapeSize);
        painter.setFont(font);
        painter.drawText(mousePos.x(), mousePos.y(), shapeText);
    }
    if(isShapeMode && ui->actionpolygon->isChecked()) {
        painter.drawPolygon(points);
    }
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
    ui->actionblack->setChecked(true);
    ui->actionwhite->setChecked(false);
    ui->actionblue->setChecked(false);
    ui->actiongreen->setChecked(false);
    ui->actionred->setChecked(false);
    ui->actioncustom_2->setChecked(false);
}


void MainWindow::on_actionwhite_triggered()
{
    colourPen = (Qt::white);
    ui->actionblack->setChecked(false);
    ui->actionwhite->setChecked(true);
    ui->actionblue->setChecked(false);
    ui->actiongreen->setChecked(false);
    ui->actionred->setChecked(false);
    ui->actioncustom_2->setChecked(false);
}


void MainWindow::on_actionred_triggered()
{
    colourPen = (Qt::red);
    ui->actionblack->setChecked(false);
    ui->actionwhite->setChecked(false);
    ui->actionblue->setChecked(false);
    ui->actiongreen->setChecked(false);
    ui->actionred->setChecked(true);
    ui->actioncustom_2->setChecked(false);
}


void MainWindow::on_actionblue_triggered()
{
    colourPen = (Qt::blue);
    ui->actionblack->setChecked(false);
    ui->actionwhite->setChecked(false);
    ui->actionblue->setChecked(true);
    ui->actiongreen->setChecked(false);
    ui->actionred->setChecked(false);
    ui->actioncustom_2->setChecked(false);
}


void MainWindow::on_actiongreen_triggered()
{
    colourPen = (Qt::green);
    ui->actionblack->setChecked(false);
    ui->actionwhite->setChecked(false);
    ui->actionblue->setChecked(false);
    ui->actiongreen->setChecked(true);
    ui->actionred->setChecked(false);
    ui->actioncustom_2->setChecked(false);
}


void MainWindow::on_action1px_triggered()
{
    penThickness = 1;
    ui->action1px->setChecked(true);
    ui->action2px->setChecked(false);
    ui->action3px->setChecked(false);
    ui->action4px->setChecked(false);
    ui->action5px->setChecked(false);
}


void MainWindow::on_action2px_triggered()
{
    penThickness = 2;
    ui->action1px->setChecked(false);
    ui->action2px->setChecked(true);
    ui->action3px->setChecked(false);
    ui->action4px->setChecked(false);
    ui->action5px->setChecked(false);
}


void MainWindow::on_action3px_triggered()
{
    penThickness = 3;
    ui->action1px->setChecked(false);
    ui->action2px->setChecked(false);
    ui->action3px->setChecked(true);
    ui->action4px->setChecked(false);
    ui->action5px->setChecked(false);
}


void MainWindow::on_action4px_triggered()
{
    penThickness = 4;
    ui->action1px->setChecked(false);
    ui->action2px->setChecked(false);
    ui->action3px->setChecked(false);
    ui->action4px->setChecked(true);
    ui->action5px->setChecked(false);
}


void MainWindow::on_action5px_triggered()
{
    penThickness = 5;
    ui->action1px->setChecked(false);
    ui->action2px->setChecked(false);
    ui->action3px->setChecked(false);
    ui->action4px->setChecked(false);
    ui->action5px->setChecked(true);
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

    ui->actionFill->setChecked(true);
    ui->actionpen->setChecked(false);
}


void MainWindow::on_actionpen_triggered()
{
    QIcon penIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/pen.png");
    ui->menufill->setIcon(penIcon);
    isFill = false;
    isShapeMode = false;

    ui->actionFill->setChecked(false);
    ui->actionpen->setChecked(true);
}

void MainWindow::fillingPlace(QImage &image, const QPoint &pos, const QColor &color){
    QColor targetColor = image.pixelColor(pos);
    if(color == targetColor){return;}

    QStack<QPoint> stack;
    stack.push(pos);

    while(!stack.isEmpty()) {
        QPoint pt = stack.pop(); // извлекает точку из stack

        // проверяем, что точка находится внутри границ изображения
        if (pt.x() < 0 || pt.x() >= image.width() || pt.y() < 0 || pt.y() >= image.height()){continue;}

        QColor currencyColor = image.pixelColor(pt);
        if(currencyColor == targetColor) {
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

        for(QImage &img : images) {
            img = img.scaled(event->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }
    QWidget::resizeEvent(event);
}



void MainWindow::on_actionShapes()
{
    if(ui->actioncircle->isChecked() || ui->actionsquare->isChecked() || ui->actiontext->isChecked() || ui->actionpolygon->isChecked()) {
        bool ok;
        int size = QInputDialog::getInt(this, tr("Выберите размер"), tr("Размер:"), shapeSize, 1, 2500, 1, &ok);
        if(ok) {
            shapeSize = size;
            isShapeMode = true;
        }
    }
}

void MainWindow::on_actioncircle_triggered()
{
    ui->actioncircle->setChecked(true);
    ui->actionsquare->setChecked(false);
    ui->actiontext->setChecked(false);
    ui->actionpolygon->setChecked(false);
}


void MainWindow::on_actionsquare_triggered()
{
    ui->actioncircle->setChecked(false);
    ui->actionsquare->setChecked(true);
    ui->actiontext->setChecked(false);
    ui->actionpolygon->setChecked(false);
}


void MainWindow::on_actiontext_triggered()
{
    ui->actioncircle->setChecked(false);
    ui->actionsquare->setChecked(false);
    ui->actiontext->setChecked(true);
    ui->actionpolygon->setChecked(false);

    bool ok;

    shapeText = QInputDialog::getText(this, tr("Введите текст"), tr("Text: "),
        QLineEdit::Normal, QString(), &ok);

    if(ok && shapeText.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), "Текст пустой!");
        ui->actiontext->setChecked(false);
    }

}


void MainWindow::on_actionpolygon_triggered()
{
    ui->actioncircle->setChecked(false);
    ui->actionsquare->setChecked(false);
    ui->actiontext->setChecked(false);
    ui->actionpolygon->setChecked(true);

    isShapeMode = true;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(points.size() >= 2){
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            QPainter painter(&m_image);
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);

            QPolygon polygon(points);
            painter.drawPolygon(polygon);
            update();

            points.clear();
            ui->enter_label->hide();
        }
        else {
            // если клавиша не Enter, вызываем стандартную обработку
            QMainWindow::keyPressEvent(event);
        }
    }
    else {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Выбранно менее 2 точек"));
    }
}


void MainWindow::on_actioncustom_2_triggered()
{
    QColor selectColor = QColorDialog::getColor(Qt::white, this, "Выберите цвет");

    if (selectColor.isValid()) {
        colourPen = selectColor;
    }

    ui->actioncustom_2->setChecked(true);
    ui->actionblack->setChecked(false);
    ui->actionwhite->setChecked(false);
    ui->actionblue->setChecked(false);
    ui->actiongreen->setChecked(false);
    ui->actionred->setChecked(false);
}

