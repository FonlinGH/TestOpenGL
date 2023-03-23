#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actiontriangle_triggered();

    void on_actionWireFrame_triggered();

    void on_actionnonWireFrame_triggered();

    void on_actionclear_triggered();

    void on_actionrect_triggered();

    void on_actioncube_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
