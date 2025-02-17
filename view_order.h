#ifndef VIEW_ORDER_H
#define VIEW_ORDER_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class view_order;
}

class view_order : public QWidget
{
    Q_OBJECT

public:
    explicit view_order(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~view_order();

private slots:
    void loadOrders();
    void onOrderSelected(QListWidgetItem *item);

    void on_query_clicked();

private:
    Ui::view_order *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // VIEW_ORDER_H
