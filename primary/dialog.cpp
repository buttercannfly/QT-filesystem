#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

void Dialog::receivelogin()
{
    this->show();
}
Dialog::~Dialog()
{
    delete ui;
}
