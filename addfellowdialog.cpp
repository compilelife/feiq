#include "addfellowdialog.h"
#include "ui_addfellowdialog.h"
#include <QHostAddress>
#include <QMessageBox>

AddFellowDialog::AddFellowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFellowDialog)
{
    ui->setupUi(this);
    connect(ui->okBtn, SIGNAL(clicked(bool)), this, SLOT(onOkClicked()));
}

AddFellowDialog::~AddFellowDialog()
{
    delete ui;
}

QString AddFellowDialog::getIp()
{
    return ui->ipEdit->text();
}

void AddFellowDialog::onOkClicked()
{
    auto ip = ui->ipEdit->text();
    if (isValidIp(ip))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, "ip地址无效", "要添加的ip地址无效");
    }
}

bool AddFellowDialog::isValidIp(const QString &ip)
{
    QHostAddress address;
    return address.setAddress(ip);
}
