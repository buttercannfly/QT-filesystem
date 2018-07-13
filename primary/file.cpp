#include "file.h"
#include "ui_file.h"
#include<mainwindow.h>
#include<iostream>

using namespace std;

char a[40];
char fug[10];
QString name;


file::file(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file)
{
    ui->setupUi(this);

}

file::~file()
{
    delete ui;
}
void file::receiveData(QString data){
    emit quest();
    ui->label->setText(data);
    std::string name=string((const char *)data.toLocal8Bit());//转换数据类型
    int i;
    for(i=0;i < name.length();i++){
        fug[i]=name[i];
    }
    cout<<fug<<endl;
    open();
    read();
    show_file_content();
    this->show();
}

int file::read(){
    char filename[10];
    strcpy(filename,fug);
    int i;
        for (i = 0; i<640; i++)
        {
            if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)
            {
                int j;
                for (j = 0; j<8; j++)
                {
                    if (root[i].i_num == file_array[j])
                    {
                        if (1)
                        {
                            int add;
                            for (add = 0; add < 100; add++) {
                                temp_write= memory[i_node[root[i].i_num].file_address[add]].content;
                                if (temp_write.length() != 0)
                                    break;
                            }
                            std::cout << "main:"<<temp_write << endl;
                        }
                        else
                        {
                            cout<<"no ability"<<endl;
                        }
                        return 0;
                    }
                }
                if (j == 8)
                {

                }
                return 0;
            }
        }
        if (i == 640)
        {
            cout<<"not exist"<<endl;
        }
        return 0;
}

void file::show_file_content(){
        QString a=QString::fromStdString(temp_write);
        ui->textEdit->setText(a);
        cout<<"showed"<<"file_content";
        std::cout<<temp_write<<endl;
//        cout << p << endl;
}


void file::on_pushButton_clicked()
{
    char filename[10];
    QString writecc=ui->textEdit->toPlainText();
    std::string writec=string((const char *)writecc.toLocal8Bit());
    std::cout<<"writec:"<<writec<<endl;
    strcpy(filename,fug);
    cout<<filename<<endl;
    int i;
        for (i = 0; i<640; i++)
        {
            if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)
            {
                cout<<"get in smooth"<<endl;
                int j;
                for (j = 0; j<8; j++)
                {
                    if (root[i].i_num == file_array[j])
                    {
                        cout<<"running!"<<endl;
                        if (1)
                        {
                            int c, add, write_length;
//                            int temp;
                            for (add = 0; add<100; add++)
                            {
                                for (c = 0; c<1000; c++)
                                {
                                    if (memory[i_node[root[i].i_num].file_address[add]].content[c] == '\0')
                                    {
//                                        std::cout<<"read before"<<temp_write<<endl;
                                        int lead=temp_write.length();
                                        cout<<"length:"<<lead<<endl;
//                                        for(temp=0;temp<lead;temp++){

//                                        }
                                        for (write_length = 0; write_length < writec.length() && c<1000; c++, write_length++) {
                                            memory[i_node[root[i].i_num].file_address[add]].content[c] = writec[write_length];

                                        }
                                        close();
                                        cout<<"emitted"<<endl;
                                        save_info();
                                        temp_write="";
                                        std::cout<<"now"<<temp_write<<endl;
                                        return;
                                    }
                                }
                            }
                            if (add == 100)
                            {

                            }
                            return;
                        }
                        else
                        {

                            return;
                        }
                        return;
                    }
                }
                if (j == 8)
                {

                }
                return;
            }
        }
        if (i == 640)
        {

        }
//        this->close();
        return;
}

void file::close()
{
    this->hide();
    cout<<"closing"<<endl;
    char filename[10];
    strcpy(filename,fug);
    int i;
        for (i = 0; i<640; i++)
        {
            if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)
            {
                int j;
                for (j = 0; j<file_array_head; j++)
                {
                    if (root[i].i_num == file_array[j])
                    {
                        int m;
                        for (m = j; m<file_array_head; m++)
                        {
                            file_array[m] = file_array[m + 1];
                        }
                        file_array_head--;
                        return;
                    }
                }
                if (j == file_array_head)
                {

                }
                return;
            }
        }
        if (i = 640)
        {

        }
        return;
}

int file::open(){
    char filename[10];
    strcpy(filename,fug);
    int i;
        for (i = 0; i<640; i++)
        {
            if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)
            {
                if (file_array_head<8)
                {
                    cout<<"open succeed"<<endl;
                    file_array[file_array_head] = root[i].i_num;
                    file_array_head++;
                }
                else
                {
                    printf("打开的文件已达上限，无法打开本文件\n");
                }
                return root[i].i_num;
            }
        }
        if (i = 640)
        {
            printf("您要打开的文件不存在或不属于该用户\n");
        }
        return 0;
}

void file::save_info(){
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
