#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "nutsocket.h"
#include "newconnectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nut::registerSocketFactory(engine::SocketFactory);
    m_threadManager = std::shared_ptr<engine::ThreadManager>(new engine::ThreadManager(this, 4));

    //_thread = new engine::NetworkThread();

    //_thread->start("192.168.1.8", 3493, 4);
}

MainWindow::~MainWindow()
{
    m_threadManager->stop();
    delete ui;
}

void MainWindow::showNewConnectionDialog()
{
    NewConnectionDialog * dlg = new NewConnectionDialog(this);

    //dlg->deleteLater();
    connect(dlg, &NewConnectionDialog::createNewConnection, this, &MainWindow::createNewConnection, Qt::QueuedConnection);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

void MainWindow::createNewConnection(const QString &address, uint16_t port)
{
    m_threadManager->newThread(address, port);
}


void MainWindow::on_actionConnect_triggered()
{
    showNewConnectionDialog();
}
