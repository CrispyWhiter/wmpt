#include "select_rest.h"
#include "ui_select_rest.h"
#include "select_dish.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QIntValidator>

// 已废弃

select_rest::select_rest(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::select_rest)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->idrest->setValidator(new QIntValidator(this));
    this->setWindowTitle("选择餐厅");
    iduser=id;
    sqlQuery=s;

    ui->restlist->clear();
    QString empty = "";
    QString empty32 = empty.leftJustified(32);
    QString empty24 = empty.leftJustified(24);
    ui->restlist->insertPlainText(("餐厅id"));
    ui->restlist->insertPlainText(empty24);
    ui->restlist->insertPlainText("餐厅名字\n");
    QString listrest = QString("select idrest,restname from rests");
    if(sqlQuery->exec(listrest))
    {
        while(sqlQuery->next())
        {
            ui->restlist->insertPlainText("\n");
            ui->restlist->insertPlainText(sqlQuery->value("idrest").toString());
            ui->restlist->insertPlainText(empty32);
            ui->restlist->insertPlainText(sqlQuery->value("restname").toString());
        }
        ui->restlist->insertPlainText("\n\n餐厅读取完毕");
    }
    else
    {
        qDebug()<<"sql2 fail";
    }
}

select_rest::~select_rest()
{
    delete ui;
}

void select_rest::on_select_clicked()
{
    if(!ui->idrest->text().isEmpty())
    {
        QString idrest = QString("select idrest from rests where idrest='%1'").arg(ui->idrest->text().toInt());
        sqlQuery->exec(idrest);
        if(sqlQuery->next())
        {
            qDebug()<<"restaurant selected";
            select_dish *w = new select_dish(ui->idrest->text().toInt(),iduser,sqlQuery);
            w->show();
        }
        else
        {
            qDebug()<<sqlQuery->lastError();
            QMessageBox::warning(this, tr("XwX"), tr("没有这个餐厅！"));
        }
    }
}

