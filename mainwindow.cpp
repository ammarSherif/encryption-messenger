#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    programEdit = false;
    ui->numKey->setValue(3);
    ui->strKey->setText("MONARCHY");
}

MainWindow::~MainWindow()
{
    delete ui;
}
