#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>

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

private:
    Ui::MainWindow *ui;
    QImage m_image;
    QPoint m_lastPoint;
};
#endif // MAINWINDOW_H
