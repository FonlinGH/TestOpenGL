#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actiontriangle_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Triangle);
}

void MainWindow::on_actionWireFrame_triggered()
{
    ui->openGLWidget->setWireFrame(true);
}

void MainWindow::on_actionnonWireFrame_triggered()
{
    ui->openGLWidget->setWireFrame(false);
}

void MainWindow::on_actionclear_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::None);
}

void MainWindow::on_actionrect_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Rect);
}

void MainWindow::on_actioncube_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Cube);
}
