#include "view_income.h"
#include "ui_view_income.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QList>

view_income::view_income(int id_rest, int id_user, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::view_income)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("查看收入");
    sqlQuery=s;
    iduser=id_user;
    idrest=id_rest;
    totalincome = 0;
    totalorder = 0;

    totalincome = gettotalincome();
    totalorder = gettotalorder();

    ui->income->setText(QString::number(totalincome));
    ui->order->setText(QString::number(totalorder));
}

view_income::~view_income()
{
    delete ui;
}

int view_income::gettotalincome()
{
    // //暂存订单id
    // QList<int> idorderList;
    // QString sql = "SELECT idorder FROM `order` WHERE idrest=:idrest";
    // sqlQuery->prepare(sql);
    // sqlQuery->bindValue(":idrest",idrest);
    // sqlQuery->exec();
    // while(sqlQuery->next())
    // {
    //     idorderList.append(sqlQuery->value("idorder").toInt());
    // }

    // //逐个计算总价格
    // foreach (int idorder, idorderList)
    // {
    //     qDebug()<<idorder;
    //     QString sql = "SELECT od.dish_number, m.dish_price "
    //                   "FROM order_data od "
    //                   "JOIN menu m ON od.iddish = m.idmenu "
    //                   "WHERE od.idorder = :idorder";
    //     sqlQuery->prepare(sql);
    //     sqlQuery->bindValue(":idorder",idorder);
    //     if(!sqlQuery->exec())
    //     {
    //         qDebug()<<sqlQuery->lastError();
    //     }
    //     while (sqlQuery->next())
    //     {
    //         int price = sqlQuery->value("dish_price").toInt();
    //         int num = sqlQuery->value("dish_number").toInt();
    //         qDebug()<<price;
    //         qDebug()<<num;
    //         totalincome += (price*num);
    //         qDebug()<<totalincome;
    //     }

    // }

    // return totalincome;

    ////////在循环里进行sql查询可能不太科学，所以这个函数我写了两种版本，下面这个是新的

    int totalincome = 0; // 初始化 totalincome

    // 查询所有订单的菜品数量和价格
    QString sql = "SELECT od.dish_number, m.dish_price "
                  "FROM order_data od "
                  "JOIN menu m ON od.iddish = m.idmenu "
                  "JOIN `order` o ON od.idorder = o.idorder "
                  "WHERE o.idrest = :idrest AND o.status IN (1,2,3)";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idrest", idrest);
    if (!sqlQuery->exec())
    {
        qDebug() << "查询失败:" << sqlQuery->lastError();
        return 0;
    }

    // 计算总收入
    while (sqlQuery->next())
    {
        int price = sqlQuery->value("dish_price").toInt();
        int num = sqlQuery->value("dish_number").toInt();
        totalincome += (price * num);
    }

    return totalincome;
}

int view_income::gettotalorder()
{
    QString sql = "SELECT idorder FROM `order` WHERE idrest=:idrest";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idrest",idrest);
    sqlQuery->exec();
    while(sqlQuery->next())
    {
        totalorder+=1;
    }
    return totalorder;
}
