#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),server(this)
{
    ui->setupUi(this);
    server.listen(QHostAddress("127.0.0.1"),8080);
    readAccountsFromFile("C:/Users/My/Desktop/QT_/Banka_Account.txt");
    connect(&server,SIGNAL(newConnection()),this,SLOT(onNewConnected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onNewConnected(){
    ui->textEdit->append("On New connection");
    QTcpSocket *clientSocket=server.nextPendingConnection();
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    sockets.push_back(clientSocket);
}
void MainWindow::onReadyRead(){
    ui->textEdit->append("On Ready Read");
    QTcpSocket* s=static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data=s->readAll();
    QStringList list = QString(data).split(":");
    if(list.size()==2){
        QString username = list[0];//Correct print
        QString password = list[1];
        //iterate through the account list and find the account with the matching username and password
        for(int i=0;i<accountList.size();i++){
            if(accountList.at(i).username == username && accountList.at(i).password == password){
                ui->textEdit->setText("Connected");
                QByteArray accountInfo;
                accountInfo.append(accountList.at(i).name + ":" + accountList.at(i).bankID + ":" + accountList.at(i).accountNumber + ":" +accountList.at(i).balance);
                s->write(accountInfo);
                break;
            }
        }
    }
    else if(list[0]=="withdraw"){
        ui->textEdit->append("On Withdraw.Your withdraw amount is: "+list[3]);
        QString username = list[1];//Correct print
        QString password = list[2];
        int amount=list[3].toInt();
        for(int i=0;i<accountList.size();i++){
            if(accountList.at(i).username == username && accountList.at(i).password == password){
                int currentBalance = accountList.at(i).balance.toInt();
                if(currentBalance >= amount){
                    currentBalance -= amount;
                    accountList[i].balance = QString::number(currentBalance);
                    QByteArray accountInfo;
                    accountInfo.append(accountList.at(i).name + ":" + accountList.at(i).bankID + ":" + accountList.at(i).accountNumber + ":" + accountList.at(i).balance);
                    s->write(accountInfo);
                    break;
                }
            }
        }
    }
    else if(list[0]=="deposit"){
        ui->textEdit->append("On Deposit.Your deposit amount is: "+list[3]);
        QString username = list[1];//Correct print
        QString password = list[2];
        int amount=list[3].toInt();
        for(int i=0;i<accountList.size();i++){
            if(accountList.at(i).username == username && accountList.at(i).password == password){
                int currentBalance = accountList.at(i).balance.toInt();
                currentBalance += amount;
                accountList[i].balance = QString::number(currentBalance);
                QByteArray accountInfo;
                accountInfo.append(accountList.at(i).name + ":" + accountList.at(i).bankID + ":" + accountList.at(i).accountNumber + ":" + accountList.at(i).balance);
                s->write(accountInfo);
                break;
            }
        }
    }
    else if(list[0]=="transfer"){
        ui->textEdit->append("On Transfer.Your transfer amount is: "+list[4]);
        QString username = list[1];
        QString password = list[2];
        QString account_Number=list[3];
        int amount=list[4].toInt();
        QString bank_ID;
        QString name2;
        int balance2;
        for(int i=0;i<accountList.size();i++){
            if(accountList.at(i).username == username && accountList.at(i).password == password){
                int currentBalance = accountList.at(i).balance.toInt();
                int sender_index=i;
                for(int j=0;j<accountList.size();j++){
                    if(accountList.at(j).accountNumber==account_Number){
                        bank_ID=accountList.at(j).bankID;
                        balance2=accountList.at(j).balance.toInt();
                        name2=accountList.at(j).name;
                        if(accountList.at(sender_index).bankID==bank_ID){
                            if(currentBalance>=amount){
                                currentBalance-=amount;
                                balance2+=amount;
                            }
                        }
                        else{
                            currentBalance=currentBalance-(amount*1.25);
                            balance2=balance2+(amount*1.25);
                        }
                        accountList[sender_index].balance = QString::number(currentBalance);
                        accountList[j].balance=QString::number(balance2);
                        QByteArray accountInfo;
                        accountInfo.append(accountList.at(sender_index).name + ":" + accountList.at(sender_index).bankID + ":" + accountList.at(sender_index).accountNumber + ":" + accountList.at(sender_index).balance+":"+name2+":"+bank_ID+":"+account_Number+":"+accountList[j].balance);
                        qDebug()<<accountInfo.size();
                        s->write(accountInfo);
                        break;
                    }
                }
            }
        }


    }
}
void MainWindow::readAccountsFromFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error: Could not open file";
        return;
    }
    QTextStream in(&file);
    //int i=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList accountInfo = line.split(" ");
        if (accountInfo.size() != 6){
            qDebug() << "Error: Invalid file format";
            return;
        }
        Account account;
        account.username= accountInfo[0];
        account.name=accountInfo[1];
        account.bankID=accountInfo[2];
        account.accountNumber=accountInfo[3];
        account.password=accountInfo[4];
        account.balance = accountInfo[5];
        accountList.append(account);
        //qDebug()<<accountList.at(i).username;
        //i++;
    }
    ui->textEdit->append("Readed.");

    file.close();
}

