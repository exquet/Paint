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
        m_image = QImage(800, 600, QImage::Format_ARGB32); // создание холста
        m_image.fill(Qt::transparent);

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
        QIcon eraserIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/eraser.png");
        ui->actioneraser->setIcon(eraserIcon);

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

        //utilities
        QIcon utilitiesIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/utilities.png");
        ui->menuutilities->setIcon(utilitiesIcon);
        QIcon gridIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/grid.png");
        ui->actiongrid->setIcon(gridIcon);
        QIcon imageIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/image.png");
        ui->actionadd_img->setIcon(imageIcon);

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
        if(event->button() == Qt::LeftButton) {
            // проверяем, нажата ли область одного из загруженных изображений
            for (int i = loadedImages.size() - 1; i >= 0; i--) {
                LoadedImage &img = loadedImages[i];
                QRect imageRect(img.pos, img.image.size());
                if(imageRect.contains(event->pos())){
                    img.dragging = true;
                    img.dragOffset = event->pos() - img.pos;
                    // перемещаем выбранное изображение в конец вектора, чтобы оно отображалось поверх остальных
                    loadedImages.move(i, loadedImages.size()-1);
                    update();
                    return; // если нажата область изображения, не выполняем остальной код
                }
            }

            m_lastPoint = event->pos();
            images.push_back(m_image);
            QPainter painter(&m_image);
            painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

            if(isFill){ fillingPlace(m_image, m_lastPoint, colourPen); }
            if(isShapeMode && ui->actioncircle->isChecked()) {
                painter.drawEllipse(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
                update();
            }
            if(isShapeMode && ui->actionsquare->isChecked()) {
                painter.drawRect(event->pos().x() - shapeSize/2, event->pos().y() - shapeSize/2, shapeSize, shapeSize);
                update();
            }
            if(isShapeMode && ui->actiontext->isChecked()) {
                QFont font = painter.font();
                font.setPointSize(shapeSize);
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

        for (int i = 0; i < loadedImages.size(); i++) {
            if(loadedImages[i].dragging) {
                loadedImages[i].pos = event->pos() - loadedImages[i].dragOffset;
                update();
                return; // если перетаскивается изображение, остальной код не выполняется
            }
        }

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
            else if(ui->actioneraser->isChecked()) {
                painter.setPen(QPen(Qt::white, penThickness*1.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.setBrush(Qt::white);
                painter.drawEllipse(event->pos().x() - penThickness/2, event->pos().y() - penThickness/2, penThickness, penThickness);
            }

            else{
                // перо: черный цвет, толщина линии 3 пикселя, сплошная линия с закругленными концами
                painter.setPen(QPen(colourPen, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawLine(m_lastPoint, event->pos()); // отрисовка линии от начального положения до текущего
                m_lastPoint = event->pos(); // обновление положения курсора
            }
        }
        update();
    }

    void MainWindow::mouseReleaseEvent(QMouseEvent *event){ // отпускание ЛКМ
        if(event->button() == Qt::LeftButton){
            for (int i = 0; i < loadedImages.size(); i++) {
                loadedImages[i].dragging = false;
            }
        }
        QMainWindow::mouseReleaseEvent(event);
    }


    void MainWindow::paintEvent(QPaintEvent *event){
        QPainter painter(this);

        // this->rect() возвращает QRect(прямоугольную область) текущего виджета
        painter.fillRect(this->rect(), Qt::white);

        if(ui->actiongrid->isChecked()){
            QPen gridPen(Qt::lightGray);
            gridPen.setStyle(Qt::DashLine);
            gridPen.setWidth(1);
            painter.setPen(gridPen);

            int gridSpacing = 20;  // интервал

            int widgetWidth = this->width();
            int widgetHeight = this->height();
            // вертикальные линии
            for (int x = 0; x < widgetWidth; x += gridSpacing){
                painter.drawLine(x, 0, x, widgetHeight);
            }
            // горизонтальные линии
            for (int y = 0; y < widgetHeight; y += gridSpacing){
                painter.drawLine(0, y, widgetWidth, y);
            }
        }

        painter.drawImage(0, 0, m_image); // отрисовка изображения

        QPen previewPen(Qt::gray);
        previewPen.setStyle(Qt::SolidLine);
        previewPen.setWidth(1);
        painter.setPen(previewPen);
        painter.setBrush(Qt::NoBrush);

        for(const LoadedImage &img : loadedImages) { // рисование всех изображений
            painter.drawImage(img.pos, img.image);
        }

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
        saveAction();
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
        undoAction();
    }


    void MainWindow::on_actionFill_triggered()
    {
        QIcon fillIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/fill.png");
        ui->menufill->setIcon(fillIcon);
        isFill = true;
        isShapeMode = false;

        ui->actionFill->setChecked(true);
        ui->actionpen->setChecked(false);
        ui->actioneraser->setChecked(false);
    }


    void MainWindow::on_actionpen_triggered()
    {
        QIcon penIcon("C:/Users/dimat/Documents/QT projects/Paint/Icons/pen.png");
        ui->menufill->setIcon(penIcon);
        isFill = false;
        isShapeMode = false;

        ui->actionFill->setChecked(false);
        ui->actionpen->setChecked(true);
        ui->actioneraser->setChecked(false);
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
            QImage newImage(event->size(), QImage::Format_ARGB32);
            newImage.fill(Qt::transparent);
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
        if (event->modifiers() == Qt::ControlModifier) {
            if(event->key() == Qt::Key_Z) {
                undoAction();
            }
            if(event->key() == Qt::Key_S) {
                saveAction();
            }
        }
        else {
            QMainWindow::keyPressEvent(event);
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


    void MainWindow::on_actioneraser_triggered()
    {
        ui->actionFill->setChecked(false);
        ui->actionpen->setChecked(false);
        ui->actioneraser->setChecked(true);

        isFill = false;
        isShapeMode = false;
    }

    void MainWindow::undoAction() {
        if(!images.isEmpty()){
            m_image = images.last();
            update();
            images.pop_back();
        }
        else{
            QMessageBox::information(this, tr("Error!"), tr("There is no action to return"));
        }
    }

    void MainWindow::saveAction() {
        ui->actiongrid->setChecked(false);
        QString filters = "PNG (*.png);;JPEG (*.jpg *.jpeg);;All Files (*)";
        QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить изображение"), "image", filters);

        QImage saveImage(m_image.size(), QImage::Format_RGB32); // новое изображение
        saveImage.fill(Qt::white);
        QPainter painter(&saveImage);
        painter.drawImage(0, 0, m_image); // отрисовка рисунка пользователя

        saveImage.save(fileName);
        ui->actiongrid->setChecked(true);
    }

    void MainWindow::on_actionadd_img_triggered()
    {
        QString filters = "All Files (*) ;;JPEG (*.jpg *.jpeg) ;;PNG (*.png)";
        QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите изображение"), "", filters);

        if(fileName.isEmpty())
            return;

        QImage img;
        if(!img.load(fileName)) {
            QMessageBox::warning(this, tr("Ошибка загрузки"), tr("Не удалось загрузить изображение"));
            return;
        }

        img = img.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        LoadedImage newImage;
        newImage.image = img;
        newImage.pos = QPoint(0, 0);
        newImage.dragging = false;

        loadedImages.push_back(newImage);

        update();
    }

    void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
        // перебираем все загруженные изображения в обратном порядке,
        // чтобы при наложении (overlap) самое верхнее изображение реагировало первым.
        for (int i = loadedImages.size() - 1; i >= 0; i--) {
            LoadedImage &img = loadedImages[i];
            QRect imageRect(img.pos, img.image.size()); // определяем область изображения на холсте
            if (imageRect.contains(event->pos())) {
                QMenu menu(this);
                QAction *resizeAction = menu.addAction("Изменить размер");
                QAction *deleteAction = menu.addAction("Удалить");
                // отображаем меню по глобальной позиции курсора и ожидаем выбор пользователя
                QAction *selectedAction = menu.exec(event->globalPos());
                if (selectedAction == resizeAction) {
                    bool ok;
                    int newWidth = QInputDialog::getInt(this, "Изменить размер", "Новая ширина:",
                                                         img.image.width(), 1, 10000, 1, &ok);
                    if (!ok) return; // если пользователь отменил ввод - выходим из метода
                    int newHeight = QInputDialog::getInt(this, "Изменить размер", "Новая высота:",
                                                         img.image.height(), 1, 10000, 1, &ok);
                    if (!ok) return;

                    img.image = img.image.scaled(newWidth, newHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    update();
                }

                if (selectedAction == deleteAction) {
                    QMessageBox::StandardButton delMessage;
                    delMessage = QMessageBox::question(this, "Подтверждение", "Вы уверены?",
                                                  QMessageBox::Yes | QMessageBox::No);

                    if (delMessage == QMessageBox::Yes) {
                        loadedImages.removeAt(i);
                    }
                    else {
                        return;
                    }
                }


                // после обработки первого найденного изображения выходим из метода
                return;
            }
        }
        QMainWindow::contextMenuEvent(event);
    }

