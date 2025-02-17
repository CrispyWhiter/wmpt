#ifndef EDIT_REST_MESSAGE_H
#define EDIT_REST_MESSAGE_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

namespace Ui {
class edit_rest_message;
}

class edit_rest_message : public QWidget
{
    Q_OBJECT

public:
    explicit edit_rest_message(int id,QSqlQuery *s,QWidget *parent = nullptr);
    ~edit_rest_message();

private slots:
    void on_edit_clicked();

private:
    Ui::edit_rest_message *ui;
    int iduser;
    QSqlQuery *sqlQuery;
};

#endif // EDIT_REST_MESSAGE_H
