#ifndef SELECT_DISH_H
#define SELECT_DISH_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QWidget>

namespace Ui {
class select_dish;
}

class select_dish : public QWidget
{
    Q_OBJECT

public:
    explicit select_dish(int id_rest,int id_user,QSqlQuery *s,QWidget *parent = nullptr);
    ~select_dish();

signals:
    void updataorder();
    void updatatotalprice();

private slots:
    void showorder();

    void showtotalprice();

    void on_select_dish_2_clicked();

    void on_edit_num_clicked();

    void on_delete_dish_clicked();

    void on_order_clicked();

private:
    Ui::select_dish *ui;
    int idrest;
    int iduser;
    int idorder;
    QSqlQuery *sqlQuery;
    int clean_empty_order();
};

#endif // SELECT_DISH_H
