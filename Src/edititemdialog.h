#ifndef EDITITEMDIALOG_H
#define EDITITEMDIALOG_H

#include <QDialog>

namespace Ui {
class edititemdialog;
}

class edititemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit edititemdialog(QWidget *parent = 0);
    ~edititemdialog();
    void edititemdialog::CanEdit();

private slots:
    void on_pushButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_3_clicked();

private:
    Ui::edititemdialog *ui;
};

#endif // EDITITEMDIALOG_H
