#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <nutclient.h>
#include "nutsocket.h"
#include "networkthread.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nut::registerSocketFactory(engine::SocketFactory);
    _thread = new engine::NetworkThread();
    _thread->start("192.168.1.8", 3493, 4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

