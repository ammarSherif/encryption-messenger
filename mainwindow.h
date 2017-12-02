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

private slots:
    void on_msg_textChanged();

    void on_cipher_textChanged();

    void on_numKey_valueChanged(int arg1);

    void on_strKey_textChanged(const QString &arg1);

    void on_enAlgorithm_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    bool programEdit;
    string txt;
    string cipher;
    string key;
};

#endif // MAINWINDOW_H
