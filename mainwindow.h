#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QInputDialog>

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

private:
    Ui::MainWindow *ui;
    QImage m_image;
    QPoint m_lastPoint;
    QColor colourPen;
    int penThickness;
};
#endif // MAINWINDOW_H
