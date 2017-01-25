#ifndef SELLLATERDIALOG_H
#define SELLLATERDIALOG_H

#include <QDialog>
#include <QCompleter>
#include <QSqlQuery>

namespace Ui {
class SellLaterDialog;
}

class SellLaterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SellLaterDialog(QWidget *parent = 0,size_t d_mode = 0);
    ~SellLaterDialog();
    enum Mode { Normal, Reservation, AddDebt };
    size_t GetMode(){return _mode;}
private slots:
    void GetClientInfo();
    void on_pushButton_clicked();

signals:
    void PayLaterOK(QString);
    void RetentionOK(QString, double);
private:
    Ui::SellLaterDialog *ui;
    QStringList *List;
    size_t _mode;
    unsigned int clientId;
};

#endif // SELLLATERDIALOG_H
