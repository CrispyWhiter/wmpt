#include "select_rest_2.h"
#include "ui_select_rest_2.h"
#include "select_dish_2.h"
#include "edit_menu.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>

select_rest_2::select_rest_2(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::select_rest_2)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    iduser=id;
    sqlQuery=s;

    this->setWindowTitle("选择餐厅");

    //设置 QListWidget 的选择模式为单选
    ui->restlist->setSelectionMode(QAbstractItemView::SingleSelection);

    //连接 QListWidget 的点击事件
    connect(ui->restlist, &QListWidget::itemClicked, this, &select_rest_2::onRestSelected);

    //加载餐厅列表
    QString listrest = "SELECT idrest, restname FROM rests";
    if (sqlQuery->exec(listrest)) {
        while (sqlQuery->next()) {
            int idrest = sqlQuery->value("idrest").toInt();
            QString restname = sqlQuery->value("restname").toString();

            // 将餐厅信息添加到 QListWidget
            QListWidgetItem *item = new QListWidgetItem(
                QString("餐厅ID: %1, 餐厅名称: %2").arg(idrest).arg(restname), ui->restlist);
            item->setData(Qt::UserRole, idrest); // 将餐厅 ID 存储为 UserRole
        }
    } else {
        qDebug() << "加载餐厅列表失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "加载餐厅列表失败，请稍后重试。");
    }
}

select_rest_2::~select_rest_2()
{
    delete ui;
}

//餐厅选择事件
void select_rest_2::onRestSelected(QListWidgetItem *item) {
    //获取选中的餐厅 ID
    int idrest = item->data(Qt::UserRole).toInt();

    qDebug()<<idrest;
    //打开点餐界面

    QString sql = "SELECT type FROM users WHERE iduser=:iduser";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser",iduser);
    sqlQuery->exec();
    sqlQuery->next();
    int type = sqlQuery->value("type").toInt();
    if(type == 2)
    {
        //超级管理员修改菜单功能
        QString sql = "SELECT iduser FROM rests WHERE idrest=:idrest";
        sqlQuery->prepare(sql);
        sqlQuery->bindValue(":idrest",idrest);
        sqlQuery->exec();
        sqlQuery->next();
        int iduser_rest = sqlQuery->value("iduser").toInt();
        edit_menu *w = new edit_menu(iduser_rest,sqlQuery);
        w->show();
    }
    else
    {
        //普通用户选餐功能
        select_dish_2 *w = new select_dish_2(idrest, iduser, sqlQuery);
        w->show();
        this->close();
    }
}

//搜索功能
void select_rest_2::on_query_clicked()
{
    ui->restlist->clear();
    QString listrest = QString("SELECT idrest, restname FROM rests WHERE restname='%1'").arg(ui->name->text().toUtf8());
    if (sqlQuery->exec(listrest)) {
        while (sqlQuery->next()) {
            int idrest = sqlQuery->value("idrest").toInt();
            QString restname = sqlQuery->value("restname").toString();

            QListWidgetItem *item = new QListWidgetItem(
                QString("餐厅ID: %1, 餐厅名称: %2").arg(idrest).arg(restname), ui->restlist);
            item->setData(Qt::UserRole, idrest);
        }
    } else {
        qDebug() << "加载餐厅列表失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "加载餐厅列表失败，请稍后重试。");
    }
}



