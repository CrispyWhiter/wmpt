#include "admin_ban.h"
#include "ui_admin_ban.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>

admin_ban::admin_ban(QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::admin_ban)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("账号封禁/解封");
    sqlQuery=s;

    // 设置表格列数
    ui->userTable->setColumnCount(3);
    ui->userTable->setHorizontalHeaderLabels({"用户ID", "用户名", "封禁状态"});

    // 连接按钮点击事件
    connect(ui->banButton, &QPushButton::clicked, this, &admin_ban::onBanButtonClicked);
    connect(ui->unbanButton, &QPushButton::clicked, this, &admin_ban::onUnbanButtonClicked);

    // 连接表格选择事件
    connect(ui->userTable, &QTableWidget::itemClicked, this, [this](QTableWidgetItem *item) {
        selectedUserId = ui->userTable->item(item->row(), 0)->text().toInt(); // 获取选中的用户 ID
    });

    // 加载用户列表
    loadUsers();
}

admin_ban::~admin_ban()
{
    delete ui;
}

// 加载用户列表
void admin_ban::loadUsers() {
    // 清空表格
    ui->userTable->setRowCount(0);

    // 查询所有用户
    QString sql = "SELECT iduser, name, is_ban FROM users WHERE type IN (0,1)";
    if (!sqlQuery->exec(sql)) {
        qDebug() << "加载用户列表失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "加载用户列表失败，请稍后重试");
        return;
    }

    // 遍历查询结果，添加到表格
    while (sqlQuery->next()) {
        int iduser = sqlQuery->value("iduser").toInt();
        QString name = sqlQuery->value("name").toString();
        int isBan = sqlQuery->value("is_ban").toInt();

        // 在表格中添加一行
        int row = ui->userTable->rowCount();
        ui->userTable->insertRow(row);

        // 设置用户 ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(iduser));
        ui->userTable->setItem(row, 0, idItem);

        // 设置用户名
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        ui->userTable->setItem(row, 1, nameItem);

        // 设置封禁状态
        QTableWidgetItem *banItem = new QTableWidgetItem(isBan == 1 ? "已封禁" : "正常");
        ui->userTable->setItem(row, 2, banItem);
    }
}

// 封禁账号
void admin_ban::onBanButtonClicked() {
    if(!(ui->ensure->text()=="I am sure what I am doing"))
    {
        QMessageBox::warning(this, "提示", "请输入确定文本");
        return;
    }

    if (selectedUserId == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个用户");
        return;
    }

    // 更新用户的封禁状态
    QString sql = "UPDATE users SET is_ban = 1 WHERE iduser = :iduser";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", selectedUserId);
    if (!sqlQuery->exec()) {
        qDebug() << "封禁账号失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "封禁账号失败，请稍后重试");
        return;
    }

    QMessageBox::information(this, "成功", "账号已封禁");
    loadUsers(); // 重新加载用户列表
}

// 解封账号
void admin_ban::onUnbanButtonClicked() {
    if(!(ui->ensure->text()=="I am sure what I am doing"))
    {
        QMessageBox::warning(this, "提示", "请输入确定文本");
        return;
    }

    if (selectedUserId == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个用户");
        return;
    }

    // 更新用户的封禁状态
    QString sql = "UPDATE users SET is_ban = 0 WHERE iduser = :iduser";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":iduser", selectedUserId);
    if (!sqlQuery->exec()) {
        qDebug() << "解封账号失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "解封账号失败，请稍后重试");
        return;
    }

    QMessageBox::information(this, "成功", "账号已解封");
    loadUsers(); // 重新加载用户列表
}
