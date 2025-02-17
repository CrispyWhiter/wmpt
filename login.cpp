#include "login.h"
#include "ui_login.h"
#include "reg.h"
#include "customer_main.h"
#include "restaurant_main.h"
#include "superuser_main.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QIntValidator>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_DeleteOnClose);

    ui->Password->setValidator(new QIntValidator(ui->Password));
    ui->Password->setPlaceholderText("仅数字");
    ui->Password->setEchoMode(QLineEdit::Password);
    this->setWindowTitle("登陆");

    wmptdb=QSqlDatabase::addDatabase("QMYSQL"); //建立数据库连接
    wmptdb.setHostName("127.0.0.1");
    wmptdb.setPort(3306);
    wmptdb.setUserName("root");
    wmptdb.setPassword("123456");
    wmptdb.setDatabaseName("wmpt");
    if(wmptdb.open()){
        qDebug()<<"connected mysql"; //只有一个数据库时，需要把sqlQuery类与数据库进行绑定
        sqlQuery = new QSqlQuery("wmpt"); //新窗口操作数据库需传参sqlQuery

    }else{
        qDebug()<<"connect mysql fail"<<wmptdb.lastError().text();
    }

}

login::~login()
{
    delete ui;
}

void login::on_Quit_clicked()
{
    this->close();
}


void login::on_Reg_clicked()
{
    reg *w = new reg(sqlQuery);
    w->show();
}


void login::on_Login_clicked()
{
    QString login = QString("select iduser,name,password,type,is_ban from users"); //验证账号密码
    if(sqlQuery->exec(login))
    {
        qDebug()<<"login query step 1 success";
        int userquery = 0;    //验证状态
        int typequery = 0;    //用户类型
        int iduserquery = 0;    //用户id
        int is_ban = 0;     //字面意思
        while(sqlQuery->next())
        {
            QString name = (sqlQuery->value("name")).toString();
            QString password = (sqlQuery->value("password").toString());
            int type = (sqlQuery->value("type")).toInt();
            int iduser = (sqlQuery->value("iduser")).toInt();
            int isban = (sqlQuery->value("is_ban")).toInt();
            qDebug()<<name<<" "<<password;
            if((name==ui->User->text())&(password==ui->Password->text()))
            {
                userquery=1;
                typequery=type;
                iduserquery=iduser;
                is_ban=isban;
            }

        }
        qDebug()<<userquery;
        qDebug()<<typequery;
        qDebug()<<iduserquery;

        if(userquery)
        {
            if(is_ban)
            {
                QMessageBox::warning(this, tr("账号异常"), tr("你的账号貌似已被封禁，请联系平台管理员"));
                return;
            }
            switch(typequery)
            {
                case 0: //客户界面入口
                    {
                        qDebug()<<"customer login";
                        customer_main *w = new customer_main(iduserquery,sqlQuery);
                        w->show();
                        break;
                    }

                case 1: //商家界面入口
                    {
                        qDebug()<<"rests login";
                        restaurant_main *w = new restaurant_main(iduserquery,sqlQuery);
                        w->show();
                        break;
                    }
                case 2: //超级管理员入口
                    {
                        qDebug()<<"root login";
                        superuser_main *w = new superuser_main(iduserquery,sqlQuery);
                        w->show();
                        break;
                    }
            }
        }
        else
        {
            QMessageBox::warning(this, tr("异常"), tr("账号或密码错误"));
        }
    }
    else
    {
        qDebug()<<"login query step 1 fail";
    }
}

