#include "users.h"
#include "ui_users.h"
#include <QtSql>
#include <QString>
#include "mymsg.h"

users::users(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::users)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("users");
    model->select();
    ui->tableView->setModel(model);

    ui->toolBox->setCurrentIndex(0);
    ui->lineEdit->setFocus();
}

users::~users()
{
    delete ui;
}

void users::on_pushButton_clicked()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query(QString("INSERT INTO main.users (user, password, level)"
                    "VALUES ('%1', '%2', '%3')").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text())
                    .arg(ui->comboBox->currentIndex()+1));
    AddEmployeeMsg();
    users::close();
    QSqlDatabase::database().commit();
}
