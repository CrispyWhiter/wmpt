#include "commentwindow.h"
#include "ui_commentwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>

commentwindow::commentwindow(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::commentwindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("评论");
    iduser=id;
    sqlQuery=s;

    // 连接订单列表的点击事件
    connect(ui->orderList, &QListWidget::itemClicked, this, &commentwindow::onOrderSelected);

    // 连接提交按钮的点击事件
    connect(ui->submitButton, &QPushButton::clicked, this, &commentwindow::onSubmitComment);

    // 加载历史订单
    loadOrders();
}

commentwindow::~commentwindow()
{
    delete ui;
}

void commentwindow::loadOrders()
{
    ui->orderList->clear();

    QString sql = "SELECT o.idorder, r.restname, o.status "
                  "FROM `order` o "
                  "JOIN rests r ON o.idrest = r.idrest "
                  "WHERE o.iduser = :iduser";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", iduser);
    if (!sqlQuery->exec())
    {
        qDebug() << "加载订单失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("加载订单失败，请稍后重试");
        return;
    }

    while (sqlQuery->next())
    {
        int idorder = sqlQuery->value("idorder").toInt();
        QString restName = sqlQuery->value("restname").toString();
        int status = sqlQuery->value("status").toInt();
        if (status!=2&&status!=3)
        {
            continue;
        }
        QString itemText = QString("订单号: %1, 餐厅: %2, 状态: %3")
                               .arg(idorder)
                               .arg(restName)
                               .arg(status == 2 ? "已完成" : "已评价");
        QListWidgetItem *item = new QListWidgetItem(itemText, ui->orderList);
        item->setData(Qt::UserRole, idorder);
    }

    if (ui->orderList->count() == 0)
    {
        ui->statusLabel->setText("您没有已完成或已评价的历史订单");
    }
    else
    {
        ui->statusLabel->setText("tips: 新的评价会覆盖旧的，这不是bug");
    }
}

void commentwindow::onOrderSelected(QListWidgetItem *item)
{
    selectedOrderId = item->data(Qt::UserRole).toInt();
    ui->statusLabel->setText("已选择订单号: " + QString::number(selectedOrderId));
}

void commentwindow::onSubmitComment()
{
    if (selectedOrderId == -1)
    {
        ui->statusLabel->setText("请先选择一个订单");
        return;
    }

    QString comment = ui->commentEdit->toPlainText().trimmed();
    if (comment.isEmpty())
    {
        ui->statusLabel->setText("评论内容不能为空");
        return;
    }

    QString sql = "DELETE FROM order_commit "
                   "WHERE idorder=:idorder AND iduser=:iduser";
    //qDebug()<<sql;  //如果炸了就用这个来看看是不是sql语句有问题
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    sqlQuery->bindValue(":iduser", iduser);
    if (!sqlQuery->exec())
    {
        qDebug() << "清除旧评论失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("清除旧评论失败，请稍后重试");
        return;
    }
    else
    {
        qDebug() << "清除了旧评论";
    }

    QString sql1 = "INSERT INTO order_commit (idorder, iduser, commit) "
                  "VALUES (:idorder, :iduser, :commit)";
    qDebug()<<sql1;  //如果炸了就用这个来看看是不是sql语句有问题
    sqlQuery->prepare(sql1);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    sqlQuery->bindValue(":iduser", iduser);
    sqlQuery->bindValue(":commit", comment);
    if (!sqlQuery->exec())
    {
        qDebug() << "插入评论失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("提交评论失败，请稍后重试");
        return;
    }

    QString sql2 = "UPDATE `order` SET status=3 "
                   "WHERE idorder=:idorder AND iduser=:iduser";
    //qDebug()<<sql2;  //如果炸了就用这个来看看是不是sql语句有问题
    sqlQuery->prepare(sql2);
    sqlQuery->bindValue(":idorder", selectedOrderId);
    sqlQuery->bindValue(":iduser", iduser);
    if (!sqlQuery->exec())
    {
        qDebug() << "更新标签失败:" << sqlQuery->lastError();
        ui->statusLabel->setText("更新标签失败，请稍后重试");
        return;
    }

    ui->statusLabel->setText("评论提交成功");
    ui->commentEdit->clear();
    selectedOrderId = -1;

    loadOrders(); //刷新订单选择控件
}
