#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <windows.h>
#include <iostream>
using namespace std;

extern char rightly[10];
class Dialog;
//命令
struct command{
    char com[10];
};

//盘块信息（1KB/块）
struct block
{
    int n;				//存放空闲盘块的个数
    int free[50];		//存放空闲盘块的地址
    int a;				//盘块是否被占用标志
    char content[1000];	//块上每个字节存放的符号
};

//超级块
struct block_super{
    int n;      //空闲的盘块的个数
    int free[50];    //存放进入栈中的空闲块
};

//i结点信息（32B/i节点）
struct node{
    int file_style;    //i结点 文件类型
    int file_length;   //i结点 文件长度
    int file_address[100];  //文件占用的物理块号。
    int file_UserId;
};

//用户信息
struct user {
    char username[10];
    char password[10];
} ;

//目录项信息
struct dir      {
    char file_name[10];   //文件名
    int  i_num=-1;     //文件的结点号
    char dir_name[10];   //目录名或者说文件所在目录
};

extern FILE *fp;
extern int user_num;				//已注册用户数量
extern int login_userid;			//当前登录的用户ID
extern QString cur_user;				//当前用户
extern int file_array[8];			// 打开文件表组
extern int file_array_head;			//文件表组头
extern int physic[100];				//文件地址缓冲区
extern char cur_dir[10];			//当前目录
extern string temp_write;			//临时文件内容
extern node temp_file;			//临时文件


extern command cmd[17];
extern block memory[20449];
extern block_super super_block;
extern node i_node[640];
extern user user_array[8];
extern dir root[640];

/*函数定义*/
extern void del_dir(char filename[]);			//删除目录
extern void back_dir();							//返回上一级
extern void create_dir(char filename[]);		//创建目录
extern void display_curdir();					//显示当前目录下的文件列表
extern void display_dir(char filename[]);		//进入指定的目录
extern void close(char filename[10]);			//关闭文件
extern void del_file(char filename[]);			//删除文件
extern int open(char filename[10]);				//打开文件
extern void copy(char filename[10]);			//复制文件
extern void paste(char filename[10]);			//粘贴文件
extern int read(char filename[10]);				//读取文件内容
extern void show_file_content();				//显示文件内容
extern void show_file(char filename[]);			//显示文件信息
extern void help();								//命令帮助
extern void display_sys();						//显示系统信息（磁盘使用情况）
extern void read_file();				//读出系统文件的信息
extern void write_file();				//将信息写入系统文件
extern int login();								//登陆
extern void logout();							//登出
extern void reg();								//注册
extern void format();							//初始化
extern void allot(int length);					//分配空间
extern void callback(int length);				//回收空间
extern void write(char filename[10], string writec);//写入文件
extern void create_file(char filename[], int length, int userid); //创建文件
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void showmain();
    void regi();

private slots:
    int on_login_clicked();
    void format();
    void on_reg_clicked();
    void read_file();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
};

#endif // MAINWINDOW_H
