#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
namespace Ui {
class login;
}
QT_END_NAMESPACE

class login : public QWidget
{
    Q_OBJECT

public:
    login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_Quit_clicked();

    void on_Reg_clicked();

    void on_Login_clicked();

private:
    Ui::login *ui;
    QSqlDatabase wmptdb;
    QSqlError sqlError;
    QSqlQuery *sqlQuery;
    QSqlRecord sqlRecord;
};
#endif // LOGIN_H
