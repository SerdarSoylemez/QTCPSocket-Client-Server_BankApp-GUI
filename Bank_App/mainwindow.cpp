#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),socket(this)
{
    ui->setupUi(this);
    //connect(ui->Connect,SIGNAL(pressed()),this,SLOT(on_Connect_clicked()));
    connect(&socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    //connect(ui->Show,SIGNAL(pressed()),this,SLOT(on_Show_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_Connect_clicked()
{
    username = ui->userName->text();
    password =  ui->Password->text();
    if(username.isEmpty() || password.isEmpty()){
        ui->textEdit->setText("Error connection");
        return;
    }
    else{
        socket.connectToHost(QHostAddress("127.0.0.1"),8080);
        ui->textEdit->setText("Connected");//Sonra hata kodunu yaz.
        QByteArray data;
        data.append(username + ":" + password);
        socket.write(data);
    }
}
void MainWindow::onReadyRead()
{
    socketData = socket.readAll();
    qDebug()<<"Data: "<<socketData;
}
void MainWindow::on_Show_clicked()
{
    QStringList list = QString(socketData).split(":");
    ui->textEdit_2->setText("Name:"+list[0]+"\nBank ID:"+list[1]+"\nAccount Number:"+list[2]+"\nBalance:"+list[3]);
}
void MainWindow::on_Cekme_clicked()
{   QString withdraw="withdraw";
    QString miktar=ui->Miktar_cekme->text();
    QByteArray data;
    data.append(withdraw+":"+username+":"+password+":"+miktar);
    socket.write(data);
    QStringList list= QString(socketData).split(":");
    if (list.size() >= 4) {
      ui->textEdit_4->setText("Name:"+list[0]+"\nBank ID:"+list[1]+"\nAccount Number:"+list[2]+"\nBalance:"+list[3]);
    } else {
      ui->textEdit_4->setText("Error: Incomplete data received from server");
    }
}


void MainWindow::on_Yatirma_clicked()
{
    QString deposit="deposit";
    QString miktar=ui->miktarYatir->text();
    QByteArray data;
    data.append(deposit+":"+username+":"+password+":"+miktar);
    socket.write(data);
    QStringList list= QString(socketData).split(":");
    if (list.size() >= 4) {
        ui->textEdit_5->setText("Name:"+list[0]+"\nBank ID:"+list[1]+"\nAccount Number:"+list[2]+"\nBalance:"+list[3]);
    }
    else {
        ui->textEdit_5->setText("Error: Incomplete data received from server");
    }
}


void MainWindow::on_Transfer_clicked()
{
    QString transfer="transfer";
    QString tAccount=ui->toID->text();
    QString miktar=ui->miktarTran->text();
    QByteArray data;
    data.append(transfer+":"+username+":"+password+":"+tAccount+":"+miktar);
    socket.write(data);
    QStringList list= QString(socketData).split(":");
    if (list.size() >= 8){
         ui->textEdit_3->setText("From:Name:"+list[0]+"\nBank ID:"+list[1]+"\nAccount Number:"+list[2]+"\nBalance:"+list[3]+"\nTo:Name:"+list[4]+"\nBank ID:"+list[5]+"\nAccount Number:"+list[6]+"\nBalance:"+list[7]);
    }
    else{
        ui->textEdit_3->setText("Error!");
    }
}

