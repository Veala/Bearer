#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkSession>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkAddressEntry>

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QNetworkConfigurationManager* manager;
    QNetworkAddressEntry entryAddress;
    QTcpServer* tcpServer;
    QTcpSocket* tcpClient;

    int nPort;

private:
    Ui::MainWindow *ui;

public slots:
    void getConfigurations();
    void getInterfaces();
    void clearText();

    void crServer();
    void crClient();

    void newConnectionToServer();
    void readClientSocketOnServer();
    void readServerSocketOnClient();
    void sendToClient(QTcpSocket*,QString);
    void sendToServer(QTcpSocket*,QString);
    void connected();

};

#endif // MAINWINDOW_H
