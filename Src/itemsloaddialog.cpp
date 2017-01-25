#include "itemsloaddialog.h"
#include "ui_itemsloaddialog.h"
#include <qdebug.h>

ItemsLoadDialog::ItemsLoadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemsLoadDialog)
{
    ui->setupUi(this);
    ui->tableWidget->hideColumn(1);
    ui->tableWidget->hideColumn(2);
}

ItemsLoadDialog::~ItemsLoadDialog()
{
    delete ui;
}

void ItemsLoadDialog::SetItems(QString expire, QString num, QString supplier)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(expire));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(num));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(supplier));
}

void ItemsLoadDialog::on_pushButton_clicked()
{
    emit SendBill(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text());
    ItemsLoadDialog::close();
}

void ItemsLoadDialog::on_tableWidget_itemSelectionChanged()
{
    ui->pushButton->setEnabled(true);
}
