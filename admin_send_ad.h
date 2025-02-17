#ifndef ADMIN_SEND_AD_H
#define ADMIN_SEND_AD_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>


namespace Ui {
class admin_send_ad;
}

class admin_send_ad : public QWidget
{
    Q_OBJECT

private slots:
    void loadAds(); // 加载广告列表
    void onAddButtonClicked(); // 添加广告
    void onEditButtonClicked(); // 编辑广告
    void onDeleteButtonClicked(); // 删除广告
    void onAdSelected(QTableWidgetItem *item); // 选择广告

public:
    explicit admin_send_ad(QSqlQuery *s,QWidget *parent = nullptr);
    ~admin_send_ad();

private:
    Ui::admin_send_ad *ui;
    QSqlQuery *sqlQuery;
    int selectedAdId;
};

#endif // ADMIN_SEND_AD_H
