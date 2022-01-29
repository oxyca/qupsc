#include "newconnectiondialog.h"
#include "ui_newconnectiondialog.h"

NewConnectionDialog::NewConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnectionDialog)
{
    ui->setupUi(this);
}

NewConnectionDialog::~NewConnectionDialog()
{
    delete ui;
}

std::tuple<QString, uint16_t> NewConnectionDialog::getAddressPort()
{
    return std::make_tuple(ui->addressLineEdit->text(), ui->portSpinBox->value());
}

void NewConnectionDialog::accept()
{
    emit createNewConnection(ui->addressLineEdit->text(), ui->portSpinBox->value());
    QDialog::accept();
}
