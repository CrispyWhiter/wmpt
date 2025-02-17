#ifndef SELECT_DISH_2_H
#define SELECT_DISH_2_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class select_dish_2;
}

class select_dish_2 : public QWidget
{
    Q_OBJECT

public:
    explicit select_dish_2(int id_rest, int id_user, QSqlQuery *s, QWidget *parent = nullptr);
    ~select_dish_2();

private slots:
    void loadMenu(); // 加载菜单
    void loadOrder(); // 加载订单
    void updateTotalPrice(); // 更新总价格
    void onAddDishClicked(); // 点菜
    void onEditQuantityClicked(); // 修改数量
    void onDeleteDishClicked(); // 删除菜品
    void onOrderClicked(); // 提交订单

private:
    Ui::select_dish_2 *ui;
    QSqlQuery *sqlQuery; // 数据库查询对象
    int iduser; // 当前用户的 ID
    int idrest; // 当前餐厅的 ID
    int idorder; // 当前订单的 ID
    int clean_empty_order(); //清理空订单
};

#endif // SELECT_DISH_2_H
