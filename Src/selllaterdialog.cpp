#include "selllaterdialog.h"
#include "ui_selllaterdialog.h"
#include "mymsg.h"
#include <QDebug>
#include <QDoubleValidator>
#include <QSqlError>

SellLaterDialog::SellLaterDialog(QWidget *parent, size_t d_mode) :
    QDialog(parent),
    _mode(d_mode),
    ui(new Ui::SellLaterDialog)
{
    ui->setupUi(this);
    QDoubleValidator* doubleValidator = new QDoubleValidator(0.000, 9999999999999.000, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->cobon->setValidator(doubleValidator);
    ui->paid_debt->setValidator(doubleValidator);
    connect(ui->name, SIGNAL(returnPressed()), this, SLOT(GetClientInfo()));
    List = new QStringList;
    QSqlQuery client_query;
    client_query.exec("SELECT * FROM [main].[clients]");

    while (client_query.next())
        *List << client_query.value(1).toString();

    QCompleter *search = new QCompleter(*List,this);
    search->setCaseSensitivity(Qt::CaseInsensitive);
    ui->name->setCompleter(search);

    switch (GetMode()) {
    case Normal:
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        setGeometry(330,330,197,72);
        break;
    case Reservation:
        setWindowTitle("حجز صنف");
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        break;
    case AddDebt:
        setWindowTitle("ادارة الدائنين");
        ui->groupBox_3->setTitle("قيمة الدين المضافة");
        break;
    default:
        break;
    }
}

SellLaterDialog::~SellLaterDialog()
{
    delete ui;
    delete List;
}

void SellLaterDialog::GetClientInfo()
{
    if(List->contains(ui->name->text()))
    {
        ui->amount->clear();
        QSqlQuery client_query;
        client_query.exec(QString("SELECT id FROM [main].[clients] WHERE name = '%1'").arg(ui->name->text()));
        client_query.first();
        clientId = client_query.value(0).toUInt();
        if(GetMode() == Reservation)
            client_query.exec(QString("SELECT pinding FROM [main].[clients_bill] WHERE client_id = '%1' AND reserved = 'YES'").arg(clientId));
        else
            client_query.exec(QString("SELECT pinding FROM [main].[clients_bill] WHERE client_id = '%1' AND reserved = 'NO'").arg(clientId));
        while(client_query.next())
            ui->amount->setText(QString::number(ui->amount->text().toDouble() + client_query.value(0).toDouble()));
        client_query.exec(QString("SELECT cash,paid FROM [main].[clients_debt] WHERE client_id = '%1'").arg(clientId));
        while(client_query.next())
            ui->amount->setText(QString::number(ui->amount->text().toDouble() + client_query.value(0).toDouble() - client_query.value(1).toDouble()));
        if(ui->amount->text()=="")
            ui->amount->setText("0.000");
        ui->pushButton->setEnabled(true);
        ui->cobon->setFocus();
    }
    else
    {
        if(YesNoMsg(windowTitle()))
        {
            QSqlQuery query(QString("INSERT INTO main.clients (name)" "VALUES ('%1')").arg(ui->name->text()));
            ui->pushButton->setEnabled(true);
            *List << ui->name->text();
        }
    }
}

void SellLaterDialog::on_pushButton_clicked()
{
    if(YesNoMsg(windowTitle()))
    {
        QSqlDatabase::database().transaction();
        bool lazyCode = true;
        if(GetMode() == Reservation && ui->cobon->text().toDouble() <= 0)
            lazyCode = false;
        if(ui->name->text() != "" && lazyCode)
        {
            switch (GetMode()) {
            case Normal:
                emit PayLaterOK(ui->name->text());
                break;
            case Reservation:
                emit RetentionOK(ui->name->text(), ui->cobon->text().toDouble());
                break;
            case AddDebt:
            {
                if(ui->paid_debt->text().toDouble() > ui->amount->text().toDouble())
                    PaidGraterCash();
                else
                {
                    QSqlDatabase::database().transaction();
                    QSqlQuery query;
                    query.exec(QString("INSERT INTO main.clients_debt (client_id,cash) "
                                        "VALUES ('%1','%2')").arg(clientId).arg(ui->cobon->text().toDouble()));
                    AddExpensesSuccessfully();

                    QSqlDatabase::database().commit();
                }
            }
                break;
            default:
                break;
            }
            SellLaterDialog::close();
        }
        else
            ClientNameNotVaildMsg();
        QSqlDatabase::database().commit();
    }
}
