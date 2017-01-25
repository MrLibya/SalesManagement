#include "billdialog.h"
#include "ui_billdialog.h"
#include "qint64validator.h"
#include "mymsg.h"
#include <QValidator>
#include <QDate>
#include <QtSql>
#include <QDebug>
#include <qsqlerror.h>
#include <QCompleter>

BillDialog::BillDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BillDialog)
{
    ui->setupUi(this);
    QDoubleValidator* doubleValidator = new QDoubleValidator(1.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->Numberinput->setValidator(new QInt64Validator(this, 1, 9999999999999));
    ui->added_amount->setValidator(new QIntValidator(1, 999999999, this));
    ui->bill_number->setValidator(new QInt64Validator(this, 1, 9999999999999));
    ui->sell_price->setValidator(doubleValidator);
    ui->buy_price->setValidator(doubleValidator);
    ui->wholesale_price->setValidator(doubleValidator);
    QObject::connect(ui->Numberinput, SIGNAL(returnPressed()), this, SLOT(GetItemInfo()));
    QObject::connect(ui->search_item, SIGNAL(returnPressed()), this, SLOT(GetItemInfo()));
    ui->Numberinput->setFocus();
    ui->groupBox_5->setEnabled(false);
    current_item = new Citem;
    QStringList List;
    QSqlQuery query;
    query.exec("SELECT * FROM [main].[suppliers]");
    while (query.next())
        List << query.value(1).toString();

    QCompleter *search = new QCompleter(List,this);
    search->setCaseSensitivity(Qt::CaseInsensitive);
    ui->supplier->setCompleter(search);
}

BillDialog::~BillDialog()
{
    delete ui;
    delete current_item;
}

void BillDialog::GetItemInfo()
{
    if (ui->tabWidget->currentIndex()== 0)
        if(!current_item->IsExisted(ui->Numberinput->text().toUInt()))
            ItemNotExistMsg();
        else
            UpdateItemInfo();
    else if (ui->tabWidget->currentIndex()== 1)
        if(!current_item->IsExisted(ui->search_item->text()))
            ItemNotExistMsg();
        else
            UpdateItemInfo();

}

void BillDialog::UpdateItemInfo()
{
    ui->tabWidget->setEnabled(false);
    ui->groupBox_5->setEnabled(true);
    ui->groupBox_6->setEnabled(true);
    ui->groupBox_7->setEnabled(true);
    ui->groupBox_8->setEnabled(true);
    ui->groupBox_9->setEnabled(true);
    ui->groupBox_10->setEnabled(true);
    ui->groupBox_11->setEnabled(true);
    ui->groupBox_12->setEnabled(true);
    ui->added_amount->setFocus();
    ui->pushButton->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->total_amount->setText(QString::number(current_item->GetTotalAmount()));
    ui->sold_amount->setText(QString::number(current_item->GetSoldAmount()));
    ui->buy_price->setText(QString::number(current_item->GetPrice()));
    ui->sell_price->setText(QString::number(current_item->GetPrice()));
    ui->wholesale_price->setText(QString::number(current_item->GetPrice()));
    ui->current_amount->setText(QString::number(current_item->GetCurrentAmount()));
}

void BillDialog::on_tabWidget_tabBarClicked(int index)
{
    ui->Numberinput->clear();
    ui->search_item->clear();
    if (index = 0)
        ui->Numberinput->setFocus();
    else
        ui->search_item->setFocus();
}

void BillDialog::on_pushButton_clicked()
{
    if(YesNoMsg(windowTitle()))
    {
        QDate date;
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        if (ui->added_amount->text() == "" || ui->sell_price->text() == "" || ui->buy_price->text() == "" || ui->wholesale_price->text()==""
                || ui->bill_number->text()=="" || ui->supplier->text()=="")
            AddItemEmptyMsg();
        else if (ui->dateEdit->date() <= date.currentDate())
            AddBillExpireMsg();
        else if (is_exist)
        {
            query.exec(QString("UPDATE main.items SET amount = '%1', buy_price = '%2', sell_price = '%3', wholesale_price = '%4', expire = '%5',bill_date = '%6' WHERE barcode = '%7' AND bill_number = '%8' AND supplier_id ='%9'")
                            .arg(ui->added_amount->text().toUInt()).arg(ui->buy_price->text().toDouble()).arg(ui->sell_price->text().toDouble()).arg(ui->wholesale_price->text().toDouble())
                            .arg(ui->dateEdit->date().toString("yyyy-MM-dd")).arg(ui->dateEdit_2->date().toString("yyyy-MM-dd")).arg(current_item->GetBarcode()).arg(ui->bill_number->text().toUInt()).arg(current_item->GetBillSupplierId()));
            double new_price = (ui->added_amount->text().toUInt() * ui->buy_price->text().toDouble()) - (current_item->GetBillBuyPrice() * current_item->GetBillAmount());
            query.exec(QString("UPDATE main.suppliers_bill set total = total + '%1', pinding = pinding + '%1' WHERE bill_id = '%2' AND supplier_id = '%3'").arg(new_price).arg(ui->bill_number->text().toUInt()).arg(current_item->GetBillSupplierId()));
            emit UpdateMainItemQuery();
            BillDialog::close();
            EditBillSuccessfullyMsg();
        }
        else
        {
            unsigned int suppId;
            if(query.exec(QString("SELECT id FROM main.suppliers WHERE name = '%1'").arg(ui->supplier->text())) && query.first())
                suppId = query.value(0).toUInt();
            else
            {
                query.exec(QString("INSERT INTO main.suppliers (name) " "VALUES ('%1')").arg(ui->supplier->text()));
                query.exec(QString("SELECT id FROM main.suppliers WHERE name = '%1'").arg(ui->supplier->text()));
                query.first();
                suppId = query.value(0).toUInt();
            }
            query.exec(QString("INSERT INTO main.items (barcode, amount, buy_price, sell_price, wholesale_price, expire, bill_date, bill_number, supplier_id)"
                            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')").arg(current_item->GetBarcode()).arg(ui->added_amount->text().toUInt())
                            .arg(ui->buy_price->text().toDouble()).arg(ui->sell_price->text().toDouble()).arg(ui->wholesale_price->text().toDouble())
                            .arg(ui->dateEdit->date().toString("yyyy-MM-dd")).arg(ui->dateEdit_2->date().toString("yyyy-MM-dd")).arg(ui->bill_number->text().toUInt()).arg(suppId));

            if(query.exec(QString("SELECT * FROM main.suppliers_bill WHERE bill_id = '%1'").arg(ui->bill_number->text().toUInt())) && query.first())
                query.exec(QString("UPDATE main.suppliers_bill set total = total + '%1', pinding = pinding + '%1' WHERE bill_id = '%2' AND supplier_id = '%3'").arg(ui->buy_price->text().toDouble() * ui->added_amount->text().toUInt()).arg(ui->bill_number->text().toUInt()).arg(suppId));
            else
                query.exec(QString("INSERT INTO main.suppliers_bill (bill_id,supplier_id,total,pinding ) "
                                   "VALUES ('%1','%2','%3','%3')").arg(ui->bill_number->text().toUInt()).arg(suppId).arg(ui->buy_price->text().toDouble() * ui->added_amount->text().toUInt()));
            BillDialog::close();
            AddBillSuccessfullyMsg();
        }
        QSqlDatabase::database().commit();
    }
}

bool BillDialog::AlreadyExist(qint64 barcode, unsigned int bill, unsigned int supplier, QString expire)
{
    QSqlQuery query(QString("SELECT * FROM [main].[items] WHERE barcode = '%1' AND bill_number = '%2' AND supplier_id = '%3' AND expire = '%4'").arg(barcode).arg(bill).arg(supplier).arg(expire));
    if (query.next())
    {
        current_item->SetBill(bill,supplier,expire);
        return true;
    }
    return false;
}

void BillDialog::on_pushButton_3_clicked()
{
    QSqlQuery query;
    if(query.exec(QString("SELECT id FROM main.suppliers WHERE name = '%1'").arg(ui->supplier->text())) && query.first())
    {
        if(AlreadyExist(current_item->GetBarcode(),ui->bill_number->text().toUInt(),query.value(0).toUInt(), ui->dateEdit->date().toString("yyyy-MM-dd")))
        {
            ui->buy_price->setText(QString::number(current_item->GetBillBuyPrice()));
            ui->sell_price->setText(QString::number(current_item->GetBillSellPrice()));
            ui->wholesale_price->setText(QString::number(current_item->GetBillWholeSalePrice()));
            ui->current_amount->setText(QString::number(current_item->GetBillAmount()));
            ui->supplier->setText(current_item->GetBillSupplierName());
            ui->dateEdit->setDate(current_item->GetBillExpire());
            ui->dateEdit_2->setDate(current_item->GetBillDate());
            ui->pushButton->setText("تحديث");
            is_exist = true;
        }
        else
            ReadBillFailedMsg();
    }
    else
        SupplierNotExist();
}
