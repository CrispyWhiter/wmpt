#ifndef SELECT_REST_2_H
#define SELECT_REST_2_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class select_rest_2;
}

class select_rest_2 : public QWidget
{
    Q_OBJECT

public:
    explicit select_rest_2(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~select_rest_2();

private slots:
    void onRestSelected(QListWidgetItem *item); // 餐厅选择事件

    void on_query_clicked();

private:
    Ui::select_rest_2 *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // SELECT_REST_2_H
