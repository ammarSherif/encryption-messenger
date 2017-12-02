#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ciphers.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool programEdit;
};

#endif // MAINWINDOW_H
