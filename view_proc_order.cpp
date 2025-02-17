#include "view_proc_order.h"
#include "ui_view_proc_order.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTimer>

view_proc_order::view_proc_order(int id_rest, int id_user, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::view_proc_order)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("查看/处理订单");
    sqlQuery=s;
    iduser=id_user;
    idrest=id_rest;
    selectedOrderId = -1;
    selectedOrderStatus = -1;

    connect(ui->orderList, &QListWidget::itemClicked, this, &view_proc_order::onOrderSelected);

    connect(ui->updateStatusButton, &QPushButton::clicked, this, &view_proc_order::onUpdateStatusButtonClicked);

    loadOrders();

    // 不断更新订单处理列表，这个不一定用得上
    QTimer *updataorder = new QTimer(this);
    connect(updataorder, &QTimer::timeout, this, &view_proc_order::loadOrders);
    updataorder->start(1000);
}

view_proc_order::~view_proc_order()
{
    delete ui;
}

// 加载所有订单
void view_proc_order::loadOrders()
{
    if(test==1)         //测试专用
    {
        qDebug()<<"2 订单处理界面定时器";
        test = 0;
    }
    else
    {
        qDebug()<<"3";
        test = 1;
    }

    ui->orderList->clear();

    // 查询当前商家的所有订单
    QString sql = "SELECT o.idorder, u.name, o.status "
                  "FROM `order` o "
                  "JOIN users u ON o.iduser = u.iduser "
                  "WHERE o.idrest = :idrest";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idrest", idrest);
    if (!sqlQuery->exec())
    {
        qDebug() << "加载订单失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载订单失败，请稍后重试。");
        return;
    }

    // 遍历查询结果，添加到订单列表
    while (sqlQuery->next())
    {
        int idorder = sqlQuery->value("idorder").toInt();
        QString userName = sqlQuery->value("name").toString();
        int status = sqlQuery->value("status").toInt();

        // 将订单信息添加到 QListWidget
        QString itemText = QString("订单号: %1, 用户: %2, 状态: %3")
                               .arg(idorder)
                               .arg(userName)
                               .arg(status == -1 ? "已拒绝" : (status == 0 ? "未付款" : (status == 1 ? "已付款" : (status == 2 ? "已完成" : "已评价"))));
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->orderList);
        item->setData(Qt::UserRole, idorder); // 将 idorder 存储为 UserRole
        item->setData(Qt::UserRole + 1, status); // 将 order.status 存储为 UserRole + 1
    }

    if (ui->orderList->count() == 0)
    {
        ui->statusLabel->setText("您没有订单。");
    }
}

// 选择订单
void view_proc_order::onOrderSelected(QListWidgetItem *item)
{
    selectedOrderId = item->data(Qt::UserRole).toInt();
    selectedOrderStatus = item->data(Qt::UserRole + 1).toInt();

    QString detailText;

    // 获取用户名称和订单状态
    QString sql = "SELECT u.name, o.status "
                  "FROM `order` o "
                  "JOIN users u ON o.iduser = u.iduser "
                  "WHERE o.idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "查询订单详情失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载订单详情失败，请稍后重试。");
        return;
    }

    if (sqlQuery->next())
    {
        QString userName = sqlQuery->value("name").toString();
        int status = sqlQuery->value("status").toInt();

        detailText += QString("用户: %1\n状态: %2\n\n")
                          .arg(userName)
                          .arg(status == -1 ? "已拒绝" : (status == 0 ? "未付款" : (status == 1 ? "已付款" : (status == 2 ? "已完成" : "已评价"))));
    }

    // 获取订单的点菜内容
    sql = "SELECT m.dish_name, od.dish_number, m.dish_price "
          "FROM order_data od "
          "JOIN menu m ON od.iddish = m.idmenu "
          "WHERE od.idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "查询点菜内容失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载点菜内容失败，请稍后重试。");
        return;
    }

    detailText += "点菜内容:\n";
    while (sqlQuery->next())
    {
        QString dishName = sqlQuery->value("dish_name").toString();
        int quantity = sqlQuery->value("dish_number").toInt();
        double dishPrice = sqlQuery->value("dish_price").toDouble();

        detailText += QString("  - %1 x %2, 单价: %3 元\n")
                          .arg(dishName)
                          .arg(quantity)
                          .arg(dishPrice, 0, 'f', 2);
    }

    // 获取订单的评论内容（如果状态为已评价）
    if (selectedOrderStatus == 3)
    {
        sql = "SELECT commit FROM order_commit WHERE idorder = :idorder";
        sqlQuery->prepare(sql);
        sqlQuery->bindValue(":idorder", selectedOrderId);
        if (!sqlQuery->exec())
        {
            qDebug() << "查询评论失败:" << sqlQuery->lastError();
            ui->statusLabel->setText("加载评论失败，请稍后重试");
            return;
        }

        if (sqlQuery->next())
        {
            QString comment = sqlQuery->value("commit").toString();
            detailText += "\n评论:\n" + comment;
        }
    }

    ui->orderDetailBrowser->setText(detailText);
}

// 更新订单状态
void view_proc_order::onUpdateStatusButtonClicked()
{
    // 检查是否选择了订单
    if (selectedOrderId == -1)
    {
        ui->statusLabel->setText("请先选择一个订单");
        return;
    }

    // 确定新状态
    int newStatus = -1;
    if (selectedOrderStatus == 0)
    {
        newStatus = 1; // 未付款 -> 已付款
    }
    else if (selectedOrderStatus == 1)
    {
        newStatus = 2; // 已付款 -> 已完成
    }
    else
    {
        ui->statusLabel->setText("该订单状态无法更新");
        return;
    }

    // 更新订单状态
    QString sql = "UPDATE `order` SET status = :status WHERE idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":status", newStatus);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "更新订单状态失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("更新订单状态失败，请稍后重试");
        return;
    }


    ui->statusLabel->setText("订单状态已更新");   // 更新界面状态
    loadOrders();
    selectedOrderId = -1;
    selectedOrderStatus = -1;
}




void view_proc_order::on_refuseButton_clicked()
{
    // 检查是否选择了订单
    if (selectedOrderId == -1)
    {
        ui->statusLabel->setText("请先选择一个订单");
        return;
    }

    // 确定新状态
    int newStatus = -1;
    if (selectedOrderStatus == 0)
    {
        newStatus = -1; // 未付款 -> 拒绝订单
    }
    else if (selectedOrderStatus == 1)
    {
        newStatus = -1; // 已付款 -> 拒绝订单
    }
    else
    {
        ui->statusLabel->setText("该订单状态无法更新");
        return;
    }

    // 更新订单状态
    QString sql = "UPDATE `order` SET status = :status WHERE idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":status", newStatus);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "更新订单状态失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("更新订单状态失败，请稍后重试");
        return;
    }


    ui->statusLabel->setText("已拒绝订单");   // 更新界面状态
    loadOrders();
    selectedOrderId = -1;
    selectedOrderStatus = -1;
}

