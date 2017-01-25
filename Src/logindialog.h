#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    LoginDialog *dlg_login;

private slots:
    void on_OK_clicked();

signals:
    //void on_SuccessfulLgoin();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
