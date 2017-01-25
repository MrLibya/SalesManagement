#ifndef REFUNDITEMDIALOG_H
#define REFUNDITEMDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QCompleter>

namespace Ui {
class RefundItemDialog;
}

class RefundItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RefundItemDialog(QWidget *parent = 0,size_t d_mode = 0);
    ~RefundItemDialog();
    enum Mode { Cash, PayLater, Reservation };
    size_t GetMode(){return _mode;}
private slots:
    void on_pushButton_clicked();
    void on_name_currentIndexChanged(const QString &arg1);

signals:
    void CanRefundItems(unsigned int);
    void CanRefundPayLaterItems(unsigned int);
    void CanUnretention_Items(unsigned int);
private:
    Ui::RefundItemDialog *ui;
    QSqlQuery *query;
    size_t _mode;
};

#endif // REFUNDITEMDIALOG_H
