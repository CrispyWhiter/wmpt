#include "edit_menu.h"
#include "ui_edit_menu.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QIntValidator>

edit_menu::edit_menu(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::edit_menu)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("修改菜单");
    ui->idmenu->setValidator(new QIntValidator(this));
    ui->dish_price->setValidator(new QIntValidator(this));
    ui->idmenu->setPlaceholderText("新增无需填写");
    iduser=id;
    sqlQuery=s;

    connect(this,SIGNAL(updatashow()),this,SLOT(listdish()));

    emit updatashow();
}

edit_menu::~edit_menu()
{
    delete ui;
}

void edit_menu::listdish()
{
    ui->showmenu->clear();
    QString empty = "";
    QString empty32 = empty.leftJustified(32);
    QString empty24 = empty.leftJustified(24);
    ui->showmenu->insertPlainText(("菜品id"));
    ui->showmenu->insertPlainText(empty24);
    ui->showmenu->insertPlainText("菜品名字");
    ui->showmenu->insertPlainText(empty24);
    ui->showmenu->insertPlainText("菜品价格\n");
    QString sql1 = QString("select idrest from rests where iduser=%1").arg(iduser);
    if(sqlQuery->exec(sql1))
    {
        sqlQuery->next();
        QString sql2 = QString("select idmenu,dish_name,dish_price from menu where idrest=%1").arg(sqlQuery->value("idrest").toInt());
        if(sqlQuery->exec(sql2))
        {
            while(sqlQuery->next())
            {
                ui->showmenu->insertPlainText("\n");
                ui->showmenu->insertPlainText(sqlQuery->value("idmenu").toString());
                ui->showmenu->insertPlainText(empty32);
                ui->showmenu->insertPlainText(sqlQuery->value("dish_name").toString());
                ui->showmenu->insertPlainText(empty32);
                ui->showmenu->insertPlainText(sqlQuery->value("dish_price").toString());
            }
            ui->showmenu->insertPlainText("\n\n菜单读取完毕");
        }
        else
        {
            qDebug()<<"sql2 fail";
        }
    }
    else
    {
        qDebug()<<"sql1 fail";
    }
}

void edit_menu::on_new_2_clicked()
{
    QString sql1 = QString("select idrest from rests where iduser=%1").arg(iduser);
    if(sqlQuery->exec(sql1))
    {
        sqlQuery->next();
        if(!ui->dish_name->text().isEmpty()&&!ui->dish_price->text().isEmpty())
        {
            QString newdish = QString("insert into menu(idrest,dish_name,dish_price) values('%1','%2','%3');").arg(sqlQuery->value("idrest").toInt()).arg(ui->dish_name->text().toUtf8()).arg(ui->dish_price->text().toInt());
            if(sqlQuery->exec(newdish))
            {
                emit updatashow();
            }
            else
            {
                qDebug()<<sqlQuery->lastError();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("添加失败"), tr("请输入菜品名字和价格（均不得留空）"));
        }
    }
    else
    {
        qDebug()<<"sql1 fail";
    }
}


void edit_menu::on_edit_clicked()
{
    QString sql1 = QString("select idrest from rests where iduser=%1").arg(iduser);
    if(sqlQuery->exec(sql1))
    {
        sqlQuery->next();
        if(!ui->idmenu->text().isEmpty()&&!ui->dish_name->text().isEmpty()&&!ui->dish_price->text().isEmpty())
        {
            QString editdish = QString("update menu set dish_name='%1',dish_price='%2' where idrest ='%3' and idmenu='%4'").arg(ui->dish_name->text().toUtf8()).arg(ui->dish_price->text().toInt()).arg(sqlQuery->value("idrest").toInt()).arg(ui->idmenu->text().toInt());
            if(sqlQuery->exec(editdish))
            {
                emit updatashow();
            }
            else
            {
                qDebug()<<sqlQuery->lastError();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("修改失败"), tr("请输入菜品id,菜品名字和价格（均不得留空）"));
        }
    }
    else
    {
        qDebug()<<"sql1 fail";
    }
}


void edit_menu::on_delete_2_clicked()
{
    QString sql1 = QString("select idrest from rests where iduser=%1").arg(iduser);
    if(sqlQuery->exec(sql1))
    {
        sqlQuery->next();
        if(!ui->idmenu->text().isEmpty())
        {
            QString deletedish = QString("delete from menu where idrest ='%1' and idmenu='%2'").arg(sqlQuery->value("idrest").toInt()).arg(ui->idmenu->text().toInt());
            if(sqlQuery->exec(deletedish))
            {
                emit updatashow();
            }
            else
            {
                qDebug()<<sqlQuery->lastError();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("删除失败"), tr("请输入菜品id"));
        }
    }
    else
    {
        qDebug()<<"sql1 fail";
    }
}

