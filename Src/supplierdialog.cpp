#include "supplierdialog.h"
#include "ui_supplierdialog.h"
#include "mymsg.h"
#include <QValidator>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>

SupplierDialog::SupplierDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupplierDialog)
{
    ui->setupUi(this);
    QDoubleValidator* doubleValidator = new QDoubleValidator(1.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->paymenet_amount->setValidator(doubleValidator);
    QObject::connect(ui->name, SIGNAL(returnPressed()), this, SLOT(GetClientInfo()));
    QStringList List;
    CartItems = new QSqlQueryModel(this);
    QSqlQuery query;
    query.exec("SELECT * FROM main.suppliers");
    while (query.next())
        List << query.value(1).toString();

    QCompleter *search = new QCompleter(List,this);
    search->setCaseSensitivity(Qt::CaseInsensitive);
    ui->name->setCompleter(search);
    HeadsTableView = new QStringList;
    *HeadsTableView << "رقم التعريف" << "الباركود" << "الكميه" << "الكميه المباعه" << "سعر الشراء" << "سعر البيع" << "سعر الجمله" << "تاريخ الصلاحيه" << "تاريخ الفاتوره" << "رقم الفاتورة" << "رقم المورد" << "مستهلكه" << "محجوزة";

}

SupplierDialog::~SupplierDialog()
{
    delete ui;
    delete HeadsTableView;
}

void SupplierDialog::GetClientInfo()
{
    QSqlQuery query;
    if(query.exec(QString("SELECT id FROM [main].[suppliers] WHERE name = '%1'").arg(ui->name->text())) && query.first())
    {
        supplierid = query.value(0).toUInt();
        query.exec(QString("SELECT * FROM [main].[suppliers_bill] WHERE supplier_id = '%1'").arg(supplierid));
        while(query.next())
        {
            ui->total_amount->setText(QString::number(ui->total_amount->text().toDouble() + query.value(2).toDouble()));
            ui->paid_amount->setText(QString::number(ui->paid_amount->text().toDouble() + query.value(3).toDouble()));
            if(query.value(4).toDouble() > 0)
                ui->cart_id->addItem(query.value(0).toString());
        }
        ui->inhold_amount->setText(QString::number(ui->total_amount->text().toDouble() - ui->paid_amount->text().toDouble()));
        ui->cart_id->setEnabled(true);
        if(ui->cart_id->currentIndex() >=0)
        {
            ui->pushButton->setEnabled(true);
            ui->name->setEnabled(false);
        }
    }
}

void SupplierDialog::on_cart_id_currentIndexChanged(const QString &arg1)
{

    QSqlQuery query;
    CartItems->setQuery(QString("SELECT * from main.items WHERE bill_number = '%1' AND supplier_id = '%2'")
                        .arg(arg1.toUInt()).arg(supplierid));
    for(int i=0;i<HeadsTableView->count();i++)
        CartItems->setHeaderData(i, Qt::Horizontal, HeadsTableView->at(i));
    ui->Cart->setModel(CartItems);
    ui->Cart->setColumnHidden(0,true);
    ui->Cart->setColumnHidden(10,true);
    query.exec(QString("SELECT total,pinding FROM main.suppliers_bill WHERE bill_id ='%1'").arg(arg1.toUInt()));
    query.first();
    ui->bill_total->setText(query.value(0).toString());
    ui->bill_inhold->setText(query.value(1).toString());
}

void SupplierDialog::on_pushButton_clicked()
{
    if(YesNoMsg(windowTitle()))
    {
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        if(ui->cashpayment->isChecked())
        {
            if(ui->bill_payment->isChecked())
            {
                query.exec(QString("UPDATE main.suppliers_bill SET paid = '%1', pinding = '0.0' WHERE cart_id = '%2'").arg(ui->bill_total->text().toDouble()).arg(ui->cart_id->currentText().toUInt()));
                query.exec(QString("INSERT INTO main.suppliers_bill_payment (bill_id,amount,'when')" "VALUES ('%1','%2','%3')").arg(ui->cart_id->currentText().toUInt()).arg(ui->bill_total->text().toDouble()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                ClientFinishPaymentMsg();
            }
            else if(ui->amount_payment->isChecked())
            {
                if(ui->paymenet_amount->text().toDouble() > ui->inhold_amount->text().toDouble())
                    ClientBillAboveTotalPriceMsg();
                else
                {
                    query.exec(QString("UPDATE main.suppliers_bill SET paid = paid + '%1', pinding = total-paid-'%1' WHERE bill_id = '%2'").arg(ui->paymenet_amount->text().toDouble()).arg(ui->cart_id->currentText().toUInt()));
                    query.exec(QString("INSERT INTO main.suppliers_bill_payment (bill_id,amount,'when')" "VALUES ('%1','%2','%3')").arg(ui->cart_id->currentText().toUInt()).arg(ui->paymenet_amount->text().toDouble()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                    ClientFinishPaymentMsg();
                }
            }
        }
        /* unimplemented
        else if(ui->bankcheck->isChecked())
        {

        }
        */
        QSqlDatabase::database().commit();
        SupplierDialog::close();
    }
}

void SupplierDialog::on_amount_payment_clicked()
{
    ui->paymenet_amount->setEnabled(true);
    ui->paymenet_amount->setFocus();
}

void SupplierDialog::on_bill_payment_clicked()
{
    ui->paymenet_amount->setEnabled(false);
}
