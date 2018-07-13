#include "filewindow.h"
#include "ui_filewindow.h"
#include<mainwindow.h>
#include<QMessageBox>
#include<QMenu>
#include<QInputDialog>
#include<QGridLayout>
#include<QListWidgetItem>

int column=0;
int index=0;
struct f{
    char fname[10];
    int rows;
    int piece;
};
f nice[8];
int park=0;


int part_temp;

f d[8];
int dir_park=0;
char rightly[10];

Filewindow::Filewindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filewindow)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::IconMode);
    QObject::connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclicked(QListWidgetItem*)));
    QObject::connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*)));

}
void Filewindow::receivelogin()
{
    this->show();
    park=0;
    emit fire();
    refresh_pro();
    char a[10];
    char b[20]="login_user:";
    memset(a,0,sizeof(a));
    strcpy(a,user_array[login_userid].username);
    strcat(b,a);
    ui->label->setText(b);
}
Filewindow::~Filewindow()
{
    delete ui;
}

void Filewindow::refresh_pro(){
    ui->listWidget->clear();
   int i,k;
   memset(nice,0,sizeof(f));
   park=0;
   memset(d,0,sizeof(f));
   dir_park=0;
        for (i = 0; i<640; i++)
        {
            if (strcmp(cur_dir, root[i].dir_name) == 0 && i_node[root[i].i_num].file_UserId == login_userid)   //查询文件中 所在目录信息和当前目录信息相同的数据
            {
                k = root[i].i_num;
                if(i_node[k].file_length==20)
                {
                QListWidgetItem *item=new QListWidgetItem;
                item->setText(root[i].file_name);
                item->setIcon(QIcon(":/test.png"));
                ui->listWidget->addItem(item);//add item

                int count=ui->listWidget->count();;
                strcpy(nice[park].fname,root[i].file_name);
                nice[park].rows=count-1;
                nice[park].piece=i;
                park++;

                park=ui->listWidget->count();
                }
                else if(i_node[k].file_length==4){
                    QListWidgetItem *item=new QListWidgetItem;
                    item->setText(root[i].file_name);
                    item->setIcon(QIcon(":/icon.png"));
                    ui->listWidget->addItem(item);//add item

                    int count=ui->listWidget->count();
                    strcpy(d[dir_park].fname,root[i].file_name);
                    d[dir_park].rows=count-1;
                    d[dir_park].piece=i;

                    dir_park++;//park表示当前文件数目-1

                }


            }
        }
}

void Filewindow::contextMenuEvent(QContextMenuEvent *event)//右键菜单
{
    QMenu *menu = new QMenu(this);
    QAction *pNew = new QAction("新建文件",this);
    menu->addAction(pNew);
    connect(pNew,SIGNAL(triggered()),this,SLOT(add()));
    QAction *pp = new QAction("新建文件夹",this);
    connect(pp,SIGNAL(triggered()),this,SLOT(adddir()));
    menu->addAction(pp);
    menu->move(cursor().pos());
    menu->show();
}

void Filewindow::add()
{
//    park=ui->listWidget->count();
    cout<<"initial"<<park<<endl;
    cout<<"cur_dir"<<cur_dir<<endl;
    QListWidgetItem *item=new QListWidgetItem;
    bool ok;
    QString text=QInputDialog::getText(this,tr("输入文件名"),tr("输入文件名"),QLineEdit::Normal,tr("file"),&ok);
    std::string str=string((const char *)text.toLocal8Bit());//转换数据类型
    cout<<str<<endl;
    if(str!="")
    {
        int i=0, j=0,kk=0;
        int length=20;
//        std:string limit="o+r+w";
        char filename[10]="";
        for(kk=0;kk < str.length();kk++){
            filename[kk]=str[kk];
        }

            for (i = 0; i<640; i++)
            {
                //判断重名
                if (strcmp(filename, root[i].file_name) == 0)
                {
                    QMessageBox::information(this, "Title", "文件名重复");
                    return;
                }
            }
            for (i = 0; i<640; i++)
            {
                //找到空闲块
                if (root[i].i_num == -1)
                {
                    item->setText(text);
                    item->setIcon(QIcon(":/test.png"));

                    ui->listWidget->addItem(item);
                    int count=ui->listWidget->count();;
                    root[i].i_num = i;
                    strcpy(nice[park].fname,filename);
                    nice[park].rows=count-1;
                    nice[park].piece=i;

                    park++;//park表示当前文件数目-1
                    if(park>=8)
                    {
                        QMessageBox::information(this, "Title", "文件过多");
                    }
                    cout<<"now park_num:"<<park<<endl;

                    strcpy(root[i].file_name, filename);
                    cout<<cur_dir<<endl;
                    strcpy(root[i].dir_name, cur_dir);  //把当前目录名 给新建立的文件
                    i_node[i].file_style = 1;//style==0 说明文件是目录文件
                    i_node[i].file_length = 20;
//                    cout<<i_node[i].file_length<<"*******";
//                    cout<<i<<endl;
//                    i_node[i].limit = limit;
                    i_node[i].file_UserId = login_userid;
                    allot(length);//分配磁盘空间
                    for (j = 0; j<length; j++)
                    {
                        i_node[i].file_address[j] = physic[j];
                    }

                    //初始化文件
                    for (int add = 0; add < 100; add++)
                        for (int c = 0; c < length; c++) {
                            memory[i_node[root[i].i_num].file_address[add]].content[c] = '\0';
                        }
                    int u=0;
                    for (u = 0; u<100; u++)//分配完清空缓冲区
                    {
                        physic[u] = -1;
                    }

                    sava();
                    emit fire();
                    refresh_pro();
                    break;
                }
            }
    }
    else{
        QMessageBox::information(this, "Title", "文件不能为空,请重新输入");
    }

}
void Filewindow::adddir()
{
    QListWidgetItem *item=new QListWidgetItem;
    bool ok;
    QString text=QInputDialog::getText(this,tr("输入文件夹名"),tr("输入文件夹名称"),QLineEdit::Normal,tr("new_folder"),&ok);
    std::string str=string((const char *)text.toLocal8Bit());//转换数据类型
    if(str!="")
    {
        int i, j,kk;
        int length=4;
//        std:string limit="o+r+w";
        char filename[20]="";
        for(kk=0;kk < str.length();kk++){
            filename[kk]=str[kk];
        }
            for (i = 0; i<640; i++)
            {
                //判断重名
                if (strcmp(filename, root[i].file_name) == 0)
                {
                    printf("已经存在同名文件，不允许建立重名的文件\n");
                    return;
                }
            }
            for (i = 0; i<640; i++)
            {
                //找到空闲块
                if (root[i].i_num == -1)
                {
                    item->setText(text);
                    item->setIcon(QIcon(":/icon.png"));

                    ui->listWidget->addItem(item);
                    int count=ui->listWidget->count();
                    root[i].i_num = i;

                    strcpy(d[dir_park].fname,filename);
                    d[dir_park].rows=count-1;
//                    cout<<"********:::::"+d[dir_park].rows<<endl;
                    d[dir_park].piece=i;

                    dir_park++;//park表示当前文件数目-1
                    strcpy(root[i].file_name, filename);
//                    cout<<root[i].file_name<<endl;
                    strcpy(root[i].dir_name, cur_dir);  //把当前目录名 给新建立的文件
                    i_node[i].file_style = 0;//style==0 说明文件是目录文件
                    i_node[i].file_length = 4;
//                    i_node[i].limit = limit;
                    i_node[i].file_UserId = login_userid;
                    allot(length);//分配磁盘空间
                    for (j = 0; j<length; j++)
                    {
                        i_node[i].file_address[j] = physic[j];
                    }

                    //初始化文件
                    for (int add = 0; add < 100; add++)
                        for (int c = 0; c < length; c++) {
                            memory[i_node[root[i].i_num].file_address[add]].content[c] = '\0';
                        }
                    int u;
                    for (u = 0; u<100; u++)//分配完清空缓冲区
                    {
                        physic[u] = -1;
                    }
                    sava();
                    emit fire();
                    refresh_pro();
                    break;
                }
            }
    }
    else{
        QMessageBox::information(this, "Title", "文件夹名不能为空,请重新输入");
    }
}
void Filewindow::allocate()
{

}

void Filewindow::on_pushButton_clicked()//保存
{
        int i;
        fp = fopen("filesave", "wb");
        for (i = 0; i<20449; i++)
        {
            fwrite(&memory[i], sizeof(struct block), 1, fp);
        }
        fwrite(&super_block, sizeof(struct block_super), 1, fp);

        for (i = 0; i<640; i++)
        {
            fwrite(&i_node[i], sizeof(struct node), 1, fp);
        }
        for (i = 0; i<640; i++)
        {
            fwrite(&root[i], sizeof(struct dir), 1, fp);
        }
        for (int i = 0; i < 8; i++) {
            fwrite(&user_array[i], sizeof(struct user), 1, fp);
        }
        fwrite(&user_num, sizeof(int), 1, fp);
        fclose(fp);
        this->close();
}


void Filewindow::on_pushButton_2_clicked()//初始化
{
        int i, j, k=0;
        super_block.n = 50;
        for (i = 0; i<50; i++)     //超级块初始化
        {
            super_block.free[i] = i;   //存放进入栈中的空闲块
                                       //super_block.stack[i]=50+i;  //存放下一组的盘块
        }
//
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
        int l=0;
        for (l = 0; l<8; l++)
        {
            file_array[l] = -1;
        }
        file_array_head = 0;
        int u=0;
        for (u = 0; u<100; u++)
        {
            physic[u] = -1;
        }
        int counter =ui->listWidget->count();
            for(int index=0;index<counter;index++)
            {
                QListWidgetItem *item = ui->listWidget->takeItem(0);
                    delete item;
            }
        QMessageBox::about(this, "Title", "文件初始化完毕");
        sava();
        emit fire();
        refresh_pro();
}

void Filewindow::allot(int length)
{
    int i, j, k, m, p;
        for (i = 0; i<length; i++)
        {
            k = 50 - super_block.n;		//超级块中表示空闲块的指针
            m = super_block.free[k];	//栈中的相应盘块的地址
            p = super_block.free[49];   //栈中的最后一个盘块指向的地址
            if (p == -1)				//没有剩余盘块
            {
                printf("内存不足,不能够分配空间\n");
                callback(i);//之前已分配的i个盘块回收；
                break;
            }
            if (super_block.n == 1)
            {
                memory[m].a = 1;    //将最后一个盘块分配掉
                physic[i] = m;
                super_block.free[49] = -1;
                super_block.n = 0;
                for (j = 50 - memory[m].n; j<50; j++) //从最后一个盘块中取出下一组盘块号写入栈中
                {
                    super_block.free[j] = memory[m].free[j];
                    super_block.n++;
                }
                continue;     //要跳过这次循环，下面的语句在IF中已经执行过
            }
            physic[i] = m;     //如果栈中超过一个盘，栈中的相应盘块的地址写进 文件地址缓冲区
            memory[m].a = 1;
            m = -1;
            super_block.n--;
        }

}
void Filewindow::callback(int length){
    int i, j, k, m = 0;
        for (i = length - 1; i >= 0; i--)
        {
            k = physic[i];				//需要提供要回收的文件的地址
            m = 49 - super_block.n;		//回收到栈中的哪个位置
            if (super_block.n == 50)	//注意 当super_block.n==50时 m=-1;的值
            {							//super_block.n==50的时候栈满了，要将这个栈中的所有地址信息写进下一个地址中
                for (j = 0; j<50; j++)
                {
                    memory[k].free[j] = super_block.free[j];
                }
                int u;
                for (u = 0; u<50; u++)
                {
                    super_block.free[u] = -1;
                }
                super_block.n = 0;
                memory[k].n = 50;
            }
            memory[k].a = 0;
            if (m == -1)
            {
                m = 49;      //将下一个文件地址中的盘块号回收到栈底中，这个地址中存放着刚才满栈的地址的信息
            }
            super_block.free[m] = physic[i]; //将下一个文件地址中的盘块号回收到栈中
            super_block.n++;
        }
}

void Filewindow::on_delf_clicked()
{
    int i;int k,q;
    int row=ui->listWidget->currentRow();
    for(i=0;i<8;i++)
    {
        if(row==nice[i].rows&&park>=1)
        {
            int ty=row;
            del_file(nice[ty].fname);
            int a=nice[ty].rows;
            QListWidgetItem *item=ui->listWidget->takeItem(row);
            for(k=0;k<park;k++)
            {
                if(nice[k].rows>a&&park>=2)
                {
                    nice[k-1].rows=nice[k].rows-1;
                    nice[k-1].piece=nice[k].piece;
                    strcpy(nice[k-1].fname,nice[k].fname);
                }
            }

            nice[park-1].rows=-1;
            nice[park-1].piece=-1;
            nice[park-1].fname[0]='\0';
            park--;
            cout<<"del after"<<park<<endl;


            if(dir_park>=1){
            for(q=0;q<dir_park;q++){
                if(d[q].rows>a)
                {
                    d[q].rows=d[q].rows-1;
                }
            }

            d[dir_park-1].rows=-1;
            d[dir_park-1].piece=-1;
            d[dir_park-1].fname[0]='\0';
            if(dir_park>=1){            dir_park--;}
            }
            delete item;        //释放指针所指向的列表项
            sava();
            emit fire();
            refresh_pro();
            break;
        }

        else if(row==d[i].rows&&dir_park>=1){
            cout<<"deleting folder"<<endl;
        }
    }
    sava();
    emit fire();
    refresh_pro();
}

void Filewindow::del_file(char filename[])
{
    int i, j, k;
        for (i = 0; i<640; i++)
        {

            if ((strcmp(filename, root[i].file_name) == 0) && (strcmp(cur_dir, root[i].dir_name) == 0) && (i_node[root[i].i_num].file_UserId == login_userid))
            {
                int add, c;
                for (add = 0; add<i_node[root[i].i_num].file_length; add++)//文件内容清空
                {
                    for (c = 0; memory[i_node[root[i].i_num].file_address[add]].content[c] != '\0'; c++)
                    {
                        memory[i_node[root[i].i_num].file_address[add]].content[c] = '\0';
                    }
                }
                temp_write = "";
                k = root[i].i_num;
                i_node[k].file_UserId = -1;
//                i_node[k].limit = "";
                for (j = 0; j<i_node[k].file_length; j++)
                {
                    physic[j] = i_node[k].file_address[j];
                }
                callback(i_node[k].file_length); //调用 回收函数
                int u;//回收完情空缓存区
                for (u = 0; u<100; u++)
                {
                    physic[u] = -1;
                }
                for (j = 0; j<100; j++)     //删除文件后要将文件属性和目录项的各个值恢复初值
                {
                    i_node[k].file_address[j] = -1; //文件占用的块号地址恢复初值
                }
                strcpy(root[i].file_name, "");  //文件名恢复初值
                root[i].i_num = -1;				//目录项的I结点信息恢复初值
                strcpy(root[i].dir_name, "");	//目录项的文件目录信息恢复初值
                i_node[k].file_length = -1;		//文件长度恢复
                i_node[k].file_style = -1;		//文件类型恢复初值
                break;
            }
        }
        if (i == 640)
        {
            printf("登录用户的该目录下不存在这个文件\n");
        }
}

void Filewindow::singleclicked(QListWidgetItem *item)
{
    int i;
    char str[40];
    int row=ui->listWidget->currentRow();
    for(i=0;i<park;i++)
    {
        if(nice[i].rows==row)
        {
            sprintf(str,"目录名：%s 块号：%d 类型：FILE ",root[nice[i].piece].dir_name,root[nice[i].piece].i_num);
            ui->info->setText(str);
            cout<<"file"<<endl;
            break;
        }

    }
    for(i=0;i<dir_park;i++)
    {
        if(d[i].rows==row)
        {
            sprintf(str,"类型：directory ");
            cout<<"directory"<<endl;
            ui->info->setText(str);
            break;
        }
    }
}

void Filewindow::doubleclicked(QListWidgetItem *item)
{
    QString a;
    int i;
    int row=ui->listWidget->currentRow();
    for(i=0;i<park;i++)
    {
        if(nice[i].rows==row)
        {
            strcpy(rightly,nice[i].fname);
            a=QString::fromLocal8Bit(rightly,8);
            sava();
            emit showfile(a);
            refresh_pro();
            memset(rightly,0,sizeof(rightly));
            break;
        }
    }
    for(i=0;i<dir_park;i++)
    {
        if(d[i].rows==row)
        {
            strcpy(cur_dir, d[i].fname);
            cout<<"shuangji:::"<<cur_dir<<endl;
            memset(nice,0,sizeof(f));
            park=0;
            sava();
            emit fire();
            refresh_pro();
            park=part_temp+park;
            dir_park=0;
            sava();
//            park=park + ui->listWidget->count();
            break;
        }
    }
    sava();
    emit fire();
    refresh_pro();
}

void Filewindow::sava(){
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
        }
        for (int i = 0; i < 8; i++) {
            fwrite(&user_array[i], sizeof(struct user), 1, fp);
            //free(&user_array[i]);
        }
        fwrite(&user_num, sizeof(int), 1, fp);
        fclose(fp);
        fp=NULL;
}

void Filewindow::on_refresh_clicked()
{
    refresh_pro();
}

void Filewindow::on_retur_clicked()
{
        int  i, k;
        for (i = 0; i<640; i++)       //查询和当前目录名相同的目录文件名
        {
            k = root[i].i_num;
            if (strcmp(cur_dir, root[i].file_name) == 0 && (i_node[k].file_style == 0))
            {
                strcpy(cur_dir, root[i].dir_name); //将查询到的目录文件名  所在的目录赋值给当前目录
            }
        }
        park=0;
        dir_park=0;
        refresh_pro();
}

void Filewindow::refresh_pri(){
    ui->listWidget->clear();
   int i,k;
        for (i = 0; i<640; i++)
        {
            if (strcmp(cur_dir, root[i].dir_name) == 0 && i_node[root[i].i_num].file_UserId == login_userid)   //查询文件中 所在目录信息和当前目录信息相同的数据
            {
                k = root[i].i_num;
                if(i_node[k].file_length==20)
                {
                QListWidgetItem *item=new QListWidgetItem;
                item->setText(root[i].file_name);
                item->setIcon(QIcon(":/test.png"));
                ui->listWidget->addItem(item);//add item

                int count=ui->listWidget->count();;
                strcpy(nice[park].fname,root[i].file_name);
                nice[park].rows=count-1;
                nice[park].piece=i;
                park++;
                part_temp=ui->listWidget->count();
                }
                else if(i_node[k].file_length==4){
                    QListWidgetItem *item=new QListWidgetItem;
                    item->setText(root[i].file_name);
                    item->setIcon(QIcon(":/icon.png"));
                    ui->listWidget->addItem(item);//add item

                    int count=ui->listWidget->count();
                    strcpy(d[dir_park].fname,root[i].file_name);
                    d[dir_park].rows=count-1;
                    d[dir_park].piece=i;

                    dir_park++;//park表示当前文件数目-1

                }


            }
        }
}
