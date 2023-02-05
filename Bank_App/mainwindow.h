#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include<QString>
#include<QTcpServer>
#include<QTcpSocket>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onReadyRead();
private slots:
    void on_Connect_clicked();
    void on_Show_clicked();

    void on_Cekme_clicked();

    void on_Yatirma_clicked();

    void on_Transfer_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
    QByteArray socketData;
    QString username;
    QString password;
};
#endif // MAINWINDOW_H
