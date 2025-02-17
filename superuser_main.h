#ifndef SUPERUSER_MAIN_H
#define SUPERUSER_MAIN_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class superuser_main;
}

class superuser_main : public QWidget
{
    Q_OBJECT

public:
    explicit superuser_main(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~superuser_main();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::superuser_main *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // SUPERUSER_MAIN_H
