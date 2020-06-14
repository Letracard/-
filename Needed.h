#ifndef NEEDED_H
#define NEEDED_H

#include<QString>
#include<QTextEdit>
#include<QDebug>
#include<string>
using namespace std;
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"

//络合部分
//共存离子结构体
struct otherM
{
    QString name;//化学式
    double c;//计量点浓度

};


enum indicator{EBT=0,XO,MRX};//指示剂:EBT,XO和紫脲酸铵
enum ligand{F=0,Cl,Br,I,CN,SCN,NH3,S2O3=7,py,gly,lact,eda,phen};
        //配体:8-12分别为吡啶,甘氨酸,乳酸,乙二胺,邻二氮菲

//配体结构体
struct Ligand
{
    ligand L;//配体类型
    double c;//计量点浓度
};

ligand IntToLigand(int);

class MainR
{
public:
    MainR() {aM=1;aY=1; aMOH=aML=1; aYH=aYN=1;}//设置副反应系数初始值为1
    MainR(QString&,double,double);//传入主反应离子,初始浓度,金属离子浓度与Y浓度比值信息(默认浓度比为1)

    void Modify(QString&,double,double);//修改主反应(用于主反应数组)
    //获取M的副反应系数:传入pH和其他配体信息
    void GetaML(Ligand*,int,QTextEdit*);
    void GetaMLDefined(double*,double);
    void GetaMOH(double,QTextEdit*);
    void GetaMOHDefined(double *,double);

    //获取Y的副反应系数:传入pH和共存离子
    void GetaYN(otherM*,int);
    void GetaYH(double);

    void CalculateK_();//计算K_
    void Putout(QTextEdit*) const;//输出K_,aM,aY
    friend void ATJ(MainR*,int,QTextEdit*,bool*,double*,int*,double,int);//判断能否分步滴定
    void GetTE(double,indicator,QTextEdit*);//计算终点误差函数1:传入pH和指示剂
    void GetTE(double,QTextEdit* t);//计算终点误差函数2:用户自定义指示剂pMt'值
    bool operator>(MainR&);
    void operator=(MainR&);
    void setlgK(double);//设置lgK(仅用于排序)
    double GetlgK() const;//获取lgK
    friend void Shelter(MainR&,MainR&,double,double,int,double,int,QTextEdit*);//掩蔽计算
private:
    double lgK,lgK_,aM,aY,aMOH,aML,aYH,aYN,C,Ratio;
    //分别对应稳定常数，条件稳定常数(对数)，M和Y的副反应系数，更详细的副反应系数，起始浓度，金属离子浓度与Y浓度比值
    QString mainM;//主反应离子
};

//酸碱部分
//K值结构体
//酸
struct Ka
{
    string formula;//化学式
    double k[4];//各级常数
};

//碱
struct Kb
{
    string formula;//化学式
    double k[2];//各级常数
};

//可给出质子也可结合质子的两性物质
struct KAS
{
    string formula;//化学式
    double kCa,ka;//分别对应其共轭酸Ka值、其本身Ka值
};

//酸碱部分需要用到的函数声明
double getK1(string formula,bool type);
double getK2(string formula,bool type);
double getK3(string formula);
double getK4(string formula);
int getnumofH(string formula);
int getCpofH(string formula);
double CalculatepH1(double,double,int);
double getKAS(string,bool);
double CalculateTE_A(double,QString,int,bool);

//常用数据查询部分
void searchKa_pKa(string,QTextEdit*);
void searchKb_pKb(string,QTextEdit*);
void searchIndicator(QString,QTextEdit*);
void searchKMY(QString,QTextEdit*);
void searchPotential(QString,bool,QTextEdit*);

#endif // NEEDED_H
