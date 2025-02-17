#ifndef EDIT_MENU_H
#define EDIT_MENU_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

namespace Ui {
class edit_menu;
}

class edit_menu : public QWidget
{
    Q_OBJECT

public:
    explicit edit_menu(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~edit_menu();

signals:
    void updatashow();

private slots:
    void listdish();

    void on_new_2_clicked();

    void on_edit_clicked();

    void on_delete_2_clicked();

private:
    Ui::edit_menu *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // EDIT_MENU_H
