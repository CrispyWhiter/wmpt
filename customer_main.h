#ifndef CUSTOMER_MAIN_H
#define CUSTOMER_MAIN_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTimer>

#include <QWidget>

namespace Ui {
class customer_main;
}

class customer_main : public QWidget
{
    Q_OBJECT

public:
    explicit customer_main(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~customer_main();

private slots:
    void on_edipwd_clicked();

    void on_selrest_clicked();

    void on_queord_clicked();

    void on_pushButton_clicked();

    void on_comord_clicked();

    void on_vieord_clicked();

    void load_ad();

private:
    Ui::customer_main *ui;
    int iduser;
    QSqlQuery *sqlQuery;
    QTimer ad;
    int test;
    void updatashow();
};

#endif // CUSTOMER_MAIN_H
