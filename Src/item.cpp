#include "item.h"
#include <QtSql>
#include <mainwindow.h>

Citem::Citem() : __total_amount(0) , __sold_amount(0) , __current_amount(0) , price(0)
{
    this->__item_query = std::unique_ptr<QSqlQuery>(new QSqlQuery);
    this->__bill_query = std::unique_ptr<QSqlQuery>(new QSqlQuery);
    __item_query->exec("SELECT * FROM [main].[item_info]");
}

Citem::~Citem()
{
}

bool Citem::IsExisted(qint64 barcode)
{
    __item_query->first();
    do
    {
        if (__item_query->value(4).toUInt() == barcode)
        {
            __bill_query->exec(QString("SELECT * FROM [main].[items] WHERE barcode = '%1'").arg(barcode));
            while(__bill_query->next())
            {
                __current_amount += __bill_query->value(2).toUInt();
                __sold_amount += __bill_query->value(3).toUInt();
            }
            __total_amount = __current_amount + __sold_amount;
            SetValues(__item_query->value(1).toString(),__item_query->value(2).toString(),__item_query->value(3).toDouble(),barcode,__item_query->value(5).toString());
            return true;
        }
    }while(__item_query->next());

    return false;
}

bool Citem::IsExisted(QString title)
{
    __item_query->first();
    do
    {
        if (__item_query->value(1).toString() == title)
        {
            __bill_query->exec(QString("SELECT * FROM [main].[items] WHERE barcode = '%1'").arg(__item_query->value(4).toUInt()));
            do
            {
                __current_amount += __bill_query->value(2).toUInt();
                __sold_amount += __bill_query->value(3).toUInt();
            }while(__bill_query->next());
            __total_amount = __current_amount + __sold_amount;
            SetValues(title,__item_query->value(2).toString(),__item_query->value(3).toDouble(),__item_query->value(4).toInt(),__item_query->value(5).toString());
            return true;
        }
    }while(__item_query->next());

    return false;
}

void Citem::SetValues(QString _title, QString _category, double _price, qint64 _barcode, QString _desc)
{
    title = _title;
    category = _category;
    price = _price;
    barcode = _barcode;
    desc = _desc;
}

void Citem::SetBill(unsigned int bill, unsigned int supplier, QString bill_expire)
{
    __bill_query->first();
    do
    {
		if(barcode == __bill_query->value(1).toUInt() && bill == __bill_query->value(9).toUInt() && supplier == __bill_query->value(10).toUInt() && bill_expire == __bill_query->value(7).toString())
        {
            amountbybill = __bill_query->value(2).toUInt();
            buy_price = __bill_query->value(4).toDouble();
            sell_price = __bill_query->value(5).toDouble();
            wholesale_price = __bill_query->value(6).toDouble();
            expire = QDate::fromString(__bill_query->value(7).toString(),"yyyy-MM-dd");
            bill_date = QDate::fromString(__bill_query->value(8).toString(),"yyyy-MM-dd");
            supplier_id = supplier;
            QSqlQuery query(QString("SELECT name FROM main.suppliers WHERE id = '%1'").arg(supplier_id));
            query.first();
            supplier_name = query.value(0).toString();
            bill_number = bill;
            break;
        }
    }while(__bill_query->next());
}

void Citem::UpdateQuery()
{
    __item_query->exec("SELECT * FROM [main].[item_info]");
    __bill_query->exec("SELECT * FROM [main].[items]");
}
