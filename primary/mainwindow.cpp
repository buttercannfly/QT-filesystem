#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include<QLineEdit>
extern "C"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    std::string a;
    ui->setupUi(this);
    if ((fp = fopen("filesave", "rb")) == NULL)   //判断系统文件是否存在
        {
            cout <<"intilizing success" << endl;
            format();
        }
        else
        {
            read_file();       //读取系统文件的内容
        }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::format()
{
    int i, j, k;
        super_block.n = 50;
        for (i = 0; i<50; i++)     //超级块初始化
        {
            super_block.free[i] = i;   //存放进入栈中的空闲块
                                       //super_block.stack[i]=50+i;  //存放下一组的盘块
        }

        for (i = 0; i<640; i++)     //i结点信息初始化
        {
            for (j = 0; j<100; j++)
            {
                i_node[i].file_address[j] = -1;//文件地址
            }
//            i_node[i].limit = -1;
            i_node[i].file_length = -1;  //文件长度
            i_node[i].file_style = -1; //文件类型
            i_node[i].file_UserId = -1;//用户ID
        }

        for (i = 0; i<640; i++)     //目录项信息初始化
        {
            strcpy(root[i].file_name, "");
            root[i].i_num = -1;
            strcpy(root[i].dir_name, "");
        }
        for (i = 0; i<20449; i++)     //存储空间初始化
        {
            memory[i].n = 0;      //必须有这个
            memory[i].a = 0;
            for (j = 0; j<50; j++)
            {
                memory[i].free[j] = -1;
            }

            int c;
            for (c = 0; c<1000; c++)
            {
                memory[i].content[c] = '\0';
            }

        }
        for (i = 0; i<20449; i++)    //将空闲块的信息用成组链接的方法写进每组的最后一个块中
        {         //存储空间初始化

            if ((i + 1) % 50 == 0)
            {
                k = i + 1;
                for (j = 0; j<50; j++)
                {
                    if (k<20450)
                    {
                        memory[i].free[j] = k;//下一组空闲地址
                        memory[i].n++;  //下一组空闲个数   注意在memory[i].n++之前要给其赋初值
                        k++;
                    }
                    else
                    {
                        memory[i].free[j] = -1;
                    }
                }
                memory[i].a = 0;    //标记为没有使用
                continue;     //处理完用于存储下一组盘块信息的特殊盘块后，跳过本次循环
            }
            for (j = 0; j<50; j++)
            {
                memory[i].free[j] = -1;
            }
            memory[i].n = 0;
        }
        int l;
        for (l = 0; l<8; l++)
        {
            file_array[l] = -1;
        }
        file_array_head = 0;
        int u;
        for (u = 0; u<100; u++)
        {
            physic[u] = -1;
        }
}

int MainWindow::on_login_clicked()
{

        char  getusername[10];
//        cout<<a<<endl;
        char  getpassword[10];
        QString tempa=ui->lineEdit_2->text();
        QString tempb=ui->lineEdit->text();
        QByteArray tempuser=tempa.toLocal8Bit();
        memset(getusername,0,sizeof(getusername));
        strcpy(getusername,tempuser.data());
        tempb=ui->lineEdit->text();
        QByteArray temppass=tempb.toLocal8Bit();
        memset(getpassword,0,sizeof(getpassword));
        strcpy(getpassword,temppass.data());
        int i=0;
        if (user_num == 0) {
            QMessageBox::information(this, "Title", "请先注册");//弹出界面，提示错误
            return 0;
        }
        else {
            for (i = 0; i < user_num; i++) {
                if (strcmp(getusername,user_array[i].username)==0 && strcmp(getpassword,user_array[i].password)==0) {
                    login_userid = i;
                    cur_user = getusername;
                    this->hide();
                    emit showmain();//跳转界面
                    return 1;
                }
                else {
                    continue;
                }
            }
            QMessageBox::information(this, "Title", "用户名密码错误");
            return 0;
        }

}

void MainWindow::on_reg_clicked()
{
    emit regi();
}

void MainWindow::read_file()
{
        int i;
        fp = fopen("filesave", "rb");
        for (i = 0; i<20449; i++)
        {
            fread(&memory[i], sizeof(struct block), 1, fp);
        }

        fread(&super_block, sizeof(struct block_super), 1, fp);

        for (i = 0; i<640; i++)
        {
            fread(&i_node[i], sizeof(struct node), 1, fp);
            /*if(i_node[i].file_length>1)
//                cout<<i<<".....";
                cout<<i_node[i].file_length<<endl*/;
        }

        for (i = 0; i<640; i++)
        {
            fread(&root[i], sizeof(struct dir), 1, fp);
        }

        for (i = 0; i < 8; i++)
        {
            fread(&user_array[i], sizeof(struct user), 1, fp);
        }
        fread(&user_num, sizeof(int), 1, fp);
        fclose(fp);
}
