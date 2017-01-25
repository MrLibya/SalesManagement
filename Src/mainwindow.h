#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QtSql>
#include "logindialog.h"
#include "additemdialog.h"
#include "edititemdialog.h"
#include "users.h"
#include "typedef.h"
#include "billdialog.h"
#include "itemsloaddialog.h"
#include "selllaterdialog.h"
#include "refunditemdialog.h"
#include "clientsdialog.h"
#include "supplierdialog.h"
#include "expensesdialog.h"
#include "consumeritemdialog.h"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <reportsdialog.h>
#include <QPrinter>
#include "configdialog.h"
#include <QPrintDialog>
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void UpdateItemInfo();

    unsigned int CartItemAmount();
    int ItemInCart();
    QString SellType();
    // search line
    QCompleter *search;
    void SetSearchItem();
    //end of search line

    // Cart table
    QSqlTableModel *cartmodel;
    //end of cart table
    void FinishPayment();
    void ExpireCheck();
    void SetPrinter();
    void PrintCartBill();

private slots:
    void GetItemInfo();

    void on_LogoutButton_triggered();

    void on_AddCartButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_delete_item_clicked();

    void on_Cart_itemSelectionChanged();

    void on_additem_tab_triggered();

    void on_edititem_tab_triggered();

    void on_employee_tab_triggered();

    void on_add_Bill_tab_triggered();

    void GotBill(QString, QString, QString);

    void GotPayLaterOK(QString);

    void GotRetentionOK(QString,double);

    void GotCanRefundItems(unsigned int);

    void GotCanRefundPayLaterItems(unsigned int);

    void GotCanUnretention_Items(unsigned int);

    void on_price_sell_type_clicked();

    void on_price_wholesale_type_clicked();

    void on_price_buy_type_clicked();

    void on_pushButton_clicked();

    void on_clients_tab_triggered();

    void on_suppliers_tab_triggered();

    void on_expenses_tab_triggered();

    void on_consumer_item_tab_triggered();

    void TrayIconPressed();

    void on_retention_items_tab_triggered();

    void on_debt_manger_triggered();

    void GotUpdateMainItemQuery(){current_item->UpdateQuery();}

    void on_reports_tab_triggered();

    void on_config_tab_triggered();

    void on_about_action_triggered();

private:
    void closeEvent(QCloseEvent *event);
    bool mainWinowClose = false;
    QSystemTrayIcon *main_tray;
    QPrinter *printer;
    QPrintDialog *p_dialog;

    Ui::MainWindow *ui;
    additemdialog *d_additemdialog;
    edititemdialog *d_edititemdialog;
    SellLaterDialog *d_selllaterdialog;
    RefundItemDialog *d_refunditemdialog;
    SupplierDialog *d_supplierdialog;
    users *d_users;
    ClientsDialog *d_clientsdialog;
    ItemsLoadDialog *d_itemsloaddialog;
    LPITEM current_item;
    BillDialog *d_billdialog;
    ExpensesDialog *d_expensesdialog;
    ConsumerItemDialog *d_consumeritemdialog;
    ReportsDialog *d_reportsdialog;
    ConfigDialog *d_configdialog;
    AboutDialog *d_aboutdialog;
};

#endif // MAINWINDOW_H
