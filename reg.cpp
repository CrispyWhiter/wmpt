#include "reg.h"
#include "ui_reg.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QIntValidator>

reg::reg(QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::reg)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("注册");
    sqlQuery = s;
    ui->password->setValidator(new QIntValidator(ui->password));
    ui->password->setPlaceholderText("Only number");
    ui->password->setEchoMode(QLineEdit::Password);
}

reg::~reg()
{
    delete ui;
}

void reg::on_cancel_clicked()
{
    this->close();
}

void reg::on_reg_2_clicked()
{
    QString name = ui->name->text().toUtf8();
    QString password = ui->password->text().toUtf8();
    if(password.isEmpty())
    {
        QMessageBox::warning(this, tr("XwX"), tr("密码不得为空！"));
        return;
    }
    QString type = QString::number(ui->type->isChecked());

    QString reg=QString("insert into users(name,password,type) values('%1','%2','%3');").arg(name).arg(password).arg(type);
    if(sqlQuery->exec(reg))
    {
        qDebug()<<"reg success";
        QMessageBox::warning(this, tr(">w<"), tr("注册成功"));
    }
    else
    {
        qDebug()<<"reg fail";
    }

    if(ui->type->isChecked())
    {
        QString id = QString("SELECT LAST_INSERT_ID() AS iduser");
        sqlQuery->exec(id);
        sqlQuery->next();
        int iduser = (sqlQuery->value("iduser")).toInt();
        QString regrest=QString("insert into rests(iduser) values('%1');").arg(iduser);
        if(sqlQuery->exec(regrest))
        {
            qDebug()<<"add rest success";
        }
        else
        {
            qDebug()<<"add rest fail";
        }
    }
}

