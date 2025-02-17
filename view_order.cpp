#include "view_order.h"
#include "ui_view_order.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QList>

view_order::view_order(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::view_order)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("历史订单详情");
    iduser=id;
    sqlQuery=s;

    connect(ui->orderList, &QListWidget::itemClicked, this, &view_order::onOrderSelected);

    loadOrders();
}

view_order::~view_order()
{
    delete ui;
}

void view_order::loadOrders()
{
    ui->orderList->clear();

    // 查询当前用户的所有订单
    QString sql = "SELECT o.idorder, r.restname, o.status "
                  "FROM `order` o "
                  "JOIN rests r ON o.idrest = r.idrest "
                  "WHERE o.iduser = :iduser";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", iduser);
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
        QString restName = sqlQuery->value("restname").toString();
        int status = sqlQuery->value("status").toInt();

        // 将订单信息添加到 QListWidget
        QString itemText = QString("订单号: %1, 餐厅: %2, 状态: %3")
                               .arg(idorder)
                               .arg(restName)
                               .arg(status == -1 ? "已拒绝" : (status == 0 ? "未付款" : (status == 1 ? "已付款" : (status == 2 ? "已完成" : "已评价"))));
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->orderList);
        item->setData(Qt::UserRole, idorder); // 将订单 ID 存储为 UserRole
    }

    if (ui->orderList->count() == 0)
    {
        ui->statusLabel->setText("您没有历史订单。");
    }
}

// 选择订单
void view_order::onOrderSelected(QListWidgetItem *item)
{
    int orderId = item->data(Qt::UserRole).toInt();

    QString detailText;

    // 获取餐厅名称和订单状态
    QString sql = "SELECT r.restname, o.status "
                  "FROM `order` o "
                  "JOIN rests r ON o.idrest = r.idrest "
                  "WHERE o.idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", orderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "查询订单详情失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载订单详情失败，请稍后重试。");
        return;
    }


    if (sqlQuery->next()) {
        QString restName = sqlQuery->value("restname").toString();
        int status = sqlQuery->value("status").toInt();

        detailText += QString("餐厅: %1\n状态: %2\n\n")
                          .arg(restName)
                          .arg(status == -1 ? "已拒绝" : (status == 0 ? "未付款" : (status == 1 ? "已付款" : (status == 2 ? "已完成" : "已评价"))));
    }

    // 获取订单的点菜内容
    sql = "SELECT m.dish_name, od.dish_number, m.dish_price "
          "FROM order_data od "
          "JOIN menu m ON od.iddish = m.idmenu "
          "WHERE od.idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", orderId);
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

    // 获取订单的评论内容
    sql = "SELECT commit FROM order_commit WHERE idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", orderId);
    if (!sqlQuery->exec())
    {
        qDebug() << "查询评论失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载评论失败，请稍后重试。");
        return;
    }

    if (sqlQuery->next())
    {
        QString comment = sqlQuery->value("commit").toString();
        detailText += "\n评论:\n" + comment;
    }

    // 显示订单详情
    ui->orderDetailBrowser->setText(detailText);
    ui->statusLabel->setText("读取订单详情完毕");
}

void view_order::on_query_clicked()
{
    ui->orderList->clear();

    QString query_text = ui->type->text().toUtf8();
    QList<int> idorder_list;
    QList<int> query_true;

    // 查询当前用户的所有订单ID
    QString sql = "SELECT o.idorder "
                  "FROM `order` o "
                  "JOIN rests r ON o.idrest = r.idrest "
                  "WHERE o.iduser = :iduser";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", iduser);
    if (!sqlQuery->exec())
    {
        qDebug() << "加载订单失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载订单失败，请稍后重试。");
        return;
    }

    // 遍历查询结果，添加到订单ID列表
    while (sqlQuery->next())
    {
        idorder_list.append(sqlQuery->value("idorder").toInt());
    }

    foreach (int idorder, idorder_list)
    {
        int orderId = idorder;

        QString detailText;

        detailText += QString::number(orderId) + " ";

        // 获取餐厅名称和订单状态
        QString sql = "SELECT r.restname, o.status "
                      "FROM `order` o "
                      "JOIN rests r ON o.idrest = r.idrest "
                      "WHERE o.idorder = :idorder";
        sqlQuery->prepare(sql);
        sqlQuery->bindValue(":idorder", orderId);
        if (!sqlQuery->exec())
        {
            qDebug() << "查询订单详情失败:" << sqlQuery->lastError();
            return;
        }


        if (sqlQuery->next()) {
            QString restName = sqlQuery->value("restname").toString().toUtf8();
            int status = sqlQuery->value("status").toInt();

            detailText += QString("%1 %2 ")
                              .arg(restName)
                              .arg(status == -1 ? "已拒绝" : (status == 0 ? "未付款" : (status == 1 ? "已付款" : (status == 2 ? "已完成" : "已评价"))));
        }

        // 获取订单的点菜内容
        sql = "SELECT m.dish_name "
              "FROM order_data od "
              "JOIN menu m ON od.iddish = m.idmenu "
              "WHERE od.idorder = :idorder";
        sqlQuery->prepare(sql);
        sqlQuery->bindValue(":idorder", orderId);
        if (!sqlQuery->exec())
        {
            qDebug() << "查询点菜内容失败:" << sqlQuery->lastError();
            return;
        }

        while (sqlQuery->next())
        {
            QString dishName = sqlQuery->value("dish_name").toString().toUtf8();


            detailText += QString("%1 ")
                              .arg(dishName);
        }

        // 获取订单的评论内容
        sql = "SELECT commit FROM order_commit WHERE idorder = :idorder";
        sqlQuery->prepare(sql);
        sqlQuery->bindValue(":idorder", orderId);
        if (!sqlQuery->exec())
        {
            qDebug() << "查询评论失败:" << sqlQuery->lastError();
            return;
        }

        if (sqlQuery->next())
        {
            QString comment = sqlQuery->value("commit").toString().toUtf8();
            detailText +=comment+" ";
        }

        qDebug()<<detailText;

        if(detailText.contains(query_text, Qt::CaseInsensitive))
        {
            query_true.append(orderId);
        }
    }

    foreach (int idorder, query_true)
    {
        //qDebug()<<"第一轮查询结果ID："<<idorder;

        // 查询当前用户的所有符合特征的订单
        QString sql = "SELECT o.idorder, r.restname, o.status "
                      "FROM `order` o "
                      "JOIN rests r ON o.idrest = r.idrest "
                      "WHERE o.iduser = :iduser AND o.idorder = :idorder";
        sqlQuery->prepare(sql);
        sqlQuery->bindValue(":iduser", iduser);
        sqlQuery->bindValue(":idorder", idorder);
        if (!sqlQuery->exec())
        {
            qDebug() << "加载订单失败:" << sqlQuery->lastError();
            return;
        }

        //这种情况下应该每轮循环最多查出来一个结果，有多个是不太可能的
        sqlQuery->next();

        int idorder1 = sqlQuery->value("idorder").toInt();
        QString restName = sqlQuery->value("restname").toString();
        int status = sqlQuery->value("status").toInt();

        // 将订单信息添加到 QListWidget
        QString itemText = QString("订单号: %1, 餐厅: %2, 状态: %3")
                            .arg(idorder1)
                            .arg(restName)
                            .arg(status == -1 ? "已拒绝" : (status == 0 ? "未付款" : (status == 1 ? "已付款" : (status == 2 ? "已完成" : "已评价"))));
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->orderList);
        item->setData(Qt::UserRole, idorder); // 将订单 ID 存储为 UserRole

    }

    if (ui->orderList->count() == 0)
    {
        ui->statusLabel->setText("您没有符合特征的历史订单。");
    }
}

