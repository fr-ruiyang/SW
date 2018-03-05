#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#define M 2
#define D -1
#define DI -1

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

QString s1,s2;
/*打分规则
 *  匹配=3
 *  错配=-1
 *  空位=-2
*/

int max_flag(int a,int b,int c)
{
    if (c>=a&&c>=b)
        return 3;
    else if (a>=b&&a>=c)
        return 1;
    else
        return 2;
}

void Widget::on_pushButton_clicked()
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this,
                                          tr("选择文件"),
                                          "",
                                          tr("所有文件(*.*)"));

    if(filename.isEmpty())
    {
         return;
    }
    else
    {
        ui->lineEdit->setText(filename);
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) //加载文件
        {
            QMessageBox::warning(this,
                                     tr("警告"),
                                     tr("打开文件失败!"));
            return;
        }
//        file.readLine();
        s1=QString(file.readAll());
        s1.remove(QChar('\n'));
//        ui->textBrowser->append(tr("S串为："));
//        ui->textBrowser->append(s1);
    }
}

void Widget::on_pushButton_2_clicked()
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this,
                                          tr("选择文件"),
                                          "",
                                          tr("所有文件(*.*)"));

    if(filename.isEmpty())
    {
         return;
    }
    else
    {
        ui->lineEdit_2->setText(filename);
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) //加载文件
        {
            QMessageBox::warning(this,
                                     tr("警告"),
                                     tr("打开文件失败!"));
            return;
        }
//        file.readLine();
        s2=QString(file.readAll());
        s2.remove(QChar('\n'));
//        ui->textBrowser->append(tr("T串为："));
//        ui->textBrowser->append(s2);
    }
}

void Widget::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
    string s,t;
    if (s1.length()<=s2.length())
        s=s1.toStdString(),t=s2.toStdString();
    else
        s=s2.toStdString(),t=s1.toStdString();
    ui->textBrowser->append(tr("长串为："));
    ui->textBrowser->append(QString::fromStdString(t));
    ui->textBrowser->append(tr("短串为："));
    ui->textBrowser->append(QString::fromStdString(s));
    int m=s.length(),n=t.length();
    int matrix[m+1][n+1],flag[m+1][n+1];
    int i,j,max=0,max_s,max_t;
    for (i=0;i<=m;i++)
    {
        matrix[i][0]=0;
        flag[i][0]=0;
    }
    for (j=0;j<=n;j++)
    {
        matrix[0][j]=0;
        flag[0][j]=0;
    }
    for (i=1;i<=m;i++)
        for (j=1;j<=n;j++)
        {
            int r1=matrix[i][j-1]+DI;
            int r2=matrix[i-1][j]+DI;
            int r3=matrix[i-1][j-1];
            if (s.at(i-1)!=t.at(j-1)) r3=r3+D;
            else r3=r3+M;
            flag[i][j]=max_flag(r1,r2,r3);
            switch (flag[i][j]) {
            case 1:
                matrix[i][j]=r1;
                break;
            case 2:
                matrix[i][j]=r2;
                break;
            case 3:
                matrix[i][j]=r3;
                break;
            default:
                break;
            }
            if (matrix[i][j]<0)
            {
                matrix[i][j]=0;
                flag[i][j]=0;
            }
            if (matrix[i][j]>max)
            {
                max=matrix[i][j];
                max_s=i;
                max_t=j;
            }
        }

    for (i=0;i<=m;i++)
    {
        for (j=0;j<=n;j++)
            cout<<matrix[i][j]<<" ";
        cout<<endl;
    }

    i=max_s;j=max_t;
    vector <int> op;
    while (flag[i][j]!=0)
    {
        op.push_back(flag[i][j]);
        switch (op.back()) {
        case 1:
            j--;
            break;
        case 2:
            i--;
            break;
        case 3:
            i--;
            j--;
            break;
        default:
            break;
        }
    }
    int min_s=i,min_t=j;
    int k=i,l=j;
    string p=s,q=t;
    while (!op.empty())
    {
        switch (op.back()) {
        case 1:
//            ui->textBrowser->append(QString("%1%2%3").arg("insert(").arg(t[j-1]).arg("):"));
            p.insert(k,1,'_');
//            k++;
//            ui->textBrowser->append(QString::fromStdString(p));
//            j++;
            break;
        case 2:
//            ui->textBrowser->append(QString("%1%2%3").arg("delete(").arg(s[i-1]).arg("):"));
            q.insert(l,1,'_');
//            l++;
//            ui->textBrowser->append(QString::fromStdString(p));
//            i++;
            break;
//        case 3:
//            if (s[i-1]!=t[j-1])
//            {
//                ui->textBrowser->append(QString("%1%2%3%4%5").arg("replace(").arg(s[i-1]).arg(",").arg(t[j-1]).arg("):"));
//                p.replace(k,1,1,t[j]);
//                ui->textBrowser->append(QString::fromStdString(p));
//            }
//            i++;
//            j++;
//            k++;
//            l++;
//            break;
        default:
            break;
        }
        k++;
        l++;
        op.pop_back();
    }
    p.insert(0,min_t-min_s,' ');
    ui->textBrowser->append(tr("匹配结果为："));
    ui->textBrowser->append(QString::fromStdString(q));
    ui->textBrowser->append(QString::fromStdString(p));
}
