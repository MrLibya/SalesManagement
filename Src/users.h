#ifndef USERS_H
#define USERS_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class users;
}

class users : public QDialog
{
    Q_OBJECT

public:
    explicit users(QWidget *parent = 0);
    ~users();

    static int id;
    static int level;
    static QString name;

private slots:
    void on_pushButton_clicked();

private:
    Ui::users *ui;
    QSqlTableModel *model;
};

enum Level
{
    SELLER=1,
    CO,
    BOSS,
};

#endif // USERS_H
