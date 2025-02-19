#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QVector>
#include <QMessageBox>
#include <QStack>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QPolygon>
#include <QColorDialog>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void fillingPlace(QImage &image, const QPoint &pos, const QColor &color);
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


private slots:
    void on_actionClear_triggered();

    void on_actionSave_triggered();

    void on_actionblack_triggered();

    void on_actionwhite_triggered();

    void on_actionred_triggered();

    void on_actionblue_triggered();

    void on_actiongreen_triggered();

    void on_action1px_triggered();

    void on_action2px_triggered();

    void on_action3px_triggered();

    void on_action4px_triggered();

    void on_action5px_triggered();

    void on_actioncustom_triggered();

    void on_actionreturn_triggered();

    void on_actionFill_triggered();

    void on_actionpen_triggered();

    void on_actionShapes();


    void on_actioncircle_triggered();

    void on_actionsquare_triggered();

    void on_actiontext_triggered();

    void on_actionpolygon_triggered();

    void on_actioncustom_2_triggered();

    void on_actioneraser_triggered();

    void undoAction();
    void saveAction();

    void on_actionadd_img_triggered();

private:
    Ui::MainWindow *ui;
    QImage m_image;
    QPoint m_lastPoint;
    QColor colourPen;
    int penThickness;
    QVector<QImage> images;
    bool isFill;
    bool isShapeMode;
    int shapeSize;
    QPoint mousePos;
    QString shapeText;
    QVector<QPoint> points;
    bool isImageLoaded;
    QImage loadedImage;
    QPoint imagePos;
};
#endif // MAINWINDOW_H
