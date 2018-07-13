#ifndef EXTRACT_H
#define EXTRACT_H

#include <QDialog>

namespace Ui {
class extract;
}

class extract : public QDialog
{
    Q_OBJECT

public:
    explicit extract(QWidget *parent = 0);
    ~extract();
signals:
    void getinfo();
private:
    Ui::extract *ui;
private slots:
    void eccc();
    void on_pushButton_clicked();
    void save_inf();
};

#endif // EXTRACT_H
