#include "superuser_main.h"
#include "ui_superuser_main.h"
#include "admin_ban.h"
#include "select_rest_2.h"
#include "admin_send_ad.h"
#include "admin_delete_comment.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

superuser_main::superuser_main(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::superuser_main)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("超级管理员主菜单");
    iduser=id;
    sqlQuery=s;
}

superuser_main::~superuser_main()
{
    delete ui;
}

void superuser_main::on_pushButton_clicked() // 封禁/解封账号功能
{
    admin_ban *w = new admin_ban(sqlQuery);
    w->show();
}


void superuser_main::on_pushButton_3_clicked() // 改菜单功能（套用顾客的选餐厅和商家的改菜单）
{
    select_rest_2 *w = new select_rest_2(iduser,sqlQuery);
    w->show();
}


void superuser_main::on_pushButton_4_clicked()
{
    admin_send_ad *w = new admin_send_ad(sqlQuery);
    w->show();
}


void superuser_main::on_pushButton_2_clicked()
{
    admin_delete_comment *w = new admin_delete_comment(sqlQuery);
    w->show();
}

