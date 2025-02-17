#ifndef PAYMENTWINDOW_H
#define PAYMENTWINDOW_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class paymentwindow;
}

class paymentwindow : public QWidget
{
    Q_OBJECT

public:
    explicit paymentwindow(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~paymentwindow();

private slots:
    void loadUnpaidOrders(); // 加载未付款订单
    void onPayButtonClicked(); // 付款按钮点击事件`

private:
    Ui::paymentwindow *ui;
    int iduser;
    int selectedOrderId; // 当前选中的订单 ID
    QSqlQuery *sqlQuery;
};

#endif // PAYMENTWINDOW_H
