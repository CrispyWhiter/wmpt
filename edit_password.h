#ifndef EDIT_PASSWORD_H
#define EDIT_PASSWORD_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QWidget>

namespace Ui {
class edit_password;
}

class edit_password : public QWidget
{
    Q_OBJECT

public:
    explicit edit_password(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~edit_password();

private slots:
    void on_cancel_clicked();

    void on_edit_clicked();

private:
    Ui::edit_password *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // EDIT_PASSWORD_H
