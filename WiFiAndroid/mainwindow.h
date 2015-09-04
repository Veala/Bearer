#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkSession>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>

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

private:
    Ui::MainWindow *ui;

public slots:
    void getConfigurations();
    void getInterfaces();
    void clearText();

};

#endif // MAINWINDOW_H
