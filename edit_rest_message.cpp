#include "edit_rest_message.h"
#include "ui_edit_rest_message.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

edit_rest_message::edit_rest_message(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent) ,ui(new Ui::edit_rest_message)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("修改餐厅信息");
    sqlQuery=s;
    iduser=id;
}

edit_rest_message::~edit_rest_message()
{
    delete ui;
}

void edit_rest_message::on_edit_clicked()
{
    if(ui->name->text().isEmpty())
    {
        QMessageBox::warning(this, tr("XwX"), tr("餐厅名字不得留空"));
        return;
    }

    if(ui->start->text()==ui->close->text())
    {
        QMessageBox::warning(this, tr("XwX"), tr("开始营业时间不应等于结束营业时间"));
        return;
    }

    QString editmessage = QString("UPDATE rests SET restname='%1',time_start='%2',time_close='%3' WHERE iduser='%4'").arg(ui->name->text().toUtf8()).arg(ui->start->text()).arg(ui->close->text()).arg(iduser);
    if(sqlQuery->exec(editmessage))
    {
        qDebug()<<"edit message success";
        QMessageBox::warning(this, tr(">w<"), tr("修改餐厅信息成功"));
    }
    else
    {
        qDebug()<<"edit message fail" <<sqlQuery->lastError();
        QMessageBox::warning(this, tr("XwX"), tr("修改餐厅信息失败"));
    }
}

