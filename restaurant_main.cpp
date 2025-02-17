#include "restaurant_main.h"
#include "ui_restaurant_main.h"
#include "edit_password.h"
#include "edit_rest_message.h"
#include "edit_menu.h"
#include "view_proc_order.h"
#include "view_income.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QIntValidator>
#include <QTime>
#include <QTimer>

restaurant_main::restaurant_main(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restaurant_main)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("商家主界面");
    sqlQuery=s;
    iduser=id;
    idrest = 0;
    isopen_this = 0;
    isopen_window = 0;
    qDebug()<<iduser;

    QString restid=QString("SELECT idrest FROM rests WHERE iduser=%1").arg(iduser);    //查找账号对应的餐厅id
    sqlQuery->exec(restid);
    if(sqlQuery->next())
    {
        idrest = sqlQuery->value("idrest").toInt();
    }
    else
    {
        ui->message->insertPlainText("XwX 这个bug很罕见，找不到餐厅id\n");
    }

    connect(this, SIGNAL(update_show()), this, SLOT(updateshow()));
    emit update_show();

    //定时器循环检测新订单
    connect(&timer_notice_new_order, &QTimer::timeout, this, &restaurant_main::notice_new_order);
    timer_notice_new_order.start(1000);
}

restaurant_main::~restaurant_main()
{
    delete ui;
}

void restaurant_main::updateshow()
{
    ui->message->clear();

    ui->message->insertPlainText("欢迎使用外卖管理系统\n你的登陆身份为:  商家\n");

    QString restname=QString("SELECT idrest,restname FROM rests WHERE iduser=%1").arg(iduser);    //展示账号对应的餐厅的信息
    sqlQuery->exec(restname);
    if(sqlQuery->next()&&!sqlQuery->value("restname").isNull())
    {
        ui->message->insertPlainText("你的用户名 :  ");
        ui->message->insertPlainText(QString::number(iduser));
        ui->message->insertPlainText("\n");
        ui->message->insertPlainText("我的餐厅ID :  ");
        ui->message->insertPlainText((sqlQuery->value("idrest")).toString());
        ui->message->insertPlainText("\n");
        ui->message->insertPlainText("我的餐厅 :  ");
        ui->message->insertPlainText((sqlQuery->value("restname")).toString());
        ui->message->insertPlainText("\n");
    }
    else
    {
        ui->message->insertPlainText("XwX 你似乎还没有设置餐厅名字\n");
    }

    QString resttime=QString("SELECT time_start,time_close FROM rests WHERE iduser=%1").arg(iduser);    //展示账号对应的餐厅的信息
    sqlQuery->exec(resttime);
    if(sqlQuery->next()&&!sqlQuery->value("time_start").isNull()&&!sqlQuery->value("time_close").isNull())
    {
        QTime start = QTime::fromString((sqlQuery->value("time_start")).toString(), "HH:mm:ss");
        QTime close = QTime::fromString((sqlQuery->value("time_close")).toString(), "HH:mm:ss");

        ui->message->insertPlainText("营业开始时间 :  ");
        ui->message->insertPlainText(start.toString("HH:mm"));
        ui->message->insertPlainText("\n");
        ui->message->insertPlainText("营业结束时间 :  ");
        ui->message->insertPlainText(close.toString("HH:mm"));
        ui->message->insertPlainText("\n");
    }
    else
    {
        ui->message->insertPlainText("XwX 你似乎还没有设置餐厅营业时间\n");
    }
}

void restaurant_main::notice_new_order()
{
    if(test==1)         //测试专用
    {
        qDebug()<<"1 商家定时器";
        test = 0;
    }
    else
    {
        qDebug()<<"0";
        test = 1;
    }
    qDebug()<<"isopen_window: "<<isopen_window;
    qDebug()<<"isopen_this: "<<isopen_this;

    QString new_order=QString("SELECT idorder FROM `order` WHERE status=1 AND idrest='%1'").arg(idrest);
    if(!sqlQuery->exec(new_order))
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
        }
        ui->message->insertPlainText("嘀嘀嘀，您有新的已付款订单！\n");
        ui->message->insertPlainText("订单ID：");
        ui->message->insertPlainText(sqlQuery->value("idorder").toString());
        ui->message->insertPlainText("\n");
        ui->message->insertPlainText("请及时移步至 “查看/处理订单” 以处理您的订单\n\n");
    }

    if(!isnew)
    {
        emit update_show();
        isopen_this = 0;
    }
    else
    {
        if(!isopen_this&&!isopen_window)
        {
            isopen_this = 1;
            isopen_window = 1;
            sqlQuery->clear();
            view_proc_order_w = new view_proc_order(idrest,iduser,sqlQuery);
            connect(view_proc_order_w, &QObject::destroyed, this, &restaurant_main::edit_menu_w_close);
            view_proc_order_w->show();
        }
    }
}

void restaurant_main::edit_menu_w_close()
{
    qDebug() << "11111";

    disconnect(view_proc_order_w, &QObject::destroyed, this, &restaurant_main::edit_menu_w_close);
    //delete view_proc_order_w;
    //view_proc_order_w = nullptr;
    isopen_this = 0;
    isopen_window = 0;
}

void restaurant_main::on_editpassword_clicked()
{
    edit_password *w = new edit_password(iduser,sqlQuery);
    w->show();
}


void restaurant_main::on_editmessage_clicked()
{
    edit_rest_message *w = new edit_rest_message(iduser,sqlQuery);
    w->show();
}


void restaurant_main::on_editmenu_clicked()
{
    edit_menu *w = new edit_menu(iduser,sqlQuery);
    w->show();
}


void restaurant_main::on_pushButton_clicked()
{
    view_proc_order_w = new view_proc_order(idrest,iduser,sqlQuery);
    connect(view_proc_order_w, &QObject::destroyed, this, &restaurant_main::edit_menu_w_close);
    view_proc_order_w->show();
}


void restaurant_main::on_viewincome_clicked()
{
    view_income *w = new view_income(idrest,iduser,sqlQuery);
    w->show();
}

