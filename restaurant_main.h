#ifndef RESTAURANT_MAIN_H
#define RESTAURANT_MAIN_H

#include "qtimer.h"
#include "view_proc_order.h"
#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class restaurant_main;
}

class restaurant_main : public QWidget
{
    Q_OBJECT

public:
    explicit restaurant_main(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~restaurant_main();

private slots:
    void on_editpassword_clicked();

    void on_editmessage_clicked();

    void on_editmenu_clicked();

    void on_pushButton_clicked();

    void on_viewincome_clicked();

public slots:
    void updateshow();
    void notice_new_order();
    void edit_menu_w_close();

signals:
    void update_show();

private:
    Ui::restaurant_main *ui;
    int iduser;
    int idrest;
    int isopen_this;
    int isopen_window;
    QSqlQuery *sqlQuery;
    QTimer timer_notice_new_order;
    int test;
    view_proc_order *view_proc_order_w;
};

#endif // RESTAURANT_MAIN_H
