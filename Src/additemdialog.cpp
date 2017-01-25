#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "qint64validator.h"
#include "mymsg.h"
#include "typedef.h"
#include "item.h"
#include <QtSql>

additemdialog::additemdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::additemdialog)
{
    ui->setupUi(this);
    ui->barcode->setValidator(new QInt64Validator(this, 10, 9999999999999));
    QDoubleValidator* doubleValidator = new QDoubleValidator(1.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->price->setValidator(doubleValidator);
}

additemdialog::~additemdialog()
{
    delete ui;
}

void additemdialog::on_pushButton_clicked()
{
    LPITEM item = new Citem;
    if (ui->title->text() == "" || ui->desc->toPlainText() == "" || ui->price->text() == ""||ui->type->text()=="")
        AddItemEmptyMsg();
    else if ( item->IsExisted(ui->barcode->text().toUInt()) || item->IsExisted(ui->title->text()))
        ItemExistMsg();
    else
    {
        if(YesNoMsg(windowTitle()))
        {
            QSqlDatabase::database().transaction();
            QSqlQuery query(QString("INSERT INTO main.item_info (title, category, price, barcode, description)"
                            "VALUES ('%1', '%2', '%3', '%4', '%5')").arg(ui->title->text()).arg(ui->type->text())
                            .arg(ui->price->text()).arg(ui->barcode->text()).arg(ui->desc->toPlainText()));
            QSqlDatabase::database().commit();
            emit UpdateMainItemQuery();
            additemdialog::close();
            AddItemSuccessfullyMsg();
        }
    }
    delete item;
}
