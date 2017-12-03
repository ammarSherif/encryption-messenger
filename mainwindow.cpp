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


void MainWindow::on_msg_textChanged()
{
    if(!programEdit){
        // a human edit
        // show the cipher
        programEdit=true;
        txt=ui->msg->toPlainText().toStdString();
        key=ui->strKey->text().toStdString();
        switch(ui->enAlgorithm->currentIndex()){
        case 0: //caesar
            cipher = caeserEncrypt(txt,ui->numKey->value());
            break;
        case 1:
            cipher = encryptMul(txt,ui->numKey->value(),false);
            break;
        case 2:
            cipher = encryptMul(txt,ui->numKey->value(),true);
            break;
        case 3:
            cipher = playFairEn(txt,key,true);
            break;
        case 4:
            cipher = vigenereCipher(txt,key,true);
            break;
        case 5:
            cipher = oneTimePad(txt,key);
            ui->strKey->setText(QString::fromStdString(key));
            break;
        case 6:
            cipher = railFence(txt,true);
            break;
        case 7:
            cipher = permuteCipher(txt,key,true);
            ui->strKey->setText(QString::fromStdString(key));
            break;
        case 8:
            cipher = txt;
            DES(cipher,key,true);
            break;
        default:
            break;
        }
        ui->cipher->document()->setPlainText(QString::fromStdString(cipher));
    }else
        programEdit=false;
    // the else case means that it is a program edit, i.e. the program itself set
    // that text, so you don't have to cipher the message again as if you do so,
    // the program will enter an infinte loop of
    // cipher the message -> set the cipher in the cipher box
    // cipher box changed so decipher the cipher -> put the message in the msg box
    // and so on
    // so I just do nothing and reset the program edit to its default.
}

void MainWindow::on_cipher_textChanged()
{
    if(!programEdit){
        programEdit=true;
        cipher = ui->cipher->toPlainText().toStdString();
        key = ui->strKey->text().toStdString();
        switch(ui->enAlgorithm->currentIndex()){
        case 0: //caesar
            txt = caeserEncrypt(cipher,-1*ui->numKey->value());
            break;
        case 1:
            txt = decryptMul(cipher,ui->numKey->value(),false);
            break;
        case 2:
            txt = decryptMul(cipher,ui->numKey->value(),true);
            break;
        case 3:
            txt = playFairEn(cipher,key,false);
            break;
        case 4:
            txt = vigenereCipher(cipher,key,false);
            break;
        case 5:
            txt = vigenereCipher(cipher,key,false);
            break;
        case 6:
            txt = railFence(cipher,false);
            break;
        case 7:
            txt = permuteCipher(cipher,key,false);
            break;
        case 8:
            txt = cipher;
            DES(txt,key,false);
            break;
        default:
            break;
        }
        ui->msg->document()->setPlainText(QString::fromStdString(txt));
    } else
        programEdit=false;
}

void MainWindow::on_numKey_valueChanged(int arg1)
{
    if(!programEdit)
        emit ui->msg->textChanged();
}

void MainWindow::on_strKey_textChanged(const QString &arg1)
{
    if(!programEdit)
        emit ui->msg->textChanged();
}

void MainWindow::on_enAlgorithm_currentIndexChanged(int index)
{
    if(!programEdit)
        emit ui->msg->textChanged();
}
