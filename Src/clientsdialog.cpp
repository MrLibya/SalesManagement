#include "clientsdialog.h"
#include "ui_clientsdialog.h"
#include <QDebug>
#include <QSqlError>
#include "mymsg.h"
#include <QValidator>
#include <QDateTime>

ClientsDialog::ClientsDialog(QWidget *parent, bool check) :
    QDialog(parent),
    reservation(check),
    ui(new Ui::ClientsDialog)
{
    ui->setupUi(this);
    QDoubleValidator* doubleValidator = new QDoubleValidator(1.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->paymenet_amount->setValidator(doubleValidator);
    CartItems = new QSqlQueryModel(this);
    QSqlQuery client_query;
    client_query.exec("SELECT * FROM [main].[clients]");
    ui->name->addItem("");
    while (client_query.next())
        ui->name->addItem(client_query.value(1).toString());
}

ClientsDialog::~ClientsDialog()
{
    delete ui;
}

void ClientsDialog::on_cart_id_currentIndexChanged(const QString &arg1)
{
    CartItems->clear();
    ui->Cart->setModel(CartItems);
    ui->bill_total->setText("0.000");
    ui->bill_inhold->setText("0.000");
    if(ui->cart_id->currentIndex() >=0)
    {
        double total =0;
        QStringList HeadsTableView;
        QSqlQuery query;
        HeadsTableView << "رقم الفاتورة" << "اسم المنتج" << "الباركود" << "السعر" << "الكميه" << " التخفيض" << "النوع" << "الاجمالي" << "رقم فاتورة المنتج" << "البائع" << "التاريخ" << "رقم العميل" << "تم اعاد ة المنتج" << "مدفوع";

        if(reservation)
            CartItems->setQuery(QString("SELECT * from main.retention_items_log WHERE cart_id = '%1' AND client_id = '%2' AND refund = 'NO' ").arg(arg1.toUInt()).arg(clientid));
        else
            CartItems->setQuery(QString("SELECT * from main.sell_later_log WHERE cart_id = '%1' AND client_id = '%2' AND refund = 'NO' ").arg(arg1.toUInt()).arg(clientid));

        for(int i=0;i<HeadsTableView.count();i++)
            CartItems->setHeaderData(i, Qt::Horizontal, HeadsTableView.at(i));
        ui->Cart->setModel(CartItems);
        ui->Cart->setColumnHidden(0,true);
        ui->Cart->setColumnHidden(8,true);
        ui->Cart->setColumnHidden(11,true);
        ui->Cart->setColumnHidden(12,true);
        ui->Cart->setColumnHidden(13,true);
        for(int i=0;i<CartItems->rowCount();i++)
            total += CartItems->data(CartItems->index(i,7)).toDouble();
        ui->bill_total->setText(QString::number(total));
        query.exec(QString("SELECT pinding FROM main.clients_bill WHERE cart_id ='%1'").arg(arg1.toUInt()));
        query.first();
        ui->bill_inhold->setText(query.value(0).toString());
    }
}

void ClientsDialog::on_pushButton_clicked()
{
    if(YesNoMsg(windowTitle()))
    {
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        if(ui->cashpayment->isChecked())
        {
            if(ui->bill_payment->isChecked())
            {
                query.exec(QString("UPDATE main.clients_bill SET paid = '%1', pinding = '0.0' WHERE cart_id = '%2'").arg(ui->bill_total->text().toDouble()).arg(ui->cart_id->currentText().toUInt()));
                query.exec(QString("INSERT INTO main.clients_bill_payment (cart_id,amount,'when')" "VALUES ('%1','%2','%3')").arg(ui->cart_id->currentText().toUInt()).arg(ui->bill_total->text().toDouble()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                ClientFinishPaymentMsg();
            }
            else if(ui->amount_payment->isChecked())
            {
                if(ui->paymenet_amount->text().toDouble() > ui->inhold_amount->text().toDouble())
                    ClientBillAboveTotalPriceMsg();
                else
                {
                    query.exec(QString("UPDATE main.clients_bill SET paid = paid + '%1', pinding = total-paid-'%1' WHERE cart_id = '%2'").arg(ui->paymenet_amount->text().toDouble()).arg(ui->cart_id->currentText().toUInt()));
                    query.exec(QString("INSERT INTO main.clients_bill_payment (cart_id,amount,'when')" "VALUES ('%1','%2','%3')").arg(ui->cart_id->currentText().toUInt()).arg(ui->paymenet_amount->text().toDouble()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
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
        ClientsDialog::close();
    }
}

void ClientsDialog::on_amount_payment_clicked()
{
    ui->paymenet_amount->setEnabled(true);
    ui->paymenet_amount->setFocus();
}

void ClientsDialog::on_bill_payment_clicked()
{
    ui->paymenet_amount->setEnabled(false);
}

void ClientsDialog::on_name_currentIndexChanged(const QString &arg1)
{
    ui->cart_id->clear();
    ui->pushButton->setEnabled(false);
    ui->total_amount->setText("0.000");
    ui->paid_amount->setText("0.000");
    ui->inhold_amount->setText("0.000");
    QSqlQuery client_query;
    client_query.exec(QString("SELECT id FROM [main].[clients] WHERE name = '%1'").arg(arg1));
    if(client_query.first())
    {
        clientid = client_query.value(0).toUInt();
        if(reservation)
            client_query.exec(QString("SELECT * FROM [main].[clients_bill] WHERE client_id = '%1' AND reserved = 'YES'").arg(clientid));
        else
            client_query.exec(QString("SELECT * FROM [main].[clients_bill] WHERE client_id = '%1' AND reserved = 'NO'").arg(clientid));
        while(client_query.next())
        {
            ui->total_amount->setText(QString::number(ui->total_amount->text().toDouble() + client_query.value(2).toDouble()));
            ui->paid_amount->setText(QString::number(ui->paid_amount->text().toDouble() + client_query.value(3).toDouble()));
            if(client_query.value(4).toDouble() > 0)
                ui->cart_id->addItem(client_query.value(0).toString());
        }
        ui->inhold_amount->setText(QString::number(ui->total_amount->text().toDouble() - ui->paid_amount->text().toDouble()));
        ui->cart_id->setEnabled(true);
        if(ui->cart_id->currentIndex() >=0)
            ui->pushButton->setEnabled(true);
    }
}
