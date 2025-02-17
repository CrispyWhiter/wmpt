#include "edit_password.h"
#include "ui_edit_password.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QIntValidator>

edit_password::edit_password(int id, QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::edit_password)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("修改密码");
    ui->password->setValidator(new QIntValidator(this));
    ui->repeat->setValidator(new QIntValidator(this));
    sqlQuery=s;
    iduser=id;
}

edit_password::~edit_password()
{
    delete ui;
}

void edit_password::on_cancel_clicked()
{
    this->close();
}


void edit_password::on_edit_clicked()
{
    if(!ui->password->text().isEmpty()&&!ui->repeat->text().isEmpty())
    {
        if(ui->password->text()==ui->repeat->text())
        {
            QString editpwd = QString("UPDATE users SET password='%1' WHERE iduser=%2").arg(ui->password->text()).arg(iduser);
            if(sqlQuery->exec(editpwd))
            {
                qDebug()<<"edit password success";
                QMessageBox::warning(this, tr(">w<"), tr("修改密码成功"));
            }
            else
            {
                qDebug()<<"edit password fail" <<sqlQuery->lastError();
                QMessageBox::warning(this, tr("XwX"), tr("修改密码失败"));
            }
        }
        else
        {
            QMessageBox::warning(this, tr("XwX"), tr("两次密码输入不一致"));
        }
    }
    else
    {
        QMessageBox::warning(this, tr("XwX"), tr("密码不得为空！"));
    }
}

