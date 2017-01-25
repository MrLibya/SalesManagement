#include "refunditemdialog.h"
#include "ui_refunditemdialog.h"
#include "mymsg.h"
#include <qDebug>
#include "QSqlError"

RefundItemDialog::RefundItemDialog(QWidget *parent, size_t d_mode) :
    QDialog(parent),
    _mode(d_mode),
    ui(new Ui::RefundItemDialog)
{
    ui->setupUi(this);
    query = new QSqlQuery;
    switch (GetMode()) {
    case Cash:
    {
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->pushButton->setEnabled(true);
        setGeometry(330,330,262,70);
        setWindowTitle("مردودات نقدا");
    }
        break;
    case Reservation:
    case PayLater:
    {
        ui->groupBox_3->hide();
        ui->name->addItem("");
        query->exec("SELECT * FROM [main].[clients]");
        while (query->next())
            ui->name->addItem(query->value(1).toString());
        setGeometry(330,330,160,70);
        if(GetMode() == PayLater)
            setWindowTitle("مردودات بالأجل");
        else
            setWindowTitle("الغاء حجز");
    }
        break;
    default:
        break;
    }


}

RefundItemDialog::~RefundItemDialog()
{
    delete ui;
    delete query;
}

void RefundItemDialog::on_pushButton_clicked()
{
    if(YesNoMsg(windowTitle()))
    {
        switch (GetMode()) {
        case Cash:
        {
            bool items_found=false;
            query->exec("SELECT cart_id from main.sell_cash_log");
            while(query->next())
                if(ui->lineEdit->text().toUInt() == query->value(0).toUInt())
                {
                    items_found = true;
                    break;
                }
            if(items_found)
            {
                emit CanRefundItems(ui->lineEdit->text().toUInt());
                RefundItemDialog::close();
            }
            else
                CartIdNotExistMsg();
        }
            break;
        case PayLater:
            emit CanRefundPayLaterItems(ui->comboBox->currentText().toUInt());
            RefundItemDialog::close();
            break;
        case Reservation:
            emit CanUnretention_Items(ui->comboBox->currentText().toUInt());
            RefundItemDialog::close();
            break;
        default:
            break;
        }
    }
}

void RefundItemDialog::on_name_currentIndexChanged(const QString &arg1)
{
    ui->comboBox->clear();
    ui->pushButton->setEnabled(false);
    query->first();
    if(arg1 != "")
    {
        do
        {
            if(query->value(1).toString() == arg1)
            {
                QSqlQuery query2;
                if (GetMode() == Reservation)
                    query2.exec(QString("SELECT cart_id FROM main.clients_bill WHERE client_id = '%1' AND reserved = 'YES'").arg(query->value(0).toUInt()));
                else
                    query2.exec(QString("SELECT cart_id FROM main.clients_bill WHERE client_id = '%1' AND reserved = 'NO'").arg(query->value(0).toUInt()));
                while(query2.next())
                    ui->comboBox->addItem(query2.value(0).toString());
                break;
            }

        }while(query->next());
        if(ui->comboBox->count() > 0)
            ui->pushButton->setEnabled(true);
        else
            ClientHasNoBillLeft();
    }
}
