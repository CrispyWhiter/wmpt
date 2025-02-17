#include "select_dish_2.h"
#include "ui_select_dish_2.h"
#include "paymentwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>

select_dish_2::select_dish_2(int id_rest, int id_user, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::select_dish_2)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("选择菜品");
    sqlQuery=s;
    iduser=id_user;
    idrest=id_rest;

    // 设置表格列数
    ui->menuTable->setColumnCount(3);
    ui->menuTable->setHorizontalHeaderLabels({"菜品ID", "菜品名称", "价格"});

    ui->orderTable->setColumnCount(3);
    ui->orderTable->setHorizontalHeaderLabels({"菜品ID", "菜品名称", "数量"});

    // 连接按钮点击事件
    connect(ui->addButton, &QPushButton::clicked, this, &select_dish_2::onAddDishClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &select_dish_2::onEditQuantityClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &select_dish_2::onDeleteDishClicked);
    connect(ui->orderButton, &QPushButton::clicked, this, &select_dish_2::onOrderClicked);

    // 加载菜单
    loadMenu();

    // 清理空订单
    clean_empty_order();

    // 创建新订单
    QString createOrder = QString("INSERT INTO `order` (iduser, idrest, status) VALUES (%1, %2, 0)").arg(iduser).arg(idrest);
    if (sqlQuery->exec(createOrder)) {
        idorder = sqlQuery->lastInsertId().toInt();
        qDebug() << "新订单创建成功，订单ID:" << idorder;
    } else {
        qDebug() << "创建订单失败:" << sqlQuery->lastError();
    }

    // 加载订单
    loadOrder();
}

select_dish_2::~select_dish_2()
{
    delete ui;
}

int select_dish_2::clean_empty_order() //清除空白订单
{
    QSqlQuery *query = sqlQuery;
    int exec = 0;

    // 获取所有订单ID并存入列表
    QString getOrders = QString("SELECT idorder FROM `order` WHERE iduser='%1'").arg(iduser);
    if (!query->exec(getOrders))
    {
        qDebug() << "获取订单失败:" << query->lastError();
        return exec;
    }

    QList<int> orderIds;
    while (query->next())
    {
        orderIds.append(query->value("idorder").toInt());
    }

    // 遍历本地列表处理每个订单
    foreach (int idorder, orderIds)
    {
        QString checkData = QString("SELECT idorder_data FROM order_data WHERE idorder=%1").arg(idorder);
        if (!query->exec(checkData))
        {
            qDebug() << "检查订单数据失败:" << query->lastError();
            continue;
        }

        if (!query->next())
        { // 无关联数据，删除订单
            QString deleteOrder = QString("DELETE FROM `order` WHERE idorder=%1").arg(idorder);
            if (query->exec(deleteOrder))
            {
                qDebug() << "成功清理空订单:" << idorder;
                exec = 1;
            } else {
                qDebug() << "删除失败:" << query->lastError();
            }
        }
        else
        {
            qDebug() << "订单" << idorder << "非空，跳过";
        }
    }

    return exec;
}

// 加载菜单
void select_dish_2::loadMenu() {
    ui->menuTable->setRowCount(0);

    QString sql = "SELECT idmenu, dish_name, dish_price FROM menu WHERE idrest = :idrest";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idrest", idrest);
    if (!sqlQuery->exec()) {
        qDebug() << "加载菜单失败:" << sqlQuery->lastError();
        return;
    }

    while (sqlQuery->next()) {
        int idmenu = sqlQuery->value("idmenu").toInt();
        QString dishName = sqlQuery->value("dish_name").toString();
        double dishPrice = sqlQuery->value("dish_price").toDouble();

        int row = ui->menuTable->rowCount();
        ui->menuTable->insertRow(row);

        ui->menuTable->setItem(row, 0, new QTableWidgetItem(QString::number(idmenu)));
        ui->menuTable->setItem(row, 1, new QTableWidgetItem(dishName));
        ui->menuTable->setItem(row, 2, new QTableWidgetItem(QString::number(dishPrice, 'f', 2)));
    }
}

// 加载订单
void select_dish_2::loadOrder() {
    ui->orderTable->setRowCount(0);

    QString sql = "SELECT od.iddish, m.dish_name, od.dish_number "
                  "FROM order_data od "
                  "JOIN menu m ON od.iddish = m.idmenu "
                  "WHERE od.idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", idorder);
    if (!sqlQuery->exec()) {
        qDebug() << "加载订单失败:" << sqlQuery->lastError();
        return;
    }

    while (sqlQuery->next()) {
        int idmenu = sqlQuery->value("iddish").toInt();
        QString dishName = sqlQuery->value("dish_name").toString();
        int quantity = sqlQuery->value("dish_number").toInt();

        int row = ui->orderTable->rowCount();
        ui->orderTable->insertRow(row);

        ui->orderTable->setItem(row, 0, new QTableWidgetItem(QString::number(idmenu)));
        ui->orderTable->setItem(row, 1, new QTableWidgetItem(dishName));
        ui->orderTable->setItem(row, 2, new QTableWidgetItem(QString::number(quantity)));
    }

    // 更新总价格
    updateTotalPrice();
}

// 更新总价格
void select_dish_2::updateTotalPrice() {
    double totalPrice = 0.0;

    QString sql = "SELECT od.dish_number, m.dish_price "
                  "FROM order_data od "
                  "JOIN menu m ON od.iddish = m.idmenu "
                  "WHERE od.idorder = :idorder";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", idorder);
    if (!sqlQuery->exec()) {
        qDebug() << "计算总价格失败:" << sqlQuery->lastError();
        return;
    }

    while (sqlQuery->next()) {
        int quantity = sqlQuery->value("dish_number").toInt();
        double price = sqlQuery->value("dish_price").toDouble();
        totalPrice += quantity * price;
    }

    ui->totalPriceLabel->setText(QString("总价格: %1 元").arg(totalPrice, 0, 'f', 2));
}

// 点菜
void select_dish_2::onAddDishClicked() {
    QTableWidgetItem *item = ui->menuTable->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请先选择一个菜品。");
        return;
    }

    int row = ui->menuTable->currentRow();
    int idmenu = ui->menuTable->item(row, 0)->text().toInt();
    int quantity = ui->quantitySpinBox->value();

    if (quantity <= 0) {
        QMessageBox::warning(this, "提示", "数量必须大于 0。");
        return;
    }

    QString sql = "INSERT INTO order_data (idorder, iddish, iduser, dish_number) "
                  "VALUES (:idorder, :iddish, :iduser, :quantity)";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", idorder);
    sqlQuery->bindValue(":iddish", idmenu);
    sqlQuery->bindValue(":iduser", iduser);
    sqlQuery->bindValue(":quantity", quantity);
    if (!sqlQuery->exec()) {
        qDebug() << "点菜失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "点菜失败，请稍后重试。");
        return;
    }

    loadOrder(); // 刷新订单
}

// 修改数量
void select_dish_2::onEditQuantityClicked() {
    QTableWidgetItem *item = ui->orderTable->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请先选择一个菜品。");
        return;
    }

    int row = ui->orderTable->currentRow();
    int idmenu = ui->orderTable->item(row, 0)->text().toInt();
    int quantity = ui->quantitySpinBox->value();

    if (quantity <= 0) {
        QMessageBox::warning(this, "提示", "数量必须大于 0。");
        return;
    }

    QString sql = "UPDATE order_data SET dish_number = :quantity "
                  "WHERE idorder = :idorder AND iddish = :iddish";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":quantity", quantity);
    sqlQuery->bindValue(":idorder", idorder);
    sqlQuery->bindValue(":iddish", idmenu);
    if (!sqlQuery->exec()) {
        qDebug() << "修改数量失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "修改数量失败，请稍后重试。");
        return;
    }

    loadOrder(); // 刷新订单
}

// 删除菜品
void select_dish_2::onDeleteDishClicked() {
    QTableWidgetItem *item = ui->orderTable->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请先选择一个菜品。");
        return;
    }

    int row = ui->orderTable->currentRow();
    int idmenu = ui->orderTable->item(row, 0)->text().toInt();

    QString sql = "DELETE FROM order_data WHERE idorder = :idorder AND iddish = :iddish";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder", idorder);
    sqlQuery->bindValue(":iddish", idmenu);
    if (!sqlQuery->exec()) {
        qDebug() << "删除菜品失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "删除菜品失败，请稍后重试。");
        return;
    }

    loadOrder(); // 刷新订单
}

// 提交订单
void select_dish_2::onOrderClicked() {
    paymentwindow *w = new paymentwindow(iduser, sqlQuery);
    w->show();
    this->close();
}
