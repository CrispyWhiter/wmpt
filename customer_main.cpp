#include "customer_main.h"
#include "ui_customer_main.h"
#include "edit_password.h"
#include "select_rest_2.h"
#include "paymentwindow.h"
#include "commentwindow.h"
#include "view_order.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QIntValidator>

customer_main::customer_main(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::customer_main)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("用户主菜单");
    iduser=id;
    sqlQuery=s;
    qDebug()<<iduser;

    updatashow();

    //广告定时器
    connect(&ad, &QTimer::timeout, this, &customer_main::load_ad);
    ad.start(1000);
}

customer_main::~customer_main()
{
    delete ui;
}

void customer_main::load_ad()
{
    if(test==1)         //测试专用
    {
        qDebug()<<"4 用户定时器";
        test = 0;
    }
    else
    {
        qDebug()<<"5";
        test = 1;
    }

    QString new_ad=QString("SELECT adtext FROM ad");
    if(!sqlQuery->exec(new_ad))
    {
        qDebug()<<sqlQuery->lastError();
    }

    int clear = 0;
    int isnew = 0;

    while(sqlQuery->next())
    {
        if(clear==0)
        {
            ui->message->clear();
            clear = 1;
            isnew = 1;
            ui->message->insertPlainText("嘀嘀嘀，广告来咯！\n");
        }
        ui->message->insertPlainText(sqlQuery->value("adtext").toString());
        ui->message->insertPlainText("\n\n");
    }

    if(!isnew)
    {
        updatashow();
    }
}

void customer_main::updatashow()
{
    ui->message->clear();
    ui->message->insertPlainText("欢迎使用外卖管理系统\n你的登陆身份为:  普通用户\n");

    QString sql = QString("select name from users where iduser='%1'").arg(iduser);
    if(sqlQuery->exec(sql))
    {
        sqlQuery->next();
        ui->message->insertPlainText("你的用户名:  ");
        ui->message->insertPlainText(sqlQuery->value("name").toString());
        ui->message->insertPlainText("\n");
    }
    else
    {
        ui->message->insertPlainText("这个bug很罕见，找不到用户名");
    }
}

void customer_main::on_edipwd_clicked()
{
    edit_password *w = new edit_password(iduser,sqlQuery);
    w->show();
}


void customer_main::on_selrest_clicked()
{
    select_rest_2 *w = new select_rest_2(iduser,sqlQuery);
    w->show();
}


void customer_main::on_queord_clicked()
{
    // 清空 message 区域
    ui->message->clear();

    // 查询未完成订单（状态为 0 或 1）
    QString sql = "SELECT o.idorder, o.idrest, o.status, r.restname "
                  "FROM `order` o "
                  "JOIN rests r ON o.idrest = r.idrest "
                  "WHERE o.iduser = :iduser AND o.status IN (0, 1)";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", iduser);
    if (!sqlQuery->exec())
    {
        qDebug() << "查询未完成订单失败:" << sqlQuery->lastError();
        ui->message->setText("查询失败，请稍后重试。");
        return;
    }

    // 如果没有未完成订单
    if (!sqlQuery->next())
    {
        ui->message->setText("您没有未完成的订单。");
        return;
    }

    // 显示未完成订单
    QString result = "您的未完成订单：\n\n";
    do
    {
        int orderId = sqlQuery->value("idorder").toInt();
        QString restName = sqlQuery->value("restname").toString();
        int status = sqlQuery->value("status").toInt();

        // 添加订单信息到结果字符串
        result += QString("订单号: %1    餐厅: %2    状态: %3\n")
                      .arg(orderId)
                      .arg(restName)
                      .arg(status == 0 ? "未付款" : "已付款" );
    }
    while (sqlQuery->next());

    // 将结果显示在 message 区域
    ui->message->setText(result);
}


void customer_main::on_pushButton_clicked()
{
    paymentwindow *w = new paymentwindow(iduser,sqlQuery);
    w->show();
}


void customer_main::on_comord_clicked()
{
    commentwindow *w = new commentwindow(iduser,sqlQuery);
    w->show();
}


void customer_main::on_vieord_clicked()
{
    view_order *w = new view_order(iduser,sqlQuery);
    w->show();
}

