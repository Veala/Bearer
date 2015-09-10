#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nPort = 3387;
    manager = new QNetworkConfigurationManager(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(getConfigurations()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(clearText()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(getInterfaces()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(crServer()));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(crClient()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getConfigurations()
{
    QList<QNetworkConfiguration> configlist = manager->allConfigurations();
    ui->textEdit->append("----------------------QNetworkConfiguration---------------------");
    for (int i=0; i<configlist.size(); i++) {
        ui->textEdit->append(    "\n name(): "            + configlist[i].name()
                               + "\n state(): "           + QString::number(configlist[i].state())
                               + "\n bearerType(): "      + QString::number(configlist[i].bearerType())
                               + "\n bearerTypeName(): "  + configlist[i].bearerTypeName()
                               + "\n isValid(): "         + QString::number(configlist[i].isValid())
                               + "\n purpose(): "         + QString::number(configlist[i].purpose())
                               + "\n type(): "            + QString::number(configlist[i].type())
                            );
    }
    QNetworkConfiguration defaultConfig = manager->defaultConfiguration();
    ui->textEdit->append(    "\n default name(): "            + defaultConfig.name()
                           + "\n default state(): "           + QString::number(defaultConfig.state())
                           + "\n default bearerType(): "      + QString::number(defaultConfig.bearerType())
                           + "\n default bearerTypeName(): "  + defaultConfig.bearerTypeName()
                           + "\n default isValid(): "         + QString::number(defaultConfig.isValid())
                           + "\n default purpose(): "         + QString::number(defaultConfig.purpose())
                           + "\n default type(): "            + QString::number(defaultConfig.type())
                        );
    ui->textEdit->append("----------------------QNetworkConfiguration---------------------");

}

void MainWindow::getInterfaces()
{

    ui->textEdit->append("----------------------QHostAddress---------------------");
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (int i=0; i<addresses.size(); i++) {
        ui->textEdit->append(    "\n protocol(): "     + QString::number(addresses[i].protocol())
                               + "\n address: "        + addresses[i].toString()
                               + "\n scopeId(): "      + addresses[i].scopeId()
                            );
    }
    ui->textEdit->append("----------------------QHostAddress---------------------");

    ui->textEdit->append("----------------------QHostInterfaces------------------");
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (int i=0; i<interfaces.size(); i++) {
        ui->textEdit->append(    "\n name(): "                     + interfaces[i].name()
                                 + "\n hardwareAddress(): "        + interfaces[i].hardwareAddress()
                                 + "\n humanReadableName(): "      + interfaces[i].humanReadableName()
                                 + "\n isValid(): "                + QString::number(interfaces[i].isValid())
                                 + "\n NumAddressEntry(): "        + QString::number(interfaces[i].addressEntries().size())
                                 + "\n IsUp: "                     + QString::number(interfaces[i].flags().testFlag(QNetworkInterface::IsUp))
                                 + "\n IsRunning: "                + QString::number(interfaces[i].flags().testFlag(QNetworkInterface::IsRunning))
                                 + "\n CanBroadcast: "             + QString::number(interfaces[i].flags().testFlag(QNetworkInterface::CanBroadcast))
                                 + "\n IsLoopBack: "               + QString::number(interfaces[i].flags().testFlag(QNetworkInterface::IsLoopBack))
                                 + "\n IsPointToPoint: "           + QString::number(interfaces[i].flags().testFlag(QNetworkInterface::IsPointToPoint))
                                 + "\n CanMulticast: "             + QString::number(interfaces[i].flags().testFlag(QNetworkInterface::CanMulticast))
                            );
        foreach (QNetworkAddressEntry addressEntry, interfaces[i].addressEntries()) {
            ui->textEdit->append(   "\n ip(): "                  + addressEntry.ip().toString()
                                    + "\n netmask(): "           + addressEntry.netmask().toString()
                                    + "\n broadcast(): "         + addressEntry.broadcast().toString()
                                );
            if (addressEntry.ip().toString().split(".").size()==4) {
                if (addressEntry.ip().toString().split(".").at(2)=="0") {
                    if (addressEntry.ip().toString().split(".").at(0)=="192") {
                        entryAddress = addressEntry;
                    }
                }
            }
        }
    }
    ui->textEdit->append("----------------------QHostInterfaces------------------");

}

void MainWindow::clearText()
{
    ui->textEdit->clear();
}

void MainWindow::crServer()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectionToServer()));
    if(!tcpServer->listen(QHostAddress::Any,nPort))
        ui->textEdit->append(tcpServer->errorString());

}

void MainWindow::newConnectionToServer()
{
    QTcpSocket* tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readClientSocketOnServer()));
    sendToClient(tcpSocket,"Hello from Server, your ip: " + tcpSocket->peerAddress().toString() + "\n");
}

void MainWindow::readClientSocketOnServer()
{
    QTcpSocket* tcpSocket = (QTcpSocket*)sender();
    while (tcpSocket->canReadLine()) {
        ui->textEdit->append(QString(tcpSocket->readAll()));
    }
    //sendToClient(tcpSocket,"Your ip from server: " + tcpSocket->peerAddress().toString() + "\n");
}

void MainWindow::sendToClient(QTcpSocket* tcpSocket, QString message)
{
    tcpSocket->write(QByteArray().append(message));
}

//---------------------------------------------------------------------------

void MainWindow::crClient()
{
    tcpClient = new QTcpSocket(this);
    connect(tcpClient,SIGNAL(connected()),this,SLOT(connected()));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readServerSocketOnClient()));
    ui->textEdit->append("My address: " + entryAddress.ip().toString());
    ui->textEdit->append("My broadcast: " + entryAddress.broadcast().toString());
    //tcpClient->connectToHost(QHostAddress::Broadcast,nPort);
    tcpClient->connectToHost(QHostAddress::Broadcast,nPort);
}

void MainWindow::readServerSocketOnClient()
{
    QTcpSocket* tcpSocket = (QTcpSocket*)sender();
    while (tcpSocket->canReadLine()) {
        ui->textEdit->append(QString(tcpSocket->readAll()));
    }
    sendToServer(tcpSocket,"Your ip from client: " + tcpSocket->peerAddress().toString() + "\n");
}

void MainWindow::sendToServer(QTcpSocket* tcpSocket, QString message)
{
    tcpSocket->write(QByteArray().append(message));
}

void MainWindow::connected()
{
    ui->textEdit->append("I am connected");
}
