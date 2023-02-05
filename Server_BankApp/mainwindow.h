#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct Account{
    QString username;
    QString accountNumber;
    QString balance;
    QString name;
    QString bankID;
    QString password;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readAccountsFromFile(QString fileName);
    Account account;
    void withdraw(QString username, QString amount);
public slots:
    void onNewConnected();
    void onReadyRead();
private slots:


private:
    Ui::MainWindow *ui;
    QTcpServer server;
    QList<QTcpSocket*> sockets;
    QList<Account> accountList;
};
#endif // MAINWINDOW_H
