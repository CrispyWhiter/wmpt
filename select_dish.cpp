#include "select_dish.h"
#include "ui_select_dish.h"
#include "paymentwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QIntValidator>

// 已废弃

select_dish::select_dish(int id_rest, int id_user, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::select_dish)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("选择菜品");
    ui->id_dish->setValidator(new QIntValidator(this));
    ui->dish_num->setValidator(new QIntValidator(this));
    ui->total_price->setReadOnly(true);
    sqlQuery=s;
    iduser=id_user;
    idrest=id_rest;

    connect(this,SIGNAL(updataorder()),this,SLOT(showorder()));
    connect(this,SIGNAL(updatatotalprice()),this,SLOT(showtotalprice()));

    ui->listdish->clear();  //打印所选餐厅的菜单
    QString empty = "";
    QString empty32 = empty.leftJustified(32);
    QString empty24 = empty.leftJustified(24);
    ui->listdish->insertPlainText(("可选菜品\n"));
    ui->listdish->insertPlainText(("菜品id"));
    ui->listdish->insertPlainText(empty24);
    ui->listdish->insertPlainText("菜品名字");
    ui->listdish->insertPlainText(empty24);
    ui->listdish->insertPlainText("菜品价格\n");
    QString sqldish = QString("select idmenu,dish_name,dish_price from menu where idrest=%1").arg(idrest);
    if(sqlQuery->exec(sqldish))
    {
        while(sqlQuery->next())
        {
            ui->listdish->insertPlainText("\n");
            ui->listdish->insertPlainText(sqlQuery->value("idmenu").toString());
            ui->listdish->insertPlainText(empty32);
            ui->listdish->insertPlainText(sqlQuery->value("dish_name").toString());
            ui->listdish->insertPlainText(empty32);
            ui->listdish->insertPlainText(sqlQuery->value("dish_price").toString());
        }
        ui->listdish->insertPlainText("\n\n菜单读取完毕");
    }
    else
    {
        qDebug()<<"sqldish fail";
    }

    clean_empty_order();    //清除空白的点餐记录

    QString create_new_order = QString("insert into `order`(iduser,idrest,status) values('%1','%2','%3')").arg(iduser).arg(idrest).arg(0);
    //创建本次的点餐记录
    if(!sqlQuery->exec(create_new_order))
    {
        qDebug()<<"create new order fail"<<sqlQuery->lastError();
        return;
    }
    else
    {
        QString get_idorder = QString("select LAST_INSERT_ID() as id");
        sqlQuery->exec(get_idorder);
        sqlQuery->next();
        idorder = (sqlQuery->value("id")).toInt();
    }

    qDebug()<<"idorder: "<<idorder;
}

select_dish::~select_dish()
{
    delete ui;
}

int select_dish::clean_empty_order() //清除空白订单
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

void select_dish::showorder()    //刷新订单显示
{
    ui->listorder->clear();
    QString empty = "";
    QString empty32 = empty.leftJustified(32);
    QString empty24 = empty.leftJustified(24);
    ui->listorder->insertPlainText(("我的订单\n"));
    ui->listorder->insertPlainText(("菜品id"));
    ui->listorder->insertPlainText(empty24);
    ui->listorder->insertPlainText("菜品数量");
    QString showorder = QString("select iddish,dish_number from order_data where idorder=%1 and iduser=%2").arg(idorder).arg(iduser);
    if(sqlQuery->exec(showorder))
    {
        while(sqlQuery->next())
        {
            ui->listorder->insertPlainText("\n");
            ui->listorder->insertPlainText(sqlQuery->value("iddish").toString());
            ui->listorder->insertPlainText(empty32);
            ui->listorder->insertPlainText(sqlQuery->value("dish_number").toString());
        }
        ui->listorder->insertPlainText("\n\n订单读取完毕");
    }
    else
    {
        qDebug()<<"showorder fail";
    }
}

void select_dish::showtotalprice()   //刷新总价格
{
    // 初始化总价格为 0
    double totalPrice = 0.0;

    // 查询当前订单中的所有菜品及其数量
    QString queryOrderData = QString(
                                 "SELECT od.iddish, od.dish_number, m.dish_price "
                                 "FROM order_data od "
                                 "JOIN menu m ON od.iddish = m.idmenu "
                                 "WHERE od.idorder='%1'"
                                 ).arg(idorder);

    if (!sqlQuery->exec(queryOrderData)) {
        qDebug() << "查询订单数据失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, tr("错误"), tr("无法获取订单数据"));
        return;
    }

    // 遍历查询结果，计算总价格
    while (sqlQuery->next()) {
        int dishNumber = sqlQuery->value("dish_number").toInt(); // 菜品数量
        double dishPrice = sqlQuery->value("dish_price").toDouble(); // 菜品单价
        totalPrice += dishNumber * dishPrice; // 累加总价格
    }

    // 将总价格显示在 total_price QLineEdit 中
    ui->total_price->setText(QString::number(totalPrice, 'f', 2)); // 保留两位小数
}

void select_dish::on_select_dish_2_clicked() //选择
{
    // 输入验证
    if(ui->id_dish->text().isEmpty())
    {
        QMessageBox::warning(this, tr("XwX"), tr("请填写菜品id"));
        return;
    }

    // 获取菜品ID
    int dishId = ui->id_dish->text().toInt();

    // 检查是否已存在相同菜品
    QString checkDuplicate = QString(
                                 "SELECT 1 FROM order_data "
                                 "WHERE idorder='%1' AND iddish='%2'"
                                 ).arg(idorder).arg(dishId);

    if(sqlQuery->exec(checkDuplicate))
    {
        if(sqlQuery->next()) {  // 存在重复记录
            QMessageBox::warning(this, tr("XwX"), tr("你应该修改菜品数量而不是多次添加"));
            return;
        }
    }
    else
    {
        qDebug() << "重复检查失败:" << sqlQuery->lastError();
    }

    // 处理菜品数量
    int num = 1;
    if(!ui->dish_num->text().isEmpty())
    {
        num = ui->dish_num->text().toInt();
    }

    // 插入新记录
    QString insertSql = QString(
                            "INSERT INTO order_data(idorder,iddish,iduser,dish_number) "
                            "VALUES('%1','%2','%3','%4')"
                            ).arg(idorder).arg(dishId).arg(iduser).arg(num);

    if(!sqlQuery->exec(insertSql))
    {
        qDebug() << "插入失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, tr("XwX") ,tr("菜品id不存在"));
    }
    else
    {
        qDebug() << "添加菜品成功";
        emit updataorder();   //发送信号刷新界面
        emit showtotalprice();
    }
}

void select_dish::on_edit_num_clicked() //修改数量
{
    // 检查菜品ID是否为空
    if (ui->id_dish->text().isEmpty())
    {
        QMessageBox::warning(this, tr("XwX"), tr("请填写菜品ID"));
        return;
    }

    // 检查数量是否为空
    if (ui->dish_num->text().isEmpty())
    {
        QMessageBox::warning(this, tr("XwX"), tr("请填写菜品数量"));
        return;
    }

    // 获取菜品ID和数量
    int dishId = ui->id_dish->text().toInt();
    int newQuantity = ui->dish_num->text().toInt();

    // 检查数量是否合法（大于0）
    if (newQuantity <= 0)
    {
        QMessageBox::warning(this, tr("XwX"), tr("菜品数量必须大于0"));
        return;
    }

    // 检查菜品是否存在于当前订单中
    QString checkDishExists = QString(
                                  "SELECT 1 FROM order_data "
                                  "WHERE idorder='%1' AND iddish='%2'"
                                  ).arg(idorder).arg(dishId);

    if (!sqlQuery->exec(checkDishExists))
    {
        qDebug() << "检查菜品存在失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, tr("XwX"), tr("数据库查询失败"));
        return;
    }

    if (!sqlQuery->next())
    {
        QMessageBox::warning(this, tr("XwX"), tr("当前订单中不存在该菜品"));
        return;
    }

    // 更新菜品数量
    QString updateQuantity = QString(
                                 "UPDATE order_data "
                                 "SET dish_number='%1' "
                                 "WHERE idorder='%2' AND iddish='%3'"
                                 ).arg(newQuantity).arg(idorder).arg(dishId);

    if (!sqlQuery->exec(updateQuantity))
    {
        qDebug() << "更新菜品数量失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, tr("XwX"), tr("更新菜品数量失败"));
    }
    else
    {
        qDebug() << "菜品数量更新成功";
        QMessageBox::information(this, tr(">w<"), tr("菜品数量已更新"));
        emit updataorder(); // 发送信号更新界面
        emit showtotalprice();
    }
}


void select_dish::on_delete_dish_clicked() //删除
{
    // 检查菜品ID是否为空
    if (ui->id_dish->text().isEmpty())
    {
        QMessageBox::warning(this, tr("XwX"), tr("请填写菜品ID"));
        return;
    }

    // 获取菜品ID
    int dishId = ui->id_dish->text().toInt();

    // 检查菜品是否存在于当前订单中
    QString checkDishExists = QString(
                                  "SELECT 1 FROM order_data "
                                  "WHERE idorder='%1' AND iddish='%2'"
                                  ).arg(idorder).arg(dishId);

    if (!sqlQuery->exec(checkDishExists))
    {
        qDebug() << "检查菜品存在失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, tr("XwX"), tr("数据库查询失败"));
        return;
    }

    if (!sqlQuery->next())
    {
        QMessageBox::warning(this, tr("XwX"), tr("当前订单中不存在该菜品"));
        return;
    }

    // 删除菜品
    QString deleteDish = QString(
                             "DELETE FROM order_data "
                             "WHERE idorder='%1' AND iddish='%2'"
                             ).arg(idorder).arg(dishId);

    if (!sqlQuery->exec(deleteDish))
    {
        qDebug() << "删除菜品失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, tr("XwX"), tr("删除菜品失败"));
    }
    else
    {
        qDebug() << "删除菜品成功";
        QMessageBox::information(this, tr(">w<"), tr("菜品已删除"));
        emit updataorder(); // 发送信号更新界面
        emit updatatotalprice(); // 更新总价格显示
    }
}


void select_dish::on_order_clicked()
{
    paymentwindow *w = new paymentwindow(iduser,sqlQuery);
    w->show();
    this->close();
}

