#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    static QString _shopName;
    static int _paperWidth;
    static int _paperHigh;
    static int _cartID;

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
