#ifndef SELECT_REST_H
#define SELECT_REST_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class select_rest;
}

class select_rest : public QWidget
{
    Q_OBJECT

public:
    explicit select_rest(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~select_rest();

private slots:
    void on_select_clicked();

private:
    Ui::select_rest *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // SELECT_REST_H
