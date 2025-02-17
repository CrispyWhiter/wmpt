#include "paymentwindow.h"
#include "ui_paymentwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

paymentwindow::paymentwindow(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::paymentwindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("付款");
    sqlQuery=s;
    iduser=id;

    // 连接付款按钮的点击事件
    connect(ui->payButton, &QPushButton::clicked, this, &paymentwindow::onPayButtonClicked);

    // 加载未付款订单
    loadUnpaidOrders();

}

paymentwindow::~paymentwindow()
{
    delete ui;
}

// 加载未付款订单
void paymentwindow::loadUnpaidOrders()
{
    // 清空订单列表
    ui->orderList->clear();

    // 查询未付款订单
    QString sql = "SELECT o.idorder, r.restname "
                  "FROM `order` o "
                  "JOIN rests r ON o.idrest = r.idrest "
                  "WHERE o.iduser = :iduser AND o.status = 0"; // 状态为 0 表示未付款
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", iduser);
    if (!sqlQuery->exec())
    {
        qDebug() << "加载未付款订单失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载订单失败，请稍后重试。");
        return;
    }

    // 遍历查询结果，添加到订单列表
    while (sqlQuery->next())
    {
        int idorder = sqlQuery->value("idorder").toInt();
        QString restName = sqlQuery->value("restname").toString();

        // 将订单信息添加到 QListWidget
        QString itemText = QString("订单号: %1, 餐厅: %2").arg(idorder).arg(restName);
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->orderList);
        item->setData(Qt::UserRole, idorder); // 将订单 ID 存储为 UserRole
    }

    // 如果没有未付款订单
    if (ui->orderList->count() == 0)
    {
        ui->statusLabel->setText("您没有未付款的订单。");
    }
}

// 付款按钮点击事件
void paymentwindow::onPayButtonClicked()
{
    // 获取当前选中的订单
    QListWidgetItem *selectedItem = ui->orderList->currentItem();
    if (!selectedItem)
    {
        ui->statusLabel->setText("请先选择一个订单。");
        return;
    }

    // 获取订单 ID
    int orderId = selectedItem->data(Qt::UserRole).toInt();

    // 更新订单状态为已付款
    QSqlQuery query;
    QString sql = "UPDATE `order` SET status = 1 WHERE idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", orderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "更新订单状态失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("付款失败，请稍后重试。");
        return;
    }

    // 更新界面状态
    ui->statusLabel->setText("付款成功！订单号: " + QString::number(orderId));
    loadUnpaidOrders(); // 重新加载未付款订单
}
