#include "Needed.h"
#include <math.h>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"

//络合部分
ligand IntToLigand(int num)
{
    ligand a=F;
    switch(num)
    {case 0: a=F;break;
     case 1: a=Cl;break;
     case 2: a=Br;break;
     case 3: a=I;break;
     case 4: a=CN;break;
     case 5: a=SCN;break;
     case 6: a=NH3;break;
     case 7: a=S2O3;break;
     case 8: a=py;break;
     case 9: a=gly;break;
     case 10: a=lact;break;
     case 11: a=eda;break;
     case 12: a=phen;
    }
    return a;
}

MainR::MainR(QString& name,double c,double ratio)
{
    mainM=name;
    C=c;
    Ratio=ratio;

    QFile file("://Data/KMY.txt");
    file.open(QIODevice::ReadOnly);//以只读方式打开文件
    QTextStream iofile(&file);
    QString curN;//当前离子化学式
    double curK;//当前读取到K对数值
    iofile>>curN;
    while(name!=curN)
    {
    iofile>>curN;
    if(curN.isEmpty()==true) break;
    }
    if(curN==name) {iofile>>curK;lgK=curK;lgK_=lgK;}//输入对应K的对数值,并令初始K'等于K
    file.close();

    aM=1;
    aY=1;
    aMOH=aML=1;
    aYH=aYN=1;
}

void MainR::Modify(QString& name,double c,double ratio)//数据修改
{
    mainM=name;
    C=c;
    Ratio=ratio;

    QFile file("://Data/KMY.txt");
    file.open(QIODevice::ReadOnly);//以只读方式打开文件
    QTextStream iofile(&file);
    QString curN;//当前离子化学式
    double curK;//当前读取到K对数值
    iofile>>curN;
    while(name!=curN)
    {
    iofile>>curN;
    if(curN.isEmpty()==true) {break;}
    }
    if(curN==name) {iofile>>curK;lgK=curK;lgK_=lgK;}//输入对应K的对数值,并令初始K'等于K
    file.close();
};

void ATJ(MainR* p,int n,QTextEdit* t,bool* isS,double* cSp,int* indexp,double pH,int idcIndex)//注：p要求已按K值由大到小排序
{
    double curlgNE,curcK,dilution=1;//分别表征其他离子影响、cK'值、稀释倍数
    //查出需计算的离子数目(掩蔽的不必计算)
    int m=0;
    for(int i=0;i<n;i++)
    {
        if(isS[i]==true) {break;}
        m++;
    }

    double curpH;//记录读取到的当前pH
    double curaYH;//记录读取到的当前aYH
    QFile file("://Data/AE.txt");
    QTextStream iofile(&file);
    file.open(QIODevice::ReadOnly);
    iofile>>curpH>>curaYH;//读入当前pH和对应aYH值
    while(curpH<pH)
    {
        iofile>>curpH>>curaYH;
    }
    file.close();


    for(int i=0;i<m;i++)
    {curlgNE=1;
    dilution=dilution*(1+p[i].Ratio);//计算当前滴定完成稀释倍数
    //对掩蔽离子作处理
    for(int j=m;j<n;j++)
    {Shelter(p[i],p[j],dilution,cSp[j],indexp[j],pH,idcIndex,t);
    }
    //计算其他离子的影响(这里要考虑掩蔽离子)
    for(int k=i+1;k<n;k++)
    {curlgNE+=pow(10,p[k].lgK)*p[k].C/p[k].aM;
        if(isS[k]==true&&idcIndex>=0)//若是掩蔽离子,作掩蔽效果评价:能否控制酸度滴定
        {t->insertPlainText(QString("滴定%1时"
                             "lgaY(%2)=%3 ").arg(p[i].mainM)
                            .arg(p[k].mainM).arg(log10(1+pow(10,p[k].lgK)*p[k].C/p[k].aM/dilution)));
            if(log10(1+pow(10,p[k].lgK)*p[k].C/p[k].aM/dilution)<curaYH)
            {
                t->insertPlainText(QString("小于lgaYH=%1,据此判断掩蔽效果好\n").arg(curaYH));
            }
            else
            { t->insertPlainText(QString("大于lgaYH=%1,据此判断掩蔽效果不理想\n").arg(curaYH));}

        }
    }
    if(m-1!=i) {curlgNE=log10(curlgNE);}
    else curlgNE=0;

    curcK=log10(p[i].C)+p[i].lgK-curlgNE-log10(p[i].aML);
    if(m-1==i)//对最后的离子作修正：考虑稀释
    {curcK-=log10(dilution);}
    t->insertPlainText(QString("计算得%1的lgcK'值为%2").arg(p[i].mainM).arg(curcK));

    if(curcK>6)
    {
    t->insertPlainText(" 可准确滴定\n");
    }
    else t->insertPlainText(" 不可准确滴定\n");
    }

};

void MainR::GetTE(double pH,indicator n,QTextEdit* t)
{
    double TE,curpH,pMt,dpM;//分别对应终点误差,当前pH,
    QString curN,curpHstr;//分别对应当前读取到金属离子,当前pH(字符串形式)
    QFile file;
    QTextStream iofile(&file);
    switch(n)
    {case 0:file.setFileName("://Data/EBT.txt");break;
    case 1:file.setFileName("://Data/XO.txt");break;
    case 2:file.setFileName("://Data/MRX.txt");break;
    }
    file.open(QIODevice::ReadOnly);
    iofile>>curN;

    while(curN!=mainM)//查找对应金属离子
    {iofile>>curN;
     if(curN.isEmpty()==true) break;
    }

    if(curN==mainM)
    {
        iofile>>curpH;

    while(curpH!=pH)//查找对应pH值
    {iofile>>curpHstr;//读入对应pH值(QString形式)
    if(curpHstr.isEmpty()==true) break;
    curpH=curpHstr.toDouble();//读入的pH值转换为double
    }

    if(curpH==pH)
    {iofile>>pMt;
     cout<<pMt;
    dpM=pMt-0.5*(lgK_-log10(C/(1+Ratio)));//计算ΔpM'
    t->insertPlainText(QString("ΔpM'为%1").arg(dpM));
    TE=(pow(10,dpM)-pow(10,-dpM))/sqrt(pow(10,(lgK_+log10(C/(1+Ratio)))));//根据公式计算出TE
    t->insertPlainText(QString(" 终点误差为%1%\n").arg(100*TE));
    }
    else {t->insertPlainText("提示:不支持该离子指定pH下的TE计算！\n");return;}
    }

    else {t->insertPlainText("提示:此条件下不支持该离子的TE计算！\n");return;}

};

void MainR::GetTE(double pMt,QTextEdit* t)
{
    double dpM,TE;
    dpM=pMt-0.5*(lgK_-log10(C/(1+Ratio)));//计算ΔpM'
    t->insertPlainText(QString("ΔpM'为%1").arg(dpM));
    TE=(pow(10,dpM)-pow(10,-dpM))/sqrt(pow(10,(lgK_+log10(C/(1+Ratio)))));//根据公式计算出TE
    t->insertPlainText(QString(" 终点误差为%1%\n").arg(100*TE));

}

void MainR::GetaYN(otherM * s, int n)//计算aYN
{
    QFile file("://Data/KMY.txt");
    file.open(QIODevice::ReadOnly);//以只读方式打开文件
    QTextStream iofile(&file);

    //共存离子效应计算
    double curlgK,*data = new double[n];//分别对应读取到的K值和计算得到的NY/Y
    QString curN;//记录读取到的字符串

    iofile>>curN;

    for(int i=0;i<n;i++)//查找对应K值
    {while(curN!=s[i].name) {iofile>>curN;}
    if(curN==s[i].name)
    {iofile>>curlgK;//读入K值
    data[i]=(s[i].c)*pow(10.0,curlgK);//用对应K值计算NY与Y浓度之比
    }
    file.seek(0);
    }

    for(int i=0;i<n;i++)//计算共存离子效应系数
    {aYN+=data[i];
     aY+=data[i];
    }
    delete []data;
    file.close();

}

void MainR::GetaYH(double pH)//计算aYH
{
    if(pH<=12.2&&pH>=0.0)
    {QFile file;
    QTextStream iofile(&file);
    double curpH;//记录读取到的当前pH
    double curaYH;//记录读取到的当前aYH
    file.setFileName("://Data/AE.txt");
    file.open(QIODevice::ReadOnly);
    iofile>>curpH>>curaYH;//读入当前pH和对应aYH值
    while(curpH<pH)
    {
        iofile>>curpH>>curaYH;
    }
    aYH=pow(10,curaYH)-1;
    aY+=pow(10,curaYH)-1;
    file.close();
    }

    if(pH<0) {}


}

void MainR::GetaML(Ligand *s, int n,QTextEdit* t)//计算aML
{
    double addaML;//对应当前读取到络合效应常数
    QFile file;
    QTextStream iofile(&file);
    QString curN1,Lname;//对应读取到的当前金属离子,当前配体名称
        for(int i=0;i<n;i++)
    {
        switch(s[i].L)
        {
    case 0: file.setFileName("://Data/F.txt");Lname="F";break;
    case 1: file.setFileName("://Data/Cl.txt");Lname="Cl";break;
    case 2: file.setFileName("://Data/Br.txt");Lname="Br";break;
    case 3: file.setFileName("://Data/I.txt");Lname="I";break;
    case 4: file.setFileName("://Data/CN.txt");Lname="CN";break;
    case 5: file.setFileName("://Data/SCN.txt");Lname="SCN";break;
    case 6: file.setFileName("://Data/NH3.txt");Lname="NH3";break;
    case 7: file.setFileName("://Data/S2O3.txt");Lname="S2O3";break;
    case 8: file.setFileName("://Data/py.txt");Lname="py";break;
    case 9: file.setFileName("://Data/gly.txt");Lname="甘氨酸";break;
    case 10: file.setFileName("://Data/lact.txt");Lname="乳酸";break;
    case 11: file.setFileName("://Data/eda.txt");Lname="eda";break;
    case 12: file.setFileName("://Data/phen.txt");Lname="phen";break;
    }

    file.open(QIODevice::ReadOnly);
    iofile>>curN1;
    while(curN1 != mainM)
    {iofile>>curN1;
    if(curN1.isEmpty()==true) break;//修正:文件读完时退出循环
    }

    if(curN1==mainM)
    {
        iofile>>addaML;//读入第一个形成常数值
    for(int k=1;addaML!=-1;k++)//判断读取是否应终止: -1代表终止
    {
        if(addaML!=0)
        {aM+=pow(10.0,addaML)*pow(s[i].c,k);
         aML+=pow(10.0,addaML)*pow(s[i].c,k);}//判断数据是否缺失(0代表数据缺失),如否,进行计算
        iofile>>addaML;
    }
    }
    else
    {t->insertPlainText(QString("提示:没有找到配体%1与金属%2"
                                "络合的稳定常数数据,不作计算,请使用自定义配体功能"
                                "\n").arg(Lname).arg(mainM));}
    }
    file.close();

}

void MainR::GetaMLDefined(double *beta,double C)//根据用户自定义计算aML
{
    for(int i=0;i<6;i++)
    {
        if(beta[i]!=0)
        {aM+=pow(10,beta[i])*pow(C,i+1);
        aML+=pow(10,beta[i])*pow(C,i+1);
        }
    }

}

void MainR::GetaMOH(double pH,QTextEdit* t)//计算aMOH
{
    QFile file;
    QTextStream iofile(&file);
    double addaMOH;//对应当前读取到水解效应常数
    QString curN2;//读取到的当前金属离子
    file.setFileName("://Data/OH.txt");
    file.open(QIODevice::ReadOnly);
    iofile>>curN2;

    while(curN2!=mainM)//文件中查找对应金属离子
    {iofile>>curN2;
    if(curN2.isEmpty()==true) break;//修正:文件读完时退出循环
    }

    if(curN2==mainM)
    {
    iofile>>addaMOH;//读入第一个形成常数值
    for(int k=1;addaMOH!=-1;k++)//判断读取是否应终止: -1代表终止
    {
        if(addaMOH!=0)
        {aM+=pow(10,addaMOH)*pow((pow(10,-14.0)/pow(10,-pH)),k);
        aMOH+=pow(10,addaMOH)*pow((pow(10,-14.0)/pow(10,-pH)),k);}//判断数据是否缺失(0代表数据缺失),如否,进行计算
        iofile>>addaMOH;
    }
    }
    else {t->insertPlainText("提示:没有找到该金属离子水解稳定常数数据,"
                             "无法计算,请自定义水解效应常数\n");}
    file.close();
}

void MainR::GetaMOHDefined(double *beta,double pH)//根据用户自定义计算aMOH
{
    for(int i=0;i<6;i++)
    {if(beta[i]!=0)
        {aM+=pow(10,beta[i])*pow((pow(10,-14.0)/pow(10,-pH)),i+1);
        aMOH+=pow(10,beta[i])*pow((pow(10,-14.0)/pow(10,-pH)),i+1);
        }
    }

}

void MainR::CalculateK_()
{
    lgK_=lgK-log10(aM)-log10(aY);
};

void MainR::Putout(QTextEdit* t) const
{
    t->insertPlainText(QString("该条件下%1的条件稳定常数对数值为%2\n"
      "αM取对数为%3 其中lgαM(L)=%4 lgαM(OH)=%5\n"
      "αY取对数为%6 其中lgαY(H)=%7 lgαY(N)=%8\n").arg(mainM).arg(lgK_).arg(log10(aM)).
       arg(log10(aML)).arg(log10(aMOH)).arg(log10(aY)).arg(log10(aYH)).arg(log10(aYN)));
};

bool MainR::operator>(MainR& mr2)
{
    if(lgK>mr2.lgK) return true;
    else return false;
};

void MainR::operator=(MainR& mr2)
{
    mainM=mr2.mainM;
    lgK=mr2.lgK;
    lgK_=mr2.lgK_;
    C=mr2.C;
    aM=mr2.aM;
    aY=mr2.aY;
    Ratio=mr2.Ratio;

}

void MainR::setlgK(double k)
{
    lgK=k;
};

double MainR::GetlgK() const
{
    return lgK;
}

void Shelter(MainR& truemR,MainR& mr,double dilution,double cL,int index,double pH,int idcIndex,QTextEdit* t)
{
    QFile file;
    QTextStream iofile(&file);
    double addaML,shRatio=1;//对应读取到的形成常数,计算所得掩蔽前后浓度比
    QString curN1,Lname;//对应读取到的当前金属离子,当前配体名称
    switch(index)
    {case 0: file.setFileName("://Data/F.txt");Lname="F";break;
    case 1: file.setFileName("://Data/Cl.txt");Lname="Cl";break;
    case 2: file.setFileName("://Data/Br.txt");Lname="Br";break;
    case 3: file.setFileName("://Data/I.txt");Lname="I";break;
    case 4: file.setFileName("://Data/CN.txt");Lname="CN";break;
    case 5: file.setFileName("://Data/SCN.txt");Lname="SCN";break;
    case 6: file.setFileName("://Data/NH3.txt");Lname="NH3";break;
    case 7: file.setFileName("://Data/S2O3.txt");Lname="S2O3";break;
    case 8: file.setFileName("://Data/py.txt");Lname="py";break;
    case 9: file.setFileName("://Data/gly.txt");Lname="甘氨酸";break;
    case 10: file.setFileName("://Data/lact.txt");Lname="乳酸";break;
    case 11: file.setFileName("://Data/eda.txt");Lname="eda";break;
    case 12: file.setFileName("://Data/phen.txt");Lname="phen";break;
    }

    file.open(QIODevice::ReadOnly);
    iofile>>curN1;
    while(curN1!=mr.mainM)
    {iofile>>curN1;
    if(curN1.isEmpty()==true) break;//修正:文件读完时退出循环
    }

    if(curN1==mr.mainM)
    {
        iofile>>addaML;//读入第一个形成常数值
    for(int k=1;addaML!=-1;k++)//判断读取是否应终止: -1代表终止
    {
        if(addaML!=0)
        {shRatio+=pow(10.0,addaML)*pow(cL/dilution,k);}//判断数据是否缺失(0代表数据缺失),如否,进行计算
        iofile>>addaML;
    }
    mr.aM = shRatio;//将掩蔽前后浓度比传入对应aN中
    }

    else
    {t->insertPlainText(QString("提示:没有找到被掩蔽离子%1与该掩蔽剂络合的数据!"
                       "给出不计掩蔽的结果!\n").arg(mr.mainM));
    }
    file.close();

    //检查对主反应的影响
    shRatio=1;
    file.open(QIODevice::ReadOnly);
    iofile>>curN1;
    while(curN1!=truemR.mainM)
    {iofile>>curN1;
    if(curN1.isEmpty()==true) break;
    }

    if(curN1==truemR.mainM)
    {
        iofile>>addaML;//读入第一个形成常数值
        for(int k=1;addaML!=-1;k++)//判断读取是否应终止: -1代表终止
        {if(addaML!=0)
        {shRatio+=pow(10.0,addaML)*pow(cL/dilution,k);}//判断数据是否缺失(0代表数据缺失),如否,进行计算
        iofile>>addaML;
        }

        t->insertPlainText(QString("注意:配体%1对%2同时也产生影响! "
         "导致的lgαM(L)=%3!\n").arg(Lname).arg(truemR.mainM).arg(log10(shRatio)));
    }
    truemR.aML = shRatio;
    file.close();


    if(idcIndex>=0)
    {switch(idcIndex)
    {case 0:file.setFileName("://Data/EBT.txt");break;
    case 1:file.setFileName("://Data/XO.txt");break;
    case 2:file.setFileName("://Data/MRX.txt");break;
    }
    file.open(QIODevice::ReadOnly);

    double curpH,pMt;//分别对应当前pH,变色点pM
    QString curN,curpHstr;//分别对应当前读取到金属离子,当前pH(字符串形式)
    while(curN!=mr.mainM)//查找对应金属离子
    {iofile>>curN;
     if(curN.isEmpty()==true) break;
    }

    if(curN==mr.mainM)
    {iofile>>curpH;

    while(curpH!=pH)//查找对应pH值
    {iofile>>curpHstr;//读入对应pH值(QString形式)
    if(curpHstr.isEmpty()==true) break;
    curpH=curpHstr.toDouble();//读入的pH值转换为double
    }

    if(curpH==pH)
    {iofile>>pMt;
        if(mr.C/mr.aM/dilution<pow(10,-pMt))
        {t->insertPlainText(QString("滴定%1到计量点时%2"
           "的pMsp=%3,大于变色点pMt=%4,据此判断掩蔽效果好\n").arg(truemR.mainM)
                            .arg(mr.mainM).arg(-log10(mr.C/mr.aM/dilution))
                            .arg(pMt));
        }

        else
        {t->insertPlainText(QString("滴定%1到计量点时%2"
           "的pMsp=%3,不大于变色点pMt=%4,据此判断掩蔽效果不理想\n").arg(truemR.mainM)
                            .arg(mr.mainM).arg(-log10(mr.C/mr.aM/dilution))
                            .arg(pMt));
        }
    }

    else {t->insertPlainText(QString("提示:没有找到%1在此pH下的pMt,"
                             "无法通过判断其是否与指示剂"
                             "显色来进行掩蔽效果评价！\n").arg(mr.mainM));return;}
    }
    else {t->insertPlainText(QString("提示:此条件下没有找到%1的pMt,无法通过判断其是否与指示剂"
                                     "显色来进行掩蔽效果评价!\n").arg(mr.mainM));return;}
    file.close();
    }
}


//酸碱部分
//导入K值数据
//Ka
Ka KaD[64]=
{
    {"NH4+",{5.6e-10,0,0,0}},
    {"NH3NH2+",{1.02e-8,0,0,0}},
    {"NH4+",{1.1e-6,0,0,0}},
    {"H3AsO4",{6.5e-3,1.15e-7,3.2e-12,0}},
    {"H3AsO2",{6.0e-10,0,0,0}},
    {"H3BO2",{5.8e-10,0,0,0}},
    {"H2B4O7",{1e-4,1e-9,0,0}},
    {"H2CO3",{4.3e-7,4.8e-11,0,0}},
    {"H2CrO4",{0.16,3.2e-7,0,0}},//有特殊情况但不做处理
    {"HNCO",{3.3e-4,0,0,0}},
    {"H2S",{8.9e-8,1.20e-13,0,0}},
    {"HCN",{4.9e-10,0,0,0}},
    {"HF",{6.8e-4,0,0,0}},
    {"H2O2",{1.8e-12,0,0,0}},
    {"HBrO",{2.4e-9,0,0,0}},
    {"HClO",{3.0e-8,0,0,0}},
    {"HIO",{2.3e-11,0,0,0}},
    {"HNO2",{5.1e-4,0,0,0}},
    {"H3PO4",{6.9e-3,6.2e-8,4.8e-13,0}},
    {"H4P2O7",{3.0e-2,4.4e-3,2.5e-7,5.6e-10}},
    {"H3PO3",{5.0e-2,2.5e-7,0}},
    {"H2SiO3",{1.7e-10,1.58e-12,0,0}},
    {"H2SO4",{1e9,1.1e-2,0,0}},
    {"H2SO3",{1.29e-1,6.3e-8,0,0}},
    {"HSCN",{1.41e-1,0,0,0}},
    {"H2S2O3",{2.5e-1,1.9e-2,0,0}},
    {"HCOOH",{1.7e-4,0,0,0}},
    {"CH3COOH",{1.754e-5,0,0,0}},
    {"HAc",{1.754e-5,0,0,0}},
    {"C2H5COOH",{1.34e-5,0,0,0}},
    {"(CH3)2CHCOOH",{1.41e-5,0,0,0}},
    {"CH3(CH2)2COOH",{1.48e-5,0,0,0}},
    {"ClCH2COOH",{1.38e-3,0,0,0}},
    {"Cl2CHCOOH",{5.5e-2,0,0,0}},
    {"Cl3CCOOH",{2.2e-1,0,0,0}},
    {"H2NCH2COOH",{4.5e-3,1.7e-10,0,0}},
    {"CH3CH(OH)COOH",{1.32e-4,0,0,0}},
    {"H2N(CH2)4CH(NH2)COOH",{6.6e-3,0,0,0}},
    {"C6H5COOH",{6.2e-5,0,0,0}},
    {"H2C2O4",{5.6e-2,5.1e-5,0,0}},
    {"HOOCCOOH",{5.6e-2,5.1e-5,0,0}},
    {"C6H5OH",{1.12e-10,0,0,0}},
    {"o-C6H4(COOH)2",{1.122e-3,3.91e-6,0,0}},
    {"m-C6H4(COOH)2",{2.4e-4,2.5e-5,0,0}},
    {"p-C6H4(COOH)2",{2.9e-4,3.5e-5,0,0}},
    {"HOOCCH=CHCOOH",{1.2e-2,6.0e-7,0,0}},
    {"HOOCCH2CHOHCOOH",{4.0e-4,8.9e-6,0,0}},
    {"HOOCCH2COOH",{1.41e-3,2.2e-6,0,0}},
    {"Vitamin C",{6.8e-5,2.8e-12,0,0}},
    {"C6H8O6",{6.8e-5,2.8e-12,0,0}},//VC
    {"HOOCCH2CH2COOH",{6.2e-5,2.3e-6,0,0}},
    {"H2NC6H4SO3H",{2.6e-1,7.6e-4,0,0}},
    {"HO(COOH)C6H3SO3H",{4.7e-3,4.8e-12,0,0}},
    {"(±)HOOC(OH)CHCH(OH)COOH",{6.0e-4,1.55e-5,0,0}},
    {"(+)HOOC(OH)CHCH(OH)COOH",{9.1e-4,4.3e-5,0,0}},
    {"C2H2(COOH)2",{9.5e-4,4.1e-5,0,0}},
    {"C4H3OCOOH",{6.9e-4,0,0,0}},
    {"C3H5NH2(COOH)",{7.9e-3,8.5e-5,3.4e-10,0}},
    {"HOOCCH2C((OH)COOH)CH2COOH",{7.4e-4,1.7e-5,4.0e-7,0}},
    //{"EDTA",{5.4e-7,1.12e-11}},//K5,K6
    {"CH3COCH2COOH",{2.6e-4,0,0}},
    {"CH3COCH2COCH3",{1e-9,0,0,0}},
    {"CH2=CHCOOH",{5.5e-5,0,0,0}},
    {"HOOC(CH2)4COOH",{3.7e-5,3.9e-6,0,0}},
    {"HOC6H4COOH",{1.05e-3,0,0,0}}
};

//Kb
Kb KbD[24]=
  {
    {"NH3",{1.8e-5,0}},
    {"NH2NH2",{9.8e-7,1.32e-15}},
    {"NH2OH",{9.1e-9,0}},
    {"CH3CONH2",{2.5e-13,0}},
    {"CH3(CH2)4NH2",{4.0e-4,0}},
    {"C6H5NH2",{4.2e-10,0}},
    {"o-H2NC5H4OCH3",{3.1e-10,0}},
    {"m-H2NC5H4OCH3",{1.58e-10,0}},
    {"p-H2NC5H4OCH3",{1.9e-10,0}},
    {"Gln(谷氨酰胺)",{6.3e-6,1.05e-12}},
    {"(H2NC6H4)2",{5.0e-10,4.3e-11}},
    {"(C2H5)2NH",{8.5e-4,0}},
    {"(CH3)2NH",{5.9e-4,0}},
    {"Ph2NH",{8e-14,0}},
    {"H2NC2H4OH",{3e-5,0}},
    {"C2H5NH2",{4.3e-4,0}},
    {"H2NCH2CH2NH2",{8.5e-5,7.1e-8}},
    {"eda",{8.5e-5,7.1e-8}},
    {"H2N(CH2)6NH2",{8.5e-4,6.8e-5}},
    {"CH3(CH2)5NH2",{3.6e-4,0}},
    {"(CH2)6N4",{1.35e-9,0}},
    {"CH3NH2",{4.2e-4,0}},
    {"Ac-",{5.6e-10,0}},
    {"CH3COO-",{5.6e-10,0}}
   };

//可结合质子也可给出质子的两性物质
KAS KASD[13] =
{{"HCO3-",4.3e-7,4.8e-11},
 {"H2PO4-",6.9e-3,6.2e-8},
 {"HPO4_2-",6.2e-8,4.8e-13},
 {"HPO42-",6.2e-8,4.8e-13},
 {"HSO3-",1.29e-1,6.3e-8},
 {"H2PO3-",5.0e-2,2.5e-7},
 {"HS-",8.9e-8,1.20e-13},
 {"HB4O7-",1e-4,1e-9},
 {"HCrO4-",0.16,3.2e-7},
 {"HC2O4-",5.6e-2,5.1e-5},
 {"H2AsO4-",6.5e-3,1.15e-7},
 {"HAsO4_2-",1.15e-7,3.2e-12},
 {"HAsO42-",1.15e-7,3.2e-12}
};

double getK1(string formula,bool type)//获取酸(碱)的K1
{
    string x;
    x=formula;
    if(type==true)
    {for(int i=0;i<64;i++)
        {
            if(KaD[i].formula==x)
            {
                return KaD[i].k[0];
            }
        }
    }
    if(type==false)
    {for(int i=0;i<24;i++)
        {
            if(KbD[i].formula==x)
            {
                return KbD[i].k[0];
            }
        }
    }

    return 0;
};

double getK2(string formula,bool type)//获取酸(碱)的K2
{
    string x;
    x=formula;
    if(type==true)
    {for(int i=0;i<64;i++)
        {
            if(KaD[i].formula==x)
            {
                return KaD[i].k[1];
            }
        }
    }
    if(type==false)
    {for(int i=0;i<24;i++)
        {
            if(KbD[i].formula==x)
            {
                return KbD[i].k[1];
            }
        }
    }

    return 0;
};

double getK3(string formula)//获取酸的K3
{
    string x;
    x=formula;
    for(int i=0;i<64;i++)
        {
            if(KaD[i].formula==x)
            {
                return KaD[i].k[2];
            }
        }
    return 0;
};

double getK4(string formula)//获取酸的K4
{
    string x;
    x=formula;

    for(int i=0;i<64;i++)
        {
            if(KaD[i].formula==x)
            {
                return KaD[i].k[3];
            }
        }
    return 0;
};

int getnumofH(string formula)//获取酸中氢的个数(查找不到则返回0)
{
    string x;
    x=formula;
    int numofH=0;
    for(int i=0;i<64;i++)
    {
        if(x==KaD[i].formula)
        {
            for(int j=0;j<4;j++)
            {
                if(KaD[i].k[j]!=0)
                {
                    numofH++;
                }
             }
            break;
        }
    }

    return numofH;//四元酸
};

int getCpofH(string formula)//获取碱可以接收的质子数(查找不到则返回0)
{   string x;
    x=formula;
    int numofH=0;
    for(int i=0;i<24;i++)
    {
        if(x==KbD[i].formula)
        {if(KbD[i].k[0]!=0) {numofH++;}
         if(KbD[i].k[1]!=0) {numofH++;}
            break;
        }
    }
    return numofH;//二元碱

}

double getKAS(string formula,bool type)//获取可结合质子也可给出质子的两性物质对应K值
{   double targetK=0;
    for(int i=0;i<13;i++)
    {if(formula==KASD[i].formula)
        {if(type==true) {targetK=KASD[i].kCa;}//type为true:获取共轭酸Ka值
         else {targetK=KASD[i].ka;}//type为false:获取酸Ka值
        }

    }
    return targetK;
}

double CalculatepH1(double c,double K,int index)//计算pH
{
    double pH=7;
    if(index==0)
    {if(K*c>=20*pow(10,-14))
        {
            if(c/K>=500)
            {pH=-log10(sqrt(K*c));}
            else pH=-log10((-K+sqrt(K*K+4*K*c))/2);
        }
        else pH=-log10(sqrt(K*c+pow(10,-14)));
    }

    if(index==1)
    {if(K*c>=20*pow(10,-14))
        {
            if(c/K>=500)
            {pH=14+log10(sqrt(K*c));}
            else pH=14+log10((-K+sqrt(K*K+4*K*c))/2);
        }
        else pH=14+log10(sqrt(K*c+pow(10,-14)));

    }

    return pH;
}

double CalculateTE_A(double Csp,QString formula,int idcIndex,bool type)//计算终点误差
{
    double TE=-1;//终点误差值

    if(type==false&&getnumofH(formula.toStdString())==0)//若找不到对应的酸,直接返回
    {return TE;}

    if(type==true&&getCpofH(formula.toStdString())==0)//若找不到对应的碱,直接返回
    {return TE;}

    QString idcName;//指示剂化学式
    switch (idcIndex)
    {case 0: idcName="TB(第1次变色)";break;
    case 1: idcName="TB(第2次变色)";break;
    case 2: idcName="MY";break;
    case 3: idcName="MO";break;
    case 4: idcName="BTB";break;
    case 5: idcName="NR";break;
    case 6: idcName="PR";break;
    case 7: idcName="PP";break;
    case 8: idcName="TP";break;
    case 9: idcName="BPB";break;
    case 10: idcName="BCG";break;
    case 11: idcName="MR";break;
    case 12: idcName="BCP";
    }
    QFile file("://Data/indicator.txt");
    QTextStream iofile(&file);
    QString curN;
    file.open(QIODevice::ReadOnly);
    iofile>>curN;
    while(curN!=idcName)
    {iofile>>curN;
    if(curN.isEmpty()==true) break;
    }

    double pH,K;//获取终点pH,K值
    if(type==false)  {iofile>>pH;
                      K=getK1(formula.toStdString(),true);}//强碱滴定时读取酸色变色点
    else {iofile>>pH>>pH;
          K=getK1(formula.toStdString(),false);}//强酸滴定时读取碱色变色点

    cout<<pH<<K;
    //计算终点误差
    if(type==false)
    {
        double delta = pow(10,-pH)/(K+pow(10,-pH));
        TE = (pow(10,pH-14)-pow(10,-pH))/Csp - delta;
    }

    if(type==true)
    {
        double delta = pow(10,-14)/K/(pow(10,-14)/K+pow(10,-pH));
        TE = (pow(10,pH-14)-pow(10,-pH))/Csp - delta;
    }


    return TE;

}


//数据查询部分
//查找酸的各级Ka(pKa)值
void searchKa_pKa(string acid,QTextEdit* t)
{
    int num = getnumofH(acid);
    if(num==0)
    {t->insertPlainText("提示:没有找到对应的Ka数据!\n");return;}

    double curKa;
    curKa=getK1(acid,true);
    if(num==1) {t->insertPlainText(QString("Ka=%1 pKa=%2\n")
                             .arg(curKa).arg(-log10(curKa)));}
    else {t->insertPlainText(QString("Ka1=%1 pKa1=%2\n")
                             .arg(curKa).arg(-log10(curKa)));}

    if(num>1)
    {curKa=getK2(acid,true);
    t->insertPlainText(QString("Ka2=%1 pKa2=%2\n").arg(curKa).arg(-log10(curKa)));
    }

    if(num>2)
    {curKa=getK3(acid);
    t->insertPlainText(QString("Ka3=%1 pKa3=%2\n").arg(curKa).arg(-log10(curKa)));
    }

    if(num>3)
    {curKa=getK4(acid);
    t->insertPlainText(QString("Ka4=%1 pKa4=%2\n").arg(curKa).arg(-log10(curKa)));
    }

}

//查找碱的各级Kb(pKb)值
void searchKb_pKb(string base,QTextEdit* t)
{
    int num = getCpofH(base);
    if(num==0)
    {t->insertPlainText("提示:没有找到对应的Kb数据!\n");return;}

    double curKb;
    curKb = getK1(base,false);
    if(num==1) {t->insertPlainText(QString("Kb=%1 pKb=%2\n")
                             .arg(curKb).arg(-log10(curKb)));}
    else {t->insertPlainText(QString("Kb1=%1 pKb1=%2\n")
                             .arg(curKb).arg(-log10(curKb)));}

    if(num>1)
    {curKb=getK2(base,false);
    t->insertPlainText(QString("Kb2=%1 pKb2=%2\n").arg(curKb).arg(-log10(curKb)));
    }

}

//查找指示剂信息
void searchIndicator(QString idcname,QTextEdit* t)
{
        QString curN;
        QString a[3];
        QFile file("://Data/indicator.txt");
        QTextStream iofile1(&file);
        file.open(QIODevice::ReadOnly);

        iofile1>>curN;
        while(curN!=idcname)
        {iofile1>>curN;
        if(curN.isEmpty()==true) break;
        }

        if(curN==idcname)
        {
            for(int i=0;i<3;i++)
            iofile1>>a[i];//分别读入指示剂变色范围，颜色，pT
            t->insertPlainText(QString("指示剂%1 变色范围:%2 "
               "颜色变化:%3 pT值:%4\n").arg(idcname).arg(a[0]).arg(a[1]).arg(a[2]));
        }
        else
        {
            t->insertPlainText("提示:没有找到对应的指示剂!\n");
        }

        file.close();

}

//查找KMY
void searchKMY(QString metal,QTextEdit* t)
{
    QFile file("://Data/KMY.txt");
    file.open(QIODevice::ReadOnly);//以只读方式打开文件
    QTextStream iofile(&file);
    QString curN;//当前离子化学式
    double curK;//当前读取到K对数值
    iofile>>curN;
    while(metal!=curN)
    {
    iofile>>curN;
    if(curN.isEmpty()==true) break;
    }

    if(metal==curN)
    {iofile>>curK;
    t->insertPlainText(QString("该金属离子的KMY值为%1\n").arg(curK));
    }

    else {t->insertPlainText("提示:没有找到该金属离子的KMY值!\n");}

}

//查找电位值
void searchPotential(QString pair,bool type,QTextEdit* t)
{

    QString curEA;//储存当前读取到电对
    QString Eab;//储存读取到的电势
    QFile file;
    QTextStream iofile1(&file);


    if(type==true)//true:查找酸性条件下数据,false:查找碱性条件下数据
    {file.setFileName("://Data/EApotential.txt");}
    else {file.setFileName("://Data/EBpotential.txt");}

    file.open(QIODevice::ReadOnly);

    iofile1>>curEA;
    while(curEA!=pair)
    {iofile1>>curEA;
     if(curEA.isEmpty()==true) break;
    }

    if(curEA==pair)
    {
        iofile1>>Eab;//读入电势数据
    if(type==true) {t->insertPlainText("酸性条件下");}
    else {t->insertPlainText("碱性条件下");}
    t->insertPlainText(QString("电对%1的标准电极电位为%2V\n").arg(pair).arg(Eab));
    }

    else {t->insertPlainText("提示:没有找到相应电对的数据!\n");}


}
