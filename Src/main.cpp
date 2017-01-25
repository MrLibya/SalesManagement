#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include "logindialog.h"
#include "users.h"
#include <QStyleFactory>
#include "mymsg.h"
#include <QSplashScreen>
#include <QTimer>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);

    // DB
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QApplication::applicationDirPath()+ QDir::separator() + "SalesManagement.db");
    if(!QFile::exists(QApplication::applicationDirPath()+ QDir::separator() + "SalesManagement.db") || !db.open())
    {
        DBFailMsg();
        return 1;
    }
    QPixmap pixmap(":/icons/big.ico");
    // QSplashScreen *splash = new QSplashScreen(pixmap);
    std::unique_ptr<QSplashScreen> splash(new QSplashScreen(pixmap));
    splash->show();
    QTimer::singleShot(3000, splash.get(), SLOT(hide()));
    QTime later = QTime::currentTime().addSecs(3).addMSecs(300);
    notyet:
    if(QTime::currentTime() < later)
        goto notyet;

    //Login Dialog
    LoginDialog dialogl;

    dialogl.setModal(true);
    dialogl.exec();

    //End of Login Dialog

    // check if he didn't just close the login dialog
    if (users::id)
    {
        MainWindow w;
        //w.showFullScreen();
        w.show();
        splash->finish(&w);
        return a.exec();
    }
    return 1;
    // end if check
}
