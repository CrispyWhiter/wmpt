#ifndef COMMENTWINDOW_H
#define COMMENTWINDOW_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class commentwindow;
}

class commentwindow : public QWidget
{
    Q_OBJECT

public:
    explicit commentwindow(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~commentwindow();

private slots:
    void loadOrders(); // 加载历史订单
    void onOrderSelected(QListWidgetItem *item); // 选择订单
    void onSubmitComment(); // 提交评论

private:
    Ui::commentwindow *ui;
    int iduser;
    int selectedOrderId;
    QSqlQuery *sqlQuery;
};

#endif // COMMENTWINDOW_H
