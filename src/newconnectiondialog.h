#ifndef NEWCONNECTIONDIALOG_H
#define NEWCONNECTIONDIALOG_H

#include <tuple>
#include <QString>
#include <QDialog>

namespace Ui {
class NewConnectionDialog;
}

class NewConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnectionDialog(QWidget *parent = nullptr);
    ~NewConnectionDialog();
    std::tuple<QString, uint16_t> getAddressPort();
protected slots:
    void accept() override;
signals:
    void createNewConnection(const QString & address, uint16_t port);
private:
    Ui::NewConnectionDialog *ui;
};

#endif // NEWCONNECTIONDIALOG_H
