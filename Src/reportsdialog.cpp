#include "reportsdialog.h"
#include "ui_reportsdialog.h"
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>

ReportsDialog::ReportsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportsDialog)
{
    ui->setupUi(this);
    ui->user->addItem("كل الموظفين");
    QSqlQuery query;
    query.exec("SELECT name FROM main.users");
    while (query.next())
        ui->user->addItem(query.value(0).toString());

    query.exec("SELECT name FROM main.clients");
    while (query.next())
        ui->client_name->addItem(query.value(0).toString());
    ui->fromdate->setDate(QDate::currentDate());
    ui->todate->setDate(QDate::currentDate());
    ui->fromdate_2->setDate(QDate::currentDate());
    ui->todate_2->setDate(QDate::currentDate());
    HeadsTableView = new QStringList;
    CartItems = new QSqlQueryModel(this);
    ui->toolBox->setCurrentIndex(0);
}

ReportsDialog::~ReportsDialog()
{
    delete ui;
    delete HeadsTableView;
}

void ReportsDialog::on_pushButton_clicked()
{
    ui->fromdate->setTime(QTime::fromString("12:00:00 AM","hh:mm:ss AP"));
    ui->todate->setTime(QTime::fromString("11:59:59 PM","hh:mm:ss AP"));
    ui->total->setText("0.000");
    if(ui->user->currentIndex() == 0)
    {
        if(ui->cash_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_cash_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'NO'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
        else if (ui->later_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'NO'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
        else if (ui->refund_cash_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_cash_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'YES'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
        else if (ui->refund_later_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'YES'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
        else if (ui->consumer_items->isChecked())
            CartItems->setQuery(QString("SELECT * from main.consumer_item_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2')").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
        else if (ui->retention_items->isChecked())
            CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'NO'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
        else if (ui->unretention_items->isChecked())
            CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'YES'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }
    else
    {
        if(ui->cash_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_cash_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND whos = '%3' AND refund = 'NO'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));
        else if (ui->later_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND whos = '%3' AND refund = 'NO'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));
        else if (ui->refund_cash_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_cash_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND whos = '%3' AND refund = 'YES'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));
        else if (ui->refund_later_sell_type->isChecked())
            CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND whos = '%3' AND refund = 'YES'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));
        else if (ui->consumer_items->isChecked())
            CartItems->setQuery(QString("SELECT * from main.consumer_item_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND whos = '%3'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));
        else if (ui->retention_items->isChecked())
            CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'NO' AND whos = '%3'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));
        else if (ui->unretention_items->isChecked())
            CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND refund = 'YES' AND whos = '%3'").arg(ui->fromdate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->user->currentText()));

    }

    for(int i=0;i<HeadsTableView->count();i++)
        CartItems->setHeaderData(i, Qt::Horizontal, HeadsTableView->at(i));
    ui->tableView->setModel(CartItems);
    for(int i=0;i<CartItems->rowCount();i++)
        ui->total->setText(QString::number(ui->total->text().toDouble() + CartItems->data(CartItems->index(i,7)).toDouble()));

}

void ReportsDialog::on_cash_sell_type_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه" ;
    ui->pushButton->setEnabled(true);
}

void ReportsDialog::on_later_sell_type_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "رقم العميل" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه";
    ui->pushButton->setEnabled(true);
}

void ReportsDialog::on_refund_cash_sell_type_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه" ;
    ui->pushButton->setEnabled(true);
}

void ReportsDialog::on_refund_later_sell_type_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "رقم العميل" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه";
    ui->pushButton->setEnabled(true);
}

void ReportsDialog::on_consumer_items_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "الرقم التعريفي" << "العنوان" << "الصلاحيه" << "الكميه" << "الموظف" << "رقم فاتورة الصنف" << "التاريخ" << "رقم المورد للفاتورة" << "الباركود";
    ui->pushButton->setEnabled(true);
}

void ReportsDialog::on_retention_items_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "رقم العميل" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه";
    ui->pushButton->setEnabled(true);
}

void ReportsDialog::on_client_later_sell_type_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "رقم العميل" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه";
    ui->pushButton_3->setEnabled(true);
    ui->client_name->setEnabled(true);
    ui->bill_num->setEnabled(true);
    on_client_name_currentIndexChanged(ui->client_name->currentIndex());
}

void ReportsDialog::on_client_debt_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "الرقم التعريفي" << "رقم العميل" << "المبلغ الذي عليه" << "المبلغ المدفوع" << "التاريخ" << "السبب";
    ui->pushButton_3->setEnabled(true);
    ui->client_name->setEnabled(true);
    ui->bill_num->setEnabled(false);
    on_client_name_currentIndexChanged(ui->client_name->currentIndex());
}

void ReportsDialog::on_client_retention_clicked()
{
    HeadsTableView->clear();
    *HeadsTableView << "رقم الفاتورة" << "العنوان" << "الباركود" << "السعر" << "الكميه" << "التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة الصنف" << "الموظف" << "التاريخ" << "رقم العميل" << "اعادة" << "رقم المورد" << "تاريخ الصلاحيه";
    ui->pushButton_3->setEnabled(true);
    ui->client_name->setEnabled(true);
    ui->bill_num->setEnabled(true);
    on_client_name_currentIndexChanged(ui->client_name->currentIndex());
}

void ReportsDialog::on_client_name_currentIndexChanged(int index)
{
    ui->bill_num->clear();
    ui->bill_num->addItem("كل الفواتير");
    QSqlQuery query;
    if(ui->client_later_sell_type->isChecked())
        query.exec(QString("SELECT cart_id FROM main.clients_bill WHERE reserved = 'NO' AND client_id = '%1'").arg(index +1));
    else if(ui->client_retention->isChecked())
        query.exec(QString("SELECT cart_id FROM main.clients_bill WHERE reserved = 'YES' AND client_id = '%1'").arg(index +1));
    while(query.next())
        ui->bill_num->addItem(query.value(0).toString());
}

void ReportsDialog::on_pushButton_3_clicked()
{
    ui->fromdate_2->setTime(QTime::fromString("12:00:00 AM","hh:mm:ss AP"));
    ui->todate_2->setTime(QTime::fromString("11:59:59 PM","hh:mm:ss AP"));
    ui->total_2->setText("0.000");
    if(ui->client_debt->isChecked())
        CartItems->setQuery(QString("SELECT * from main.clients_debt WHERE client_id = '%1'").arg(ui->client_name->currentIndex()+1));
    else
    {
        if(ui->bill_num->currentIndex() == 0)
        {
            if(ui->client_later_sell_type->isChecked())
                CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND client_id = '%3'").arg(ui->fromdate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->client_name->currentIndex()+1));
            else if(ui->client_retention->isChecked())
                CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND client_id = '%3'").arg(ui->fromdate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->client_name->currentIndex()+1));

        }
        else
        {
            if(ui->client_later_sell_type->isChecked())
                CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND cart_id = '%3' AND client_id = '%4'").arg(ui->fromdate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->bill_num->currentText().toUInt()).arg(ui->client_name->currentIndex()+1));
            else if(ui->client_retention->isChecked())
                CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE datetime(`when`) BETWEEN datetime('%1') AND datetime('%2') AND cart_id = '%3' AND client_id = '%4'").arg(ui->fromdate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->todate_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->bill_num->currentText().toUInt()).arg(ui->client_name->currentIndex()+1));
        }
    }
    for(int i=0;i<HeadsTableView->count();i++)
        CartItems->setHeaderData(i, Qt::Horizontal, HeadsTableView->at(i));
    ui->tableView_2->setModel(CartItems);
    for(int i=0;i<CartItems->rowCount();i++)
        ui->total_2->setText(QString::number(ui->total_2->text().toDouble() + CartItems->data(CartItems->index(i,7)).toDouble()));

}
