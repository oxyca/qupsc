#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "threadmanager.h"
#include <QMainWindow>
#include <memory>


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
    void showNewConnectionDialog();
    void createNewConnection(const QString & address, uint16_t port);
private slots:
    void on_actionConnect_triggered();

private:

    Ui::MainWindow *ui;
    std::shared_ptr<engine::ThreadManager> m_threadManager;
};
#endif // MAINWINDOW_H
