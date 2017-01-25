#include "expensesdialog.h"
#include "ui_expensesdialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QDateTime>
#include "mymsg.h"
#include <QSqlError>
#include <QDebug>

ExpensesDialog::ExpensesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpensesDialog)
{
    ui->setupUi(this);
    QDoubleValidator* doubleValidator = new QDoubleValidator(1.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->price->setValidator(doubleValidator);

    model = new QSqlTableModel(this);
    model->setTable("expenses_log");
    QStringList HeadsTableView;
    HeadsTableView << "الرقم التعريفي" << "العنوان" << "القيمه" << "التاريخ";
    for(int i=0;i<HeadsTableView.count();i++)
        model->setHeaderData(i, Qt::Horizontal, HeadsTableView.at(i));
    model->select();
    ui->tableView->setModel(model);
}

ExpensesDialog::~ExpensesDialog()
{
    delete ui;
}

void ExpensesDialog::on_pushButton_clicked()
{
    if (ui->type->text() == "" || ui->price->text() == "")
        AddItemEmptyMsg();
    else
    {
        if(YesNoMsg(windowTitle()))
        {
            QSqlDatabase::database().transaction();
            QSqlQuery query(QString("INSERT INTO main.expenses_log (title, price, 'when')"
                            "VALUES ('%1', '%2', '%3')").arg(ui->type->text()).arg(ui->price->text().toDouble())
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            AddExpensesSuccessfully();
            ExpensesDialog::close();
            QSqlDatabase::database().commit();
        }
    }
}
