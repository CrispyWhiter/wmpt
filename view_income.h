#ifndef VIEW_INCOME_H
#define VIEW_INCOME_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

namespace Ui {
class view_income;
}

class view_income : public QWidget
{
    Q_OBJECT

public:
    explicit view_income(int id_rest,int id_user,QSqlQuery *s,QWidget *parent = nullptr);
    ~view_income();

private slots:
    int gettotalincome();  //返回总收入
    int gettotalorder();  //返回订单总数

private:
    Ui::view_income *ui;
    int idrest;
    int iduser;
    QSqlQuery *sqlQuery;
    int totalincome;
    int totalorder;
};

#endif // VIEW_INCOME_H
