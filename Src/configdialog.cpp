#include "configdialog.h"
#include "ui_configdialog.h"
#include <QSqlQuery>
#include "mymsg.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec("SELECT shop_name,paper_width,paper_high FROM main.config");
    if(query.first())
    {
        ui->name->setText(query.value(0).toString());
        ui->paper_width->setText(query.value(1).toString());
        ui->paper_high->setText(query.value(2).toString());
    }
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_pushButton_2_clicked()
{
    QSqlQuery query;
    query.exec(QString("UPDATE main.config SET shop_name = '%1', paper_width = '%2', paper_high = '%3'").arg(ui->name->text()).arg(ui->paper_width->text()).arg(ui->paper_high->text()));
    ConfigUpdated();
    ConfigDialog::close();
}
