#ifndef FILEWINDOW_H
#define FILEWINDOW_H

#include <QDialog>
#include<QWidget>
#include<QPushButton>
#include<QVBoxLayout>
#include<QListWidget>
#include<QListWidgetItem>
//QString temp_write;


namespace Ui {
class Filewindow;
}

class Filewindow : public QDialog
{
    Q_OBJECT

public:
    explicit Filewindow(QWidget *parent = 0);
    ~Filewindow();

private:
    Ui::Filewindow *ui;
    QWidget *qw;
    QPushButton *qp;
    QVBoxLayout *qb;
signals:
    void showfile(QString);
    void fire();
private slots:
    void receivelogin();
    void contextMenuEvent(QContextMenuEvent *event);
    void add();
    void adddir();
    void allocate();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void allot(int length);
    void callback(int length);
    void on_delf_clicked();
    void del_file(char filename[]);
    void singleclicked(QListWidgetItem *item);
    void doubleclicked(QListWidgetItem *item);
    void refresh_pro();
    void sava();
    void on_refresh_clicked();
    void on_retur_clicked();
    void refresh_pri();
};

#endif // FILEWINDOW_H
