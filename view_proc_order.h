#ifndef VIEW_PROC_ORDER_H
#define VIEW_PROC_ORDER_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class view_proc_order;
}

class view_proc_order : public QWidget
{
    Q_OBJECT

public:
    explicit view_proc_order(int id_rest,int id_user,QSqlQuery *s,QWidget *parent = nullptr);
    ~view_proc_order();

private slots:
    void loadOrders();
    void onOrderSelected(QListWidgetItem *item);
    void onUpdateStatusButtonClicked();
    void on_refuseButton_clicked();

private:
    Ui::view_proc_order *ui;
    int idrest;
    int iduser;
    QSqlQuery *sqlQuery;
    int selectedOrderId; // 当前选中的订单 ID
    int selectedOrderStatus; // 当前选中的订单状态
    int test;
};

#endif // VIEW_PROC_ORDER_H
