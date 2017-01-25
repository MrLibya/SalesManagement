#ifndef CITEM_H
#define CITEM_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <memory>

class Citem
{

public:
    Citem();
    virtual ~Citem();
    bool IsExisted(qint64);
    bool IsExisted(QString);
    void ExpireCheck();
    void SetValues(QString, QString, double, qint64, QString);
    void SetBill(unsigned int, unsigned int, QString);
    void UpdateQuery();

    // const returns
    const QString GetTitle() const {return title;}
    const QString GetCategory() const {return category;}
    const QString GetDesc() const {return desc;}
    const double GetPrice() const {return price;}
    const qint64 GetBarcode() const {return barcode;}
    const unsigned int GetTotalAmount() const {return __total_amount;}
    const unsigned int GetSoldAmount() const {return __sold_amount;}
    const unsigned int GetBillAmount() const {return amountbybill;}
    const double GetBillBuyPrice() const {return buy_price;}
    const double GetBillSellPrice() const {return sell_price;}
    const double GetBillWholeSalePrice() const {return wholesale_price;}
    const QDate GetBillExpire() const {return expire;}
    const QDate GetBillDate() const {return bill_date;}
    const unsigned int GetBillNumber() const {return bill_number;}
    const QString GetBillSupplierName() const {return supplier_name;}
    const unsigned int GetBillSupplierId() const {return supplier_id;}
    const unsigned int GetCurrentAmount() const {return __current_amount;}
    // end of const returns
private:
    unsigned int __total_amount;
    unsigned int __sold_amount;
    unsigned int __current_amount;
    std::unique_ptr<QSqlQuery> __item_query,__bill_query;

    QString title;
    QString category;
    double price;
    qint64 barcode;
    QString desc;
    unsigned int amountbybill;
    double buy_price;
    double sell_price;
    double wholesale_price;
    QDate expire;
    QDate bill_date;
    unsigned int bill_number;
    unsigned int supplier_id;
    QString supplier_name;
};



#endif // CITEM_H
