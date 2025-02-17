#ifndef REG_H
#define REG_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class reg;
}

class reg : public QWidget
{
    Q_OBJECT

public:
    explicit reg(QSqlQuery *s,QWidget *parent = nullptr);
    ~reg();

private slots:
    void on_cancel_clicked();

    void on_reg_2_clicked();

private:
    Ui::reg *ui;
    QSqlQuery *sqlQuery;
};

#endif // REG_H
