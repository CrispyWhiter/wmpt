#ifndef ADMIN_BAN_H
#define ADMIN_BAN_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class admin_ban;
}

class admin_ban : public QWidget
{
    Q_OBJECT

public:
    explicit admin_ban(QSqlQuery *s,QWidget *parent = nullptr);
    ~admin_ban();

private slots:
    void loadUsers();
    void onBanButtonClicked(); // 封禁账号
    void onUnbanButtonClicked(); // 解封账号

private:
    Ui::admin_ban *ui;
    QSqlQuery *sqlQuery;
    int selectedUserId;
};

#endif // ADMIN_BAN_H
