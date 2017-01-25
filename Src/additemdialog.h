#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>

namespace Ui {
class additemdialog;
}

class additemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit additemdialog(QWidget *parent = 0);
    ~additemdialog();

private slots:
    void on_pushButton_clicked();
signals:
    void UpdateMainItemQuery();
private:
    Ui::additemdialog *ui;
};

#endif // ADDITEMDIALOG_H
