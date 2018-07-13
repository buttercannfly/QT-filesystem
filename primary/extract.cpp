#include "extract.h"
#include "ui_extract.h"
#include "mainwindow.h"
#include<QMessageBox>

extract::extract(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::extract)
{
    ui->setupUi(this);
}

void extract::eccc(){
    emit getinfo();
    this->show();
}
extract::~extract()
{
    delete ui;
}

void extract::on_pushButton_clicked()
{
    char username[10];
    char password[10];
    QString tempa=ui->lineEdit->text();
    QString tempb=ui->lineEdit_2->text();
    QByteArray tempuser=tempa.toLocal8Bit();
    memset(username,0,sizeof(username));
    strcpy(username,tempuser.data());
    cout<<username<<endl;
    QByteArray temppass=tempb.toLocal8Bit();
    memset(password,0,sizeof(password));
    strcpy(password,temppass.data());
    cout<<password<<endl;
    cout<<user_num<<endl;


    if(strcmp(username,"")!=0&&strcmp(password,"")!=0)
    {
        int i;
        for (i = 0; i<user_num; i++)
                    {
                        if (strcmp(user_array[i].username, username) == 0) {
                            QMessageBox::information(this, "info", "用户已存在");
                            break;
                        }
                    }
                    if (i == user_num)
                        i=0;
    strcpy(user_array[user_num].username,username);
    strcpy(user_array[user_num].password,password);
    user_num++;
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    save_inf();
    this->hide();
    }

    else{
        QMessageBox::information(this, "info", "用户名密码不为空");
    }

}
void extract::save_inf()
{
    int i;
        fp = fopen("filesave", "wb");
        for (i = 0; i<20449; i++)
        {
            fwrite(&memory[i], sizeof(struct block), 1, fp);
            //free(&memory[i]);
        }
        fwrite(&super_block, sizeof(struct block_super), 1, fp);
        for (i = 0; i<640; i++)
        {
            fwrite(&i_node[i], sizeof(struct node), 1, fp);
            //free(&i_node[i]);
        }
        for (i = 0; i<640; i++)
        {
            fwrite(&root[i], sizeof(struct dir), 1, fp);
            //free(&root[i]);
        }
        for (int i = 0; i < 8; i++) {
            fwrite(&user_array[i], sizeof(struct user), 1, fp);
            //free(&user_array[i]);
        }
        fwrite(&user_num, sizeof(int), 1, fp);
        fclose(fp);
        fp=NULL;
}
