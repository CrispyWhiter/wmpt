#ifndef ADMIN_DELETE_COMMENT_H
#define ADMIN_DELETE_COMMENT_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class admin_delete_comment;
}

class admin_delete_comment : public QWidget
{
    Q_OBJECT

public:
    explicit admin_delete_comment(QSqlQuery *s,QWidget *parent = nullptr);
    ~admin_delete_comment();

private slots:
    void loadComments();
    void onDeleteButtonClicked();

private:
    Ui::admin_delete_comment *ui;
    QSqlQuery *sqlQuery;
    int selectedCommentId;
    int num_comment;
    int max_idorder_commit_last;
    int test;
};

#endif // ADMIN_DELETE_COMMENT_H
