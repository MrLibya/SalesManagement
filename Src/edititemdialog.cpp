#include "edititemdialog.h"
#include "ui_edititemdialog.h"
#include "qint64validator.h"
#include "mymsg.h"
#include "typedef.h"
#include "item.h"
#include <QtSql>

edititemdialog::edititemdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edititemdialog)
{
    ui->setupUi(this);
    ui->barcode->setValidator(new QInt64Validator(this, 1, 9999999999999));
    QDoubleValidator* doubleValidator = new QDoubleValidator(1.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->price->setValidator(doubleValidator);

}

edititemdialog::~edititemdialog()
{
    delete ui;
}

void edititemdialog::on_pushButton_clicked()
{
    if (ui->title->text() == "" || ui->desc->toPlainText() == "" || ui->price->text() == ""||ui->type->text()=="")
        AddItemEmptyMsg();
    else
    {
        if(YesNoMsg(windowTitle()))
        {
            QSqlDatabase::database().transaction();
            QSqlQuery query(QString("UPDATE main.item_info SET title = '%1', category = '%2', price = '%3', description = '%4' WHERE barcode = '%5'")
                            .arg(ui->title->text()).arg(ui->type->text()).arg(ui->price->text()).arg(ui->desc->toPlainText()).arg(ui->barcode->text()));
            QSqlDatabase::database().commit();
            edititemdialog::close();
            EditItemSuccessfullyMsg();
        }
    }
}

void edititemdialog::on_tabWidget_tabBarClicked(int index)
{
    ui->Numberinput->clear();
    ui->search_item->clear();
    if (index = 0)
        ui->Numberinput->setFocus();
    else
        ui->search_item->setFocus();
}

void edititemdialog::on_pushButton_3_clicked()
{
    LPITEM item = new Citem;
    if (ui->tabWidget->currentIndex()== 0)
        if(!item->IsExisted(ui->Numberinput->text().toUInt()))
            ItemNotExistMsg();
        else
            CanEdit();

    else if (ui->tabWidget->currentIndex()== 1)
        if(!item->IsExisted(ui->search_item->text()))
            ItemNotExistMsg();
        else
            CanEdit();
    delete item;
}

void edititemdialog::CanEdit()
{
    ui->tabWidget->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->groupBox->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->barcode->setEnabled(false);
    ui->barcode->setText(ui->Numberinput->text());
    ui->title->setFocus();
}
