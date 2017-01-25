#include "logindialog.h"
#include "ui_logindialog.h"
#include <QtSql>
#include "users.h"
//#include "mainwindow.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->label_3->setStyleSheet("QLabel { color : red; }");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

int users::id = 0;
int users::level =0;
QString users::name = "";

void LoginDialog::on_OK_clicked()
{
    QSqlQuery query("SELECT * FROM [main].[users]");
    while (query.next())
    {
        if (query.value(1) == ui->username_input->text() &&
               query.value(2) ==  ui->password_input->text())
        {
            users::id = query.value(0).toInt();
            users::name = ui->username_input->text();
            break;
        }
    }
    if (!users::id)
        ui->label_3->setText("اسم المستخدم او كلمه المرور خاطئه");
    else
    {
        users::level = query.value(3).toInt();
        LoginDialog::close();

        //emit on_SuccessfulLgoin();

    }
}
