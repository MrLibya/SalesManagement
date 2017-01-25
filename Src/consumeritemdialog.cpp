#include "consumeritemdialog.h"
#include "ui_consumeritemdialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include "mymsg.h"

ConsumerItemDialog::ConsumerItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsumerItemDialog)
{
    ui->setupUi(this);
    ui->Cart->horizontalHeader()->show();
    ui->delete_item->setShortcut(QApplication::translate("MainWindow", "DELETE", 0));
    ui->Cart->hideColumn(0);
    ui->Cart->hideColumn(5);
    ui->Cart->hideColumn(7);
    ui->Cart->hideColumn(8);

    QSqlQuery query;
    query.exec("SELECT * FROM main.consumer_item_log");
    while(query.next())
    {
        ui->Cart->setRowCount(ui->Cart->rowCount()+1);
        ui->Cart->setItem(ui->Cart->rowCount()-1, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 7, new QTableWidgetItem(query.value(7).toString()));
        ui->Cart->setItem(ui->Cart->rowCount()-1, 8, new QTableWidgetItem(query.value(8).toString()));
    }
}

ConsumerItemDialog::~ConsumerItemDialog()
{
    delete ui;
}

void ConsumerItemDialog::on_Cart_itemSelectionChanged()
{
    ui->delete_item->setEnabled(true);
}

void ConsumerItemDialog::on_delete_item_clicked()
{
    if(YesNoMsg(windowTitle()))
    {
        QSqlQuery query;
        query.exec(QString("DELETE FROM main.consumer_item_log WHERE id = '%1'").arg(ui->Cart->item(ui->Cart->currentRow(),0)->text().toUInt()));
        ui->Cart->removeRow(ui->Cart->currentRow());
        ui->delete_item->setEnabled(false);
    }
}
