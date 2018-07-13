#include "mainwindow.h"
#include "filewindow.h"
#include "extract.h"
#include"file.h"
#include <QApplication>

QString cur_user;

dir root[640];
user user_array[8];			//用户组
int login_userid = -1;		//登陆用户ID
int user_num = 0;			//用户总数
block_super super_block;		//超级块
block memory[20449];			//盘块
int file_array[8]= { -1,-1,-1,-1,-1,-1,-1,-1 };			// 打开文件表组
int file_array_head;			//文件表组头
int physic[100];
FILE *fp;
node i_node[640];
string temp_write;
char cur_dir[10] = "filsystem";	//当前目录



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Filewindow f;
    MainWindow w;
    extract e;
    file fi;
    memset(i_node, 0, 640*sizeof(node));
    w.show();
    QObject::connect(&w,SIGNAL(showmain()),&f,SLOT(receivelogin()));
    QObject::connect(&w,SIGNAL(regi()),&e,SLOT(eccc()));
    QObject::connect(&f,SIGNAL(showfile(QString)),&fi,SLOT(receiveData(QString)));
    QObject::connect(&f,SIGNAL(fire()),&w,SLOT(read_file()));
    QObject::connect(&fi,SIGNAL(quest()),&w,SLOT(read_file()));
    QObject::connect(&e,SIGNAL(getinfo()),&w,SLOT(read_file()));
//    QObject::connect(&fi,SIGNAL(save_info()),&f,SLOT())
    return a.exec();
}
