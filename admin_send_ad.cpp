#include "admin_send_ad.h"
#include "ui_admin_send_ad.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>


admin_send_ad::admin_send_ad(QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::admin_send_ad)
{
    ui->setupUi(this);
    sqlQuery=s;

    this->setWindowTitle("广告管理");

    // 设置表格列数
    ui->adTable->setColumnCount(2);
    ui->adTable->setHorizontalHeaderLabels({"广告ID", "广告内容"});

    // 连接按钮点击事件
    connect(ui->addButton, &QPushButton::clicked, this, &admin_send_ad::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &admin_send_ad::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &admin_send_ad::onDeleteButtonClicked);

    // 连接表格选择事件
    connect(ui->adTable, &QTableWidget::itemClicked, this, &admin_send_ad::onAdSelected);

    // 加载广告列表
    loadAds();
}

admin_send_ad::~admin_send_ad()
{
    delete ui;
}

// 加载广告列表
void admin_send_ad::loadAds() {
    // 清空表格
    ui->adTable->setRowCount(0);

    // 查询所有广告
    QString sql = "SELECT idad, adtext FROM ad";
    if (!sqlQuery->exec(sql)) {
        qDebug() << "加载广告列表失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "加载广告列表失败，请稍后重试。");
        return;
    }

    // 遍历查询结果，添加到表格
    while (sqlQuery->next()) {
        int idad = sqlQuery->value("idad").toInt();
        QString adtext = sqlQuery->value("adtext").toString();

        // 在表格中添加一行
        int row = ui->adTable->rowCount();
        ui->adTable->insertRow(row);

        // 设置广告 ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(idad));
        ui->adTable->setItem(row, 0, idItem);

        // 设置广告内容
        QTableWidgetItem *textItem = new QTableWidgetItem(adtext);
        ui->adTable->setItem(row, 1, textItem);
    }
}

// 选择广告
void admin_send_ad::onAdSelected(QTableWidgetItem *item) {
    // 获取选中的广告 ID
    selectedAdId = ui->adTable->item(item->row(), 0)->text().toInt();

    // 将广告内容显示在输入框中
    ui->adTextEdit->setText(ui->adTable->item(item->row(), 1)->text());
}

// 添加广告
void admin_send_ad::onAddButtonClicked() {
    QString adtext = ui->adTextEdit->toPlainText().trimmed();
    if (adtext.isEmpty()) {
        QMessageBox::warning(this, "提示", "广告内容不能为空。");
        return;
    }

    // 插入新广告
    QString sql = "INSERT INTO ad (adtext) VALUES (:adtext)";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":adtext", adtext);
    if (!sqlQuery->exec()) {
        qDebug() << "添加广告失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "添加广告失败，请稍后重试。");
        return;
    }

    QMessageBox::information(this, "成功", "广告已添加。");
    loadAds(); // 重新加载广告列表
    ui->adTextEdit->clear(); // 清空输入框
}

// 编辑广告
void admin_send_ad::onEditButtonClicked() {
    if (selectedAdId == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个广告。");
        return;
    }

    QString adtext = ui->adTextEdit->toPlainText().trimmed();
    if (adtext.isEmpty()) {
        QMessageBox::warning(this, "提示", "广告内容不能为空。");
        return;
    }

    // 更新广告内容
    QString sql = "UPDATE ad SET adtext = :adtext WHERE idad = :idad";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":adtext", adtext);
    sqlQuery->bindValue(":idad", selectedAdId);
    if (!sqlQuery->exec()) {
        qDebug() << "编辑广告失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "编辑广告失败，请稍后重试。");
        return;
    }

    QMessageBox::information(this, "成功", "广告已更新。");
    loadAds(); // 重新加载广告列表
    ui->adTextEdit->clear(); // 清空输入框
    selectedAdId = -1; // 重置选中的广告 ID
}

// 删除广告
void admin_send_ad::onDeleteButtonClicked() {
    if (selectedAdId == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个广告。");
        return;
    }

    // 删除广告
    QString sql = "DELETE FROM ad WHERE idad = :idad";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idad", selectedAdId);
    if (!sqlQuery->exec()) {
        qDebug() << "删除广告失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "删除广告失败，请稍后重试。");
        return;
    }

    QMessageBox::information(this, "成功", "广告已删除。");
    loadAds(); // 重新加载广告列表
    ui->adTextEdit->clear(); // 清空输入框
    selectedAdId = -1; // 重置选中的广告 ID
}
