#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkConfigurationManager(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(getConfigurations()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(clearText()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(getInterfaces()));
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
        }
    }
    ui->textEdit->append("----------------------QHostInterfaces------------------");

}

void MainWindow::clearText()
{
    ui->textEdit->clear();
}
