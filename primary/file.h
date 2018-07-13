#ifndef FILE_H
#define FILE_H

#include <QDialog>

namespace Ui {
class file;
}

class file : public QDialog
{
    Q_OBJECT

public:
    explicit file(QWidget *parent = 0);
    ~file();
signals:
    void quest();
private:
    Ui::file *ui;
private slots:
    void receiveData(QString data);
    int read();
    void show_file_content();
    void on_pushButton_clicked();
    void close();
    int open();
    void save_info();
};

#endif // FILE_H
