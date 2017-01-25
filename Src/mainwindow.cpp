#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include <QtSql>
#include <QMessageBox>
#include "users.h"
#include <QCompleter>
#include <QDateTime>
#include <item.h>
#include "mymsg.h"
#include "typedef.h"
#include "edititemdialog.h"
#include "qint64validator.h"
#include <QSqlDriver>
#include "itemsloaddialog.h"
#include <QPainter>

int ConfigDialog::_cartID = 0;
int ConfigDialog::_paperHigh = 0;
int ConfigDialog::_paperWidth = 0;
QString ConfigDialog::_shopName = "";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (users::level == BOSS)
    {
        ui->menu_2->setEnabled(true);
        ui->expenses_tab->setEnabled(true);
    }
    if(users::level >= CO)
    {
        ui->additem_tab->setEnabled(true);
        ui->edititem_tab->setEnabled(true);
        ui->add_Bill_tab->setEnabled(true);
        ui->menu_3->setEnabled(true);
        ui->menu_4->setEnabled(true);
        ui->clients_tab->setEnabled(true);
        ui->suppliers_tab->setEnabled(true);
        ui->consumer_items->setEnabled(true);
        ui->retention_items->setEnabled(true);
        ui->consumer_item_tab->setEnabled(true);
        ui->retention_items_tab->setEnabled(true);
        ui->unretention_items->setEnabled(true);
    }
    ui->Numberinput->setFocus();
    ui->Numberinput->setValidator(new QInt64Validator(this, 1, 9999999999999));
    ui->discount_amount->setValidator(new QDoubleValidator(0, 999, 3, this));
    connect(ui->Numberinput, SIGNAL(returnPressed()), this, SLOT(GetItemInfo()));
    connect(ui->search_item, SIGNAL(returnPressed()), this, SLOT(GetItemInfo()));
    SetSearchItem();
    /*incomplete
        ExpireCheck();
    */
    current_item = new Citem;
    //sometimes is go off
    ui->Cart->horizontalHeader()->show();
    ui->delete_item->setShortcut(QApplication::translate("MainWindow", "DELETE", 0));
    ui->AddCartButton->setShortcut(QApplication::translate("MainWindow", "ENTER",0));
    ui->pushButton->setShortcut(QApplication::translate("MainWindow", "ENTER",0));
    ui->Cart->hideColumn(8);
    ui->Cart->hideColumn(9);
    ui->Cart->hideColumn(10);

    QIcon tray_icon;
    tray_icon.addFile(QStringLiteral(":/icons/mainicon.ico"), QSize(), QIcon::Normal, QIcon::Off);
    main_tray = new QSystemTrayIcon(tray_icon,this);
    main_tray->activated(QSystemTrayIcon::Trigger);
    connect(main_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(TrayIconPressed()));
    connect(main_tray, SIGNAL(messageClicked()), this, SLOT(TrayIconPressed()));
    main_tray->setToolTip("هيما - إدارة المبيعات");

    QSqlQuery query("SELECT * FROM main.config");
    if(query.next())
    {
        if(query.value(1).toString() == "")
            on_config_tab_triggered();
        ConfigDialog::_cartID = query.value(0).toInt();
        ConfigDialog::_shopName = query.value(1).toString();
        ConfigDialog::_paperWidth = query.value(2).toInt();
        ConfigDialog::_paperHigh = query.value(3).toInt();
    }
    /*incomplete
    SetPrinter();
    */
}

MainWindow::~MainWindow()
{
    delete ui;
    delete current_item;
    delete printer;
}

void MainWindow::GetItemInfo()
{
    if (ui->tabWidget->currentIndex()== 0)
        if(!current_item->IsExisted(ui->Numberinput->text().toUInt()))
            ItemNotExistMsg();
        else
            UpdateItemInfo();
    else if (ui->tabWidget->currentIndex()== 1)
        if (ui->search_item->text()=="")
            ui->pushButton->setFocus();
        else if(!current_item->IsExisted(ui->search_item->text()))
            ItemNotExistMsg();
        else
            UpdateItemInfo();

}

void MainWindow::UpdateItemInfo()
{
    int numRows = 0;
    QSqlQuery query(QString("SELECT amount,expire,bill_number,supplier_id FROM [main].[items] WHERE barcode = '%1'").arg(current_item->GetBarcode()));
    while(query.next())
        //if (query.value(0).toUInt()>0) to show even if there is amount , but we need to the refund so show even no amount
            numRows += 1;
    query.first();
    if(numRows >1)
    {
        d_itemsloaddialog = new ItemsLoadDialog(this);

        do{
           // if(query.value(0).toUInt() > 0) to show even if there is amount , but we need to the refund so show even no amount
                d_itemsloaddialog->SetItems(query.value(1).toString(), query.value(2).toString(), query.value(3).toString());
        }while(query.next());
        connect(d_itemsloaddialog,SIGNAL(SendBill(QString,QString,QString)),this,SLOT(GotBill(QString,QString,QString)));
        d_itemsloaddialog->setModal(true);
        d_itemsloaddialog->exec();
        ui->price_sell_type->setChecked(true);
        ui->cash_sell_type->setChecked(true);
    }
    else if (numRows == 1)
    {
        do{
            if(query.value(0).toUInt() > 0)
            {
                current_item->SetBill(query.value(2).toUInt(),query.value(3).toUInt(),query.value(1).toString());
                break;
            }
        }while(query.next());

        ui->c_amount->setEnabled(true);
        ui->AddCartButton->setEnabled(true);
        ui->groupBox_2->setEnabled(true);
        ui->c_amount->setFocus();
        ui->discount_amount->setText("0.000");
        ui->price->setText(QString::number(current_item->GetBillSellPrice()));
        ui->price_sell_type->setChecked(true);
        ui->cash_sell_type->setChecked(true);
    }
    else
        NotEnoughAmountMsg();


}

void MainWindow::on_LogoutButton_triggered()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.isOpen())
    {
        mainWinowClose = true;
        LogOutMsg();
        db.close();
        MainWindow::close();
    }
}

void MainWindow::SetSearchItem()
{
    QStringList List;
    QSqlQuery query("SELECT title FROM [main].[item_info]");
    while (query.next())
        List << query.value(0).toString();

    search = new QCompleter(List,this);
    search->setCaseSensitivity(Qt::CaseInsensitive);
    ui->search_item->setCompleter(search);
}

void MainWindow::on_AddCartButton_clicked()
{
    if(ui->c_amount->text().toUInt() > current_item->GetBillAmount() || (CartItemAmount() + ui->c_amount->text().toUInt() ) > current_item->GetBillAmount())
        AmountOverFlowMsg();
    else
    {
        double final_price = ui->c_amount->text().toDouble() * ui->price->text().toDouble();
        if(ui->discount_amount->text().toDouble() > final_price)
        {
            CantDiscountMsg();
            ui->discount_amount->setText("0.000");
        }
        else
            final_price -= ui->discount_amount->text().toDouble();
        int InCartRow = ItemInCart();
        if(InCartRow >= 0)
        {
            ui->Cart->setItem(InCartRow, 4, new QTableWidgetItem(QString::number(ui->c_amount->text().toUInt() + ui->Cart->item(InCartRow,4)->text().toUInt())));
            ui->Cart->setItem(InCartRow, 5, new QTableWidgetItem(QString::number(ui->discount_amount->text().toDouble() + ui->Cart->item(InCartRow,5)->text().toDouble())));
            ui->Cart->setItem(InCartRow, 7, new QTableWidgetItem(QString::number(final_price + ui->Cart->item(InCartRow,7)->text().toDouble())));
        }
        else
        {
            ui->Cart->setRowCount(ui->Cart->rowCount()+1);
            ui->Cart->setItem(ui->Cart->rowCount()-1, 0, new QTableWidgetItem(current_item->GetTitle()));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 1, new QTableWidgetItem(current_item->GetCategory()));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 2, new QTableWidgetItem(current_item->GetBillExpire().toString("yyyy-MM-dd")));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 3, new QTableWidgetItem(ui->price->text()));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 4, new QTableWidgetItem(ui->c_amount->text()));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 5, new QTableWidgetItem(ui->discount_amount->text()));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 6, new QTableWidgetItem(SellType()));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 7, new QTableWidgetItem(QString::number(final_price)));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 8, new QTableWidgetItem(QString::number(current_item->GetBillNumber())));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 9, new QTableWidgetItem(QString::number(current_item->GetBillSupplierId())));
            ui->Cart->setItem(ui->Cart->rowCount()-1, 10, new QTableWidgetItem(QString::number(current_item->GetBarcode())));
            ui->Cart_total->setText(QString::number(final_price + ui->Cart_total->text().toDouble()));
        }
        ui->Numberinput->setFocus();
        ui->Numberinput->clear();
        ui->c_amount->setEnabled(false);
        ui->AddCartButton->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->discount_amount->setText("0.000");
        ui->c_amount->setValue(1);
        ui->price->clear();
        ui->pushButton->setEnabled(true);
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    ui->Numberinput->clear();
    ui->search_item->clear();
    if (index = 0)
        ui->Numberinput->setFocus();
    else
        ui->search_item->setFocus();
}

void MainWindow::on_delete_item_clicked()
{
    ui->Cart_total->setText(QString::number(ui->Cart_total->text().toDouble() - ui->Cart->item(ui->Cart->currentRow(),7)->text().toDouble()));
    ui->Cart->removeRow(ui->Cart->currentRow());
    ui->delete_item->setEnabled(false);
    if(ui->Cart->rowCount() <=0)
        ui->pushButton->setEnabled(false);
}

void MainWindow::on_Cart_itemSelectionChanged()
{
    ui->delete_item->setEnabled(true);
}

/* incomplete
void MainWindow::ExpireCheck()
{
    QSqlQuery query(QString("SELECT barcode FROM [main].[items] WHERE amount > 0 AND strftime('%Y-%m-%d', expire) - strftime('%Y-%m-%d', '%1') > strftime('%Y-%m-%d', '0000-01-00')").arg(QDate::currentDate().toString("yyyy-MM-dd")));
    while (query.next())
    {
        qDebug()<<"ok";
    }
    qDebug()<<query.lastError().text();
    qDebug()<<query.lastQuery();
}
*/
void MainWindow::on_additem_tab_triggered()
{
    d_additemdialog = new additemdialog(this);
    connect(d_additemdialog,SIGNAL(UpdateMainItemQuery()),this,SLOT(GotUpdateMainItemQuery()));
    d_additemdialog->setModal(true);
    d_additemdialog->exec();

}

void MainWindow::on_edititem_tab_triggered()
{
    d_edititemdialog = new edititemdialog(this);
    d_edititemdialog->setModal(true);
    d_edititemdialog->exec();
}

void MainWindow::on_employee_tab_triggered()
{
    d_users = new users(this);
    d_users->setModal(true);
    d_users->exec();
}

void MainWindow::on_add_Bill_tab_triggered()
{
    d_billdialog = new BillDialog(this);
    connect(d_billdialog,SIGNAL(UpdateMainItemQuery()),this,SLOT(GotUpdateMainItemQuery()));
    d_billdialog->setModal(true);
    d_billdialog->exec();
}

void MainWindow::on_clients_tab_triggered()
{
    d_clientsdialog = new ClientsDialog(this);
    d_clientsdialog->setModal(true);
    d_clientsdialog->exec();
}

void MainWindow::on_suppliers_tab_triggered()
{
    d_supplierdialog = new SupplierDialog(this);
    d_supplierdialog->setModal(true);
    d_supplierdialog->exec();
}

void MainWindow::on_expenses_tab_triggered()
{
    d_expensesdialog = new ExpensesDialog(this);
    d_expensesdialog->setModal(true);
    d_expensesdialog->exec();
}

void MainWindow::on_consumer_item_tab_triggered()
{
    d_consumeritemdialog = new ConsumerItemDialog(this);
    d_consumeritemdialog->setModal(true);
    d_consumeritemdialog->exec();
}

void MainWindow::on_retention_items_tab_triggered()
{
    d_clientsdialog = new ClientsDialog(this,true);
    d_clientsdialog->setModal(true);
    d_clientsdialog->exec();
}

void MainWindow::GotBill(QString bill,QString supplier,QString expire)
{
    current_item->SetBill(bill.toUInt(),supplier.toUInt(),expire);
    ui->c_amount->setEnabled(true);
    ui->AddCartButton->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
    ui->c_amount->setFocus();
    ui->discount_amount->setText("0.000");
    ui->price->setText(QString::number(current_item->GetBillSellPrice()));
}

void MainWindow::on_price_wholesale_type_clicked()
{
    ui->price->setText(QString::number(current_item->GetBillWholeSalePrice()));
}

void MainWindow::on_price_buy_type_clicked()
{
    ui->price->setText(QString::number(current_item->GetBillBuyPrice()));
}

void MainWindow::on_price_sell_type_clicked()
{
    ui->price->setText(QString::number(current_item->GetBillSellPrice()));
}

unsigned int MainWindow::CartItemAmount()
{
    unsigned int local_amount=0;
    for(int i=0;i<ui->Cart->rowCount();i++)
    {
        if(ui->Cart->item(i,0)->text() == current_item->GetTitle() && ui->Cart->item(i,2)->text() == current_item->GetBillExpire().toString("yyyy-MM-dd")
                && ui->Cart->item(i,8)->text().toUInt() == current_item->GetBillNumber() && ui->Cart->item(i,9)->text().toUInt() == current_item->GetBillSupplierId())
            local_amount += ui->Cart->item(i,4)->text().toUInt();
    }
    return local_amount;
}

int MainWindow::ItemInCart()
{
    for(int i=0;i<ui->Cart->rowCount();i++)
    {
        if(ui->Cart->item(i,0)->text() == current_item->GetTitle() && ui->Cart->item(i,2)->text() == current_item->GetBillExpire().toString("yyyy-MM-dd")
                && ui->Cart->item(i,8)->text().toUInt() == current_item->GetBillNumber() && ui->Cart->item(i,9)->text().toUInt() == current_item->GetBillSupplierId()
                && SellType() == ui->Cart->item(i,6)->text())
            return i;
    }
    return -1;
}

QString MainWindow::SellType()
{
    if(ui->price_sell_type->isChecked())
        return "سعر البيع";
    else if(ui->price_wholesale_type->isChecked())
        return "سعر الجمله";
    else if(ui->price_buy_type->isChecked())
        return "سعر الشراء";
    return "خطا";
}

void MainWindow::on_pushButton_clicked()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    if(ui->cash_sell_type->isChecked())
    {
        if(YesNoMsg(windowTitle()))
        {
            for(int i=0;i<ui->Cart->rowCount();i++)
            {
                query.exec(QString("INSERT INTO [main].[sell_cash_log] (cart_id, title, barcode, price, amount, disscount, type, total, bill_number, whos, 'when', supplier_id,expire) "
                            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13')").arg(ConfigDialog::_cartID).arg(ui->Cart->item(i,0)->text())
                            .arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,3)->text().toDouble()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,5)->text().toDouble()).arg(ui->Cart->item(i,6)->text())
                            .arg(ui->Cart->item(i,7)->text().toDouble()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(users::name).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
                query.exec(QString("UPDATE [main].[items] SET amount = amount - '%1',sold_amount = sold_amount + '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%5' AND expire = '%6'").arg(ui->Cart->item(i,4)->text().toUInt())
                            .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
             }
            query.exec(QString("UPDATE [main].[users] SET money = money + '%1' WHERE id = '%2'").arg(ui->Cart_total->text().toDouble()).arg(users::id));
            query.exec(QString("UPDATE main.config set cart_id = '%1'").arg(ConfigDialog::_cartID+1));
            /* incomplete
            PrintCartBill();
            */
            ConfigDialog::_cartID = ConfigDialog::_cartID + 1;
            FinishPayment();

        }
    }
    else if (ui->later_sell_type->isChecked())
    {
        d_selllaterdialog = new SellLaterDialog(this,SellLaterDialog::Normal);
        connect(d_selllaterdialog,SIGNAL(PayLaterOK(QString)),this,SLOT(GotPayLaterOK(QString)));
        d_selllaterdialog->setModal(true);
        d_selllaterdialog->exec();

    }
    else if(ui->refund_cash_sell_type->isChecked())
    {
        d_refunditemdialog = new RefundItemDialog(this,RefundItemDialog::Cash);
        connect(d_refunditemdialog,SIGNAL(CanRefundItems(unsigned int)),this,SLOT(GotCanRefundItems(unsigned int)));
        d_refunditemdialog->setModal(true);
        d_refunditemdialog->exec();
    }
    else if (ui->refund_later_sell_type->isChecked())
    {
        d_refunditemdialog = new RefundItemDialog(this,RefundItemDialog::PayLater);
        connect(d_refunditemdialog,SIGNAL(CanRefundPayLaterItems(unsigned int)),this,SLOT(GotCanRefundPayLaterItems(unsigned int)));
        d_refunditemdialog->setModal(true);
        d_refunditemdialog->exec();
    }
    else if (ui->consumer_items->isChecked())
    {
        for(int i=0;i<ui->Cart->rowCount();i++)
        {

            query.exec(QString("INSERT INTO [main].[consumer_item_log] (title, barcode, bill_number, whos, 'when', supplier_id, expire,amount,cobon) "
                        "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')").arg(ui->Cart->item(i,0)->text())
                       .arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(users::name).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()).arg(ui->Cart->item(i,4)->text().toUInt()));
            query.exec(QString("UPDATE [main].[items] SET amount = amount - '%1',consumer_amount = consumer_amount + '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%5' AND expire = '%6'").arg(ui->Cart->item(i,4)->text().toUInt())
                        .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
        }
        FinishPayment();
    }
    else if(ui->retention_items->isChecked())
    {
        d_selllaterdialog = new SellLaterDialog(this,SellLaterDialog::Reservation);
        connect(d_selllaterdialog,SIGNAL(RetentionOK(QString,double)),this,SLOT(GotRetentionOK(QString,double)));
        d_selllaterdialog->setModal(true);
        d_selllaterdialog->exec();
    }
    else if (ui->unretention_items->isChecked())
    {
        d_refunditemdialog = new RefundItemDialog(this,RefundItemDialog::Reservation);
        connect(d_refunditemdialog,SIGNAL(CanUnretention_Items(unsigned int)),this,SLOT(GotCanUnretention_Items(unsigned int)));
        d_refunditemdialog->setModal(true);
        d_refunditemdialog->exec();
    }

    QSqlDatabase::database().commit();

}
void MainWindow::FinishPayment()
{
    ui->Cart->setRowCount(0);
    ui->Cart_total->clear();
    ui->delete_item->setEnabled(false);
    ui->pushButton->setEnabled(false);
}

void MainWindow::GotPayLaterOK(QString client_name)
{
	QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.exec(QString("SELECT id from main.clients WHERE name = '%1'").arg(client_name));
    query.first();
    unsigned int client_id = query.value(0).toUInt();
    for(int i=0;i<ui->Cart->rowCount();i++)
    {
        query.exec(QString("INSERT INTO [main].[sell_later_log] (cart_id, title, barcode, price, amount, disscount, type, total, bill_number, whos, 'when', client_id, supplier_id, expire) "
                        "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14')").arg(ConfigDialog::_cartID).arg(ui->Cart->item(i,0)->text())
                        .arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,3)->text().toDouble()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,5)->text().toDouble()).arg(ui->Cart->item(i,6)->text())
                        .arg(ui->Cart->item(i,7)->text().toDouble()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(users::name).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(client_id).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));

        query.exec(QString("UPDATE [main].[items] SET amount = amount - '%1',sold_amount = sold_amount + '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%6' AND expire = '%7'").arg(ui->Cart->item(i,4)->text().toUInt())
                   .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));

    }
    query.exec(QString("INSERT INTO main.clients_bill (cart_id, client_id, total,pinding)"
                       "VALUES ('%1', '%2', '%3', '%4')").arg(ConfigDialog::_cartID).arg(client_id).arg(ui->Cart_total->text().toDouble()).arg(ui->Cart_total->text().toDouble()));
    query.exec(QString("UPDATE main.config set cart_id = '%1'").arg(ConfigDialog::_cartID+1));
    ConfigDialog::_cartID = ConfigDialog::_cartID + 1;
    QSqlDatabase::database().commit();
    FinishPayment();
}

void MainWindow::GotCanRefundItems(unsigned int cart_id)
{
	QSqlDatabase::database().transaction();
    QSqlQuery query;
    for(int i=ui->Cart->rowCount()-1;i>=0;i--)
    {
        query.exec(QString("SELECT * from main.sell_cash_log WHERE cart_id = '%1' AND barcode = '%2' AND amount = '%3' AND bill_number = '%4' AND type = '%5' AND disscount = '%6' AND supplier_id = '%7' AND expire = '%8'")
                              .arg(cart_id).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,6)->text()).arg(ui->Cart->item(i,5)->text().toDouble()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
        if(query.first() && query.value(11).toString() == "NO")
        {
            query.exec(QString("UPDATE main.sell_cash_log SET refund = '%1' WHERE cart_id = '%2' AND barcode = '%3' AND amount = '%4' AND bill_number = '%5' AND type = '%6' AND supplier_id = '%7' AND expire = '%8'")
                       .arg("YES").arg(cart_id).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,6)->text()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE [main].[items] SET amount = amount + '%1',sold_amount = sold_amount - '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%5' AND expire = '%6'").arg(ui->Cart->item(i,4)->text().toUInt())
                       .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE [main].[users] SET money = money - '%1' WHERE id = '%2'").arg(ui->Cart->item(i,7)->text().toDouble()).arg(users::id));
            ui->Cart_total->setText(QString::number(ui->Cart_total->text().toDouble() - ui->Cart->item(i,7)->text().toDouble()));
            ui->Cart->removeRow(i);
            if(ui->Cart_total->text().toDouble() < 0)
                ui->Cart_total->setText("0.000");
        }
        else
            ItemNotExistOnCartIDMsg(ui->Cart->item(i,0)->text());
    }
    QSqlDatabase::database().commit();
}

void MainWindow::GotCanRefundPayLaterItems(unsigned int cart_id)
{
	QSqlDatabase::database().transaction();
    QSqlQuery query;
    double newprice = 0;
    for(int i=ui->Cart->rowCount()-1;i>=0;i--)
    {
        query.exec(QString("SELECT * from main.sell_later_log WHERE cart_id = '%1' AND barcode = '%2' AND amount = '%3' AND bill_number = '%4' AND type = '%5' AND disscount = '%6' AND supplier_id = '%7' AND expire = '%8'")
                              .arg(cart_id).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,6)->text()).arg(ui->Cart->item(i,5)->text().toDouble()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
        if(query.first() && query.value(12).toString() == "NO")
        {
            query.exec(QString("UPDATE main.sell_later_log SET refund = 'YES' WHERE cart_id = '%1' AND barcode = '%2' AND amount = '%3' AND bill_number = '%4' AND type = '%5' AND supplier_id = '%6' AND expire = '%7'")
                       .arg(cart_id).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,6)->text()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE [main].[items] SET amount = amount + '%1',sold_amount = sold_amount - '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%5' AND expire = '%6'").arg(ui->Cart->item(i,4)->text().toUInt())
                       .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE main.clients_bill SET total = total - '%1', refund = refund + '%1' WHERE cart_id ='%2'").arg(ui->Cart->item(i,7)->text().toDouble()).arg(cart_id));
            query.exec(QString("SELECT total,paid,pinding FROM main.clients_bill WHERE cart_id = '%1'").arg(cart_id));
            query.first();
            if(query.value(2).toDouble() >= ui->Cart->item(i,7)->text().toDouble())
                query.exec(QString("UPDATE main.clients_bill SET pinding = pinding - '%1' WHERE cart_id ='%2'").arg(ui->Cart->item(i,7)->text().toDouble()).arg(cart_id));
            else
            {
                newprice = ui->Cart->item(i,7)->text().toDouble() - query.value(2).toDouble();
                query.exec(QString("UPDATE main.clients_bill SET pinding = '0.0' WHERE cart_id ='%1'").arg(cart_id));
                query.exec(QString("UPDATE main.clients_bill SET paid = paid - '%1' WHERE cart_id ='%2'").arg(newprice).arg(cart_id));
            }

            ui->Cart_total->setText(QString::number(ui->Cart_total->text().toDouble() - ui->Cart->item(i,7)->text().toDouble()));
            ui->Cart->removeRow(i);
            if(ui->Cart_total->text().toDouble() < 0)
                ui->Cart_total->setText("0.000");
        }
        else
            ItemNotExistOnCartIDMsg(ui->Cart->item(i,0)->text());
    }
    QSqlDatabase::database().commit();
}

void MainWindow::GotCanUnretention_Items(unsigned int cart_id)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    double newprice = 0;
    for(int i=ui->Cart->rowCount()-1;i>=0;i--)
    {
        query.exec(QString("SELECT * from main.retention_items_log WHERE cart_id = '%1' AND barcode = '%2' AND amount = '%3' AND bill_number = '%4' AND type = '%5' AND disscount = '%6' AND supplier_id = '%7' AND expire = '%8'")
                              .arg(cart_id).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,6)->text()).arg(ui->Cart->item(i,5)->text().toDouble()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
        if(query.first() && query.value(12).toString() == "NO")
        {
            query.exec(QString("UPDATE main.retention_items_log SET refund = 'YES' WHERE cart_id = '%1' AND barcode = '%2' AND amount = '%3' AND bill_number = '%4' AND type = '%5' AND supplier_id = '%6' AND expire = '%7'")
                       .arg(cart_id).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,6)->text()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE [main].[items] SET amount = amount + '%1',retention_amount = retention_amount - '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%5' AND expire = '%6'").arg(ui->Cart->item(i,4)->text().toUInt())
                       .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE main.clients_bill SET total = total - '%1', refund = refund + '%1' WHERE cart_id ='%2'").arg(ui->Cart->item(i,7)->text().toDouble()).arg(cart_id));
            query.exec(QString("SELECT total,paid,pinding FROM main.clients_bill WHERE cart_id = '%1'").arg(cart_id));
            query.first();
            if(query.value(2).toDouble() >= ui->Cart->item(i,7)->text().toDouble())
                query.exec(QString("UPDATE main.clients_bill SET pinding = pinding - '%1' WHERE cart_id ='%2'").arg(ui->Cart->item(i,7)->text().toDouble()).arg(cart_id));
            else
            {
                newprice = ui->Cart->item(i,7)->text().toDouble() - query.value(2).toDouble();
                query.exec(QString("UPDATE main.clients_bill SET pinding = '0.0' WHERE cart_id ='%1'").arg(cart_id));
                query.exec(QString("UPDATE main.clients_bill SET paid = paid - '%1' WHERE cart_id ='%2'").arg(newprice).arg(cart_id));
            }

            ui->Cart_total->setText(QString::number(ui->Cart_total->text().toDouble() - ui->Cart->item(i,7)->text().toDouble()));
            ui->Cart->removeRow(i);
            if(ui->Cart_total->text().toDouble() < 0)
                ui->Cart_total->setText("0.000");
        }
        else
            ItemNotExistOnCartIDMsg(ui->Cart->item(i,0)->text());
    }
    QSqlDatabase::database().commit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (mainWinowClose)
        event->accept();
    else
    {
        MainWindow::hide();
        main_tray->show();
        main_tray->showMessage("هيما - إدارة المبيعات","اذا اردت اغلاق البرنامج بشكل كامل استعمل تسجيل الخروج , اضغط هنا لفتح البرنامج",QSystemTrayIcon::Information);
        event->ignore();
    }
}

void MainWindow::TrayIconPressed()
{
    main_tray->hide();
    MainWindow::show();
}

void MainWindow::GotRetentionOK(QString client_name,double cobon)
{
    if(cobon >= ui->Cart_total->text().toDouble())
        CobonGraterTotal();
    else
    {
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        query.exec(QString("SELECT id from main.clients WHERE name = '%1'").arg(client_name));
        query.first();
        unsigned int client_id = query.value(0).toUInt();
        for(int i=0;i<ui->Cart->rowCount();i++)
        {
            query.exec(QString("INSERT INTO [main].[retention_items_log] (cart_id, title, barcode, price, amount, disscount, type, total, bill_number, whos, 'when', client_id, supplier_id, expire) "
                            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14')").arg(ConfigDialog::_cartID).arg(ui->Cart->item(i,0)->text())
                            .arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,3)->text().toDouble()).arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,5)->text().toDouble()).arg(ui->Cart->item(i,6)->text())
                            .arg(ui->Cart->item(i,7)->text().toDouble()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(users::name).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(client_id).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));
            query.exec(QString("UPDATE [main].[items] SET amount = amount - '%1',retention_amount = retention_amount + '%2'  WHERE barcode = '%3' AND bill_number = '%4' AND supplier_id = '%6' AND expire = '%7'").arg(ui->Cart->item(i,4)->text().toUInt())
                       .arg(ui->Cart->item(i,4)->text().toUInt()).arg(ui->Cart->item(i,10)->text().toUInt()).arg(ui->Cart->item(i,8)->text().toUInt()).arg(ui->Cart->item(i,9)->text().toUInt()).arg(ui->Cart->item(i,2)->text()));

        }
        query.exec(QString("INSERT INTO main.clients_bill (cart_id, client_id, total,paid, pinding, reserved)"
                           "VALUES ('%1', '%2', '%3', '%4', '%5','YES')").arg(ConfigDialog::_cartID).arg(client_id).arg(ui->Cart_total->text().toDouble()).arg(cobon).arg(ui->Cart_total->text().toDouble() - cobon));
        query.exec(QString("UPDATE main.config set cart_id = '%1'").arg(ConfigDialog::_cartID+1));
        ConfigDialog::_cartID = ConfigDialog::_cartID + 1;
        QSqlDatabase::database().commit();
        FinishPayment();
    }
}

void MainWindow::on_debt_manger_triggered()
{
    d_selllaterdialog = new SellLaterDialog(this,SellLaterDialog::AddDebt);
    d_selllaterdialog->setModal(true);
    d_selllaterdialog->exec();
}

void MainWindow::on_reports_tab_triggered()
{
    d_reportsdialog = new ReportsDialog(this);
    d_reportsdialog->setModal(true);
    d_reportsdialog->exec();
}
// incomplete
/*
void MainWindow::SetPrinter()
{
    printer = new QPrinter(QPrinter::HighResolution);
    if(ConfigDialog::_paperHigh && ConfigDialog::_paperWidth)
        printer->setPaperSize(QSizeF(ConfigDialog::_paperWidth, ConfigDialog::_paperHigh), QPrinter::Millimeter);
    else
        printer->setPaperSize(QPrinter::A4);

    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins (15,15,15,15,QPrinter::Millimeter);
    printer->setFullPage(false);
    //printer->setOutputFileName("output.pdf");
    printer->setOutputFormat(QPrinter::NativeFormat); //you can use native format of system usin QPrinter::NativeFormat
    p_dialog = new QPrintDialog(printer,this);
    p_dialog->setWindowTitle("Print Document");

}

void MainWindow::PrintCartBill()
{
    if (p_dialog->exec() != QDialog::Accepted)
        return ;
    int height = printer->height();
    int width = printer->width();
    QPainter painter(printer); // create a painter which will paint 'on printer'.
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Tahoma",16));
    painter.drawText(QRect(0, 0, width, height), Qt::AlignTop | Qt::AlignHCenter, ConfigDialog::_shopName);
    painter.setFont(QFont("Tahoma",12));
    painter.drawText(QRect(140, 310, width, height), Qt::AlignTop | Qt::AlignHCenter, "الموظف: " + users::name);
    painter.drawText(QRect(70, 560, width, height), Qt::AlignTop | Qt::AlignHCenter, "رقم الفاتورة: " + QString::number(ConfigDialog::_cartID));
    painter.setFont(QFont("Tahoma",8));
    painter.drawText(QRect(150, 840, width, height), Qt::AlignTop | Qt::AlignHCenter, "الاسم\t\t\t\t\t\t\t\tالسعر");
    for(int i=0;i<ui->Cart->rowCount();i++)
    {
        painter.drawText(QRect(150, 1030 + (i*200), width, height), Qt::AlignTop | Qt::AlignHCenter, QString("'%1'\t\t\t\t\t\t\t\t'%2'")
                         .arg(ui->Cart->item(i,0)->text()) .arg(ui->Cart->item(i,7)->text()));
    }
    painter.end();

}
*/

void MainWindow::on_config_tab_triggered()
{
    d_configdialog = new ConfigDialog(this);
    d_configdialog->setModal(true);
    d_configdialog->exec();
}

void MainWindow::on_about_action_triggered()
{
    d_aboutdialog = new AboutDialog(this);
    d_aboutdialog->setModal(true);
    d_aboutdialog->exec();
}
