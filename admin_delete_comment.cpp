#include "admin_delete_comment.h"
#include "ui_admin_delete_comment.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QTimer>

admin_delete_comment::admin_delete_comment(QSqlQuery *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::admin_delete_comment)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("评论删除");
    sqlQuery=s;
    num_comment = 0;

    ui->commentTable->setColumnCount(4);
    ui->commentTable->setHorizontalHeaderLabels({"评论ID", "订单ID", "用户ID", "评论内容"});

    connect(ui->deleteButton, &QPushButton::clicked, this, &admin_delete_comment::onDeleteButtonClicked);

    connect(ui->commentTable, &QTableWidget::itemClicked, this, [this](QTableWidgetItem *item) {
        selectedCommentId = ui->commentTable->item(item->row(), 0)->text().toInt();   // 获取选中的评论 ID
        ui->statusLabel->setText("已选择评论ID：");
        ui->statusLabel->setText(QString::number(selectedCommentId));
    });

    loadComments();

    QTimer *updatacomment = new QTimer(this);
    connect(updatacomment, &QTimer::timeout, this, &admin_delete_comment::loadComments);
    updatacomment->start(1000);
}

admin_delete_comment::~admin_delete_comment()
{
    delete ui;
}

// 加载评论列表
void admin_delete_comment::loadComments()
{
    if(test==1)         //测试专用
    {
        qDebug()<<"6 订单处理界面定时器";
        test = 0;
    }
    else
    {
        qDebug()<<"7";
        test = 1;
    }

    int comment_num_this = 0;
    int max_idorder_commit_this = 0;
    QString sql = "SELECT idorder_commit FROM order_commit";
    if(!sqlQuery->exec(sql))
    {
        qDebug() << "加载评论数量失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "加载评论数量失败，请稍后重试。");
        return;
    }
    while(sqlQuery->next())
    {
        comment_num_this+=1;
        max_idorder_commit_this=sqlQuery->value("idorder_commit").toInt();
    }
    if(comment_num_this==num_comment&&max_idorder_commit_this==max_idorder_commit_last)
    {
        qDebug() << "没有新的评论";
        return;
    }
    else
    {
        num_comment=comment_num_this;
        max_idorder_commit_last=max_idorder_commit_this;
        qDebug()<<"A";
    }

    ui->commentTable->setRowCount(0);

    // 查询所有评论
    QString sql1 = "SELECT idorder_commit, idorder, iduser, commit FROM order_commit";
    if (!sqlQuery->exec(sql1))
    {
        qDebug() << "加载评论列表失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "加载评论列表失败，请稍后重试。");
        return;
    }

    // 遍历查询结果，添加到表格
    while (sqlQuery->next())
    {
        int idorder_commit = sqlQuery->value("idorder_commit").toInt();
        int idorder = sqlQuery->value("idorder").toInt();
        int iduser = sqlQuery->value("iduser").toInt();
        QString commit = sqlQuery->value("commit").toString();

        // 在表格中添加一行
        int row = ui->commentTable->rowCount();
        ui->commentTable->insertRow(row);

        // 设置评论 ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(idorder_commit));
        ui->commentTable->setItem(row, 0, idItem);

        // 设置订单 ID
        QTableWidgetItem *orderItem = new QTableWidgetItem(QString::number(idorder));
        ui->commentTable->setItem(row, 1, orderItem);

        // 设置用户 ID
        QTableWidgetItem *userItem = new QTableWidgetItem(QString::number(iduser));
        ui->commentTable->setItem(row, 2, userItem);

        // 设置评论内容
        QTableWidgetItem *commitItem = new QTableWidgetItem(commit);
        ui->commentTable->setItem(row, 3, commitItem);
    }
}

// 删除评论
void admin_delete_comment::onDeleteButtonClicked()
{
    if (selectedCommentId == -1)
    {
        QMessageBox::warning(this, "提示", "请先选择一个评论。");
        return;
    }

    // 删除评论
    QString sql = "DELETE FROM order_commit WHERE idorder_commit = :idorder_commit";
    sqlQuery->prepare(sql);
    sqlQuery->bindValue(":idorder_commit", selectedCommentId);
    if (!sqlQuery->exec())
    {
        qDebug() << "删除评论失败:" << sqlQuery->lastError();
        QMessageBox::warning(this, "错误", "删除评论失败，请稍后重试。");
        return;
    }

    QMessageBox::information(this, "成功", "评论已删除。");
    loadComments();
    selectedCommentId = -1;
}
