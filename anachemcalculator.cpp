#include "anachemcalculator.h"
#include "ui_anachemcalculator.h"
#include "Needed.h"
#include <QString>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QCompleter>
#include <QStringList>
#include <QVariant>
#include <math.h>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"

AnaChemCalculator::AnaChemCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AnaChemCalculator)
{
    ui->setupUi(this);

    QAction *a[3],*b[2],*info;
    a[0]=new QAction(this);
    a[1]=new QAction(this);
    a[2]=new QAction(this);
    a[0]->setText("pH计算");
    a[1]->setText("分步准确滴定判断");
    a[2]->setText("终点误差计算");

    b[0]=new QAction(this);
    b[1]=new QAction(this);
    b[0]->setText("K'和终点误差计算");
    b[1]->setText("分步准确滴定判断");

    info=new QAction(this);
    info->setText("历史版本信息");

    //导入菜单栏"酸碱"的选项
    ui->menuHA->addAction(a[0]);
    ui->menuHA->addSeparator();
    ui->menuHA->addAction(a[1]);
    ui->menuHA->addSeparator();
    ui->menuHA->addAction(a[2]);

    //导入菜单栏"络合"的选项
    ui->menuHC->addAction(b[0]);
    ui->menuHC->addSeparator();
    ui->menuHC->addAction(b[1]);

    //导入菜单栏"关于Calcualtor"的选项
    ui->menuInfo->addAction(info);

    //设置说明Dialog
    connect(a[0],&QAction::triggered,
            [=]()
            {QMessageBox *mspoint = new QMessageBox(this);
            mspoint->setAttribute(Qt::WA_DeleteOnClose);
            mspoint->setWindowTitle("酸碱_pH计算说明");
            mspoint->setText("此版本的计算器水溶液pH计算模块提供多种计算模式，"
            "涵盖一元酸碱简单计算、缓冲溶液计算、多元酸碱计算、两性物质计算等多种功能。"
            "\n(注：此版本下多元酸碱仅支持一元近似计算，精确计算敬请期待后续版本。)\n"
            "您可通过选择pH计算页上的选项，并在提示的对应输入栏输入有效信息来进行您所需的计算。"
            "输入栏中设置了在不同选项下的常用可选项提示，您可通过输入所需内容的一部分来获取这些输入提示。\n"
            "在您选定某个选项时，只有与之相对应的输入区域会解锁，也只会进行相应的计算并在右侧输出框输出结果。\n"
            "祝使用愉快!");
            mspoint->setModal(false);
            mspoint->show();
            });

    connect(a[1],&QAction::triggered,
            [=]()
    {QMessageBox *mspoint = new QMessageBox(this);
        mspoint->setAttribute(Qt::WA_DeleteOnClose);
        mspoint->setWindowTitle("酸碱_分步准滴计算说明");
        mspoint->setText("此版本的计算器分步准滴计算模块支持多元弱酸、二元弱碱、混合一元弱酸、"
        "混合一元弱碱四种计算模式。\n"
        "您可通过选择“酸碱部分计算”->“分步准滴+终点误差”计算页左上部的选项，在提示的对应输入栏输入有效信息，并触发"
        "上部稍偏右的“Calculate”按钮，来进行您所需的计算。"
        "输入栏中设置了常用的弱酸、弱碱化学式提示，您可通过输入所需内容的一部分来获取这些输入提示。\n"
        "在您选定某个选项时，只有与之相对应的输入区域会解锁，也只会进行相应的计算并在右侧输出框输出结果。\n"
        "祝使用愉快!");
        mspoint->setModal(false);
        mspoint->show();

    });

    connect(a[2],&QAction::triggered,
            [=]()
    {QMessageBox *mspoint = new QMessageBox(this);
        mspoint->setAttribute(Qt::WA_DeleteOnClose);
        mspoint->setWindowTitle("酸碱_简单终点误差计算说明");
        mspoint->setText("受酸碱终点误差计算复杂度和多样性的限制，"
        "此版本的计算器酸碱滴定简单终点误差模块仅支持“强碱滴定一元弱酸”和“强酸一元弱碱”两种计算模式。\n"
        "您可通过选择“酸碱部分计算”->“分步准滴+终点误差”计算页左下部的选项，在提示的对应输入栏输入有效信息，"
        "并触发偏右下方的“Calculate”按钮，来进行您所需的计算。"
        "输入栏中设置了常用的一元弱酸、弱碱化学式提示，您可通过输入所需内容的一部分来获取这些输入提示。\n"
        "在您选定某个选项时，只有与之相对应的输入区域会解锁，也只会进行相应的计算并在右侧输出框输出结果。\n"
        "更复杂情境下的计算将会在后续版本中引入。祝使用愉快!");
        mspoint->setModal(false);
        mspoint->show();

    });

    connect(b[0],&QAction::triggered,
            [=]()
    {QMessageBox *mspoint = new QMessageBox(this);
        mspoint->setAttribute(Qt::WA_DeleteOnClose);
        mspoint->setWindowTitle("络合_K'值与终点误差计算说明");
        mspoint->setText("此版本的计算器提供了最多3种其他配体和3种共存离子状态下主反应K'值的计算。"
        "您可通过在“络合部分”->“K'值与终点误差计算”计算页在提示的对应输入栏输入主反应和"
        "副反应的有效信息来进行计算。EDTA浓度默认初始化为常用浓度0.02mol/L。"
        "对于络合效应和水解效应，此计算器额外提供了自定义模式，以满足特定的数据需求。"
        "( 注:由于络合反应体系的复杂性，其他配体与共存离子络合反应的判断和修正还未实现。尽请期待后续版本。)\n\n"
        "络合滴定终点误差模块支持EBT、XO、紫脲酸铵三种常见指示剂。此计算器默认设置下终点误差计算一项是关闭的。"
        "如需要计算，请将“终点误差计算”处的选项调为“ON”。如您有特定的指示剂需求或者没有查找到对应的pMt'数据，"
        "请选择“自定义”选项并输入对应pMt'值来进行计算。\n"
        "祝使用愉快!");
        mspoint->setModal(false);
        mspoint->show();

    });

    connect(b[1],&QAction::triggered,
            [=]()
    {QMessageBox *mspoint = new QMessageBox(this);
        mspoint->setAttribute(Qt::WA_DeleteOnClose);
        mspoint->setWindowTitle("络合_分步准滴计算说明");
        mspoint->setText("此版本的计算器支持最多4种金属离子共存体系的控制酸度分步准滴判断和掩蔽效果判断计算。"
        "默认设置下掩蔽效果的计算是关闭的。如需计算掩蔽效果，请更改被掩蔽离子对应的掩蔽剂选项，并输入掩蔽剂浓度、"
        "所用指示剂和计量点pH来进行掩蔽效果计算。\n\n"
        "需要指出的是，“[掩蔽剂](初始)”指的是由在滴定到对应计量点时过量的掩蔽剂浓度(不是分析浓度!)，"
        "根据滴定过程溶液稀释的倍数计算得到的“初始”浓度。例如用与Pb2+同浓度的EDTA滴定Pb2+和Zn2+时，"
        "用phen掩蔽Zn2+，若[phen]sp=c0，则需输入的[phen]sp=2c0。在使用掩蔽计算时还请注意这一点。\n"
        "( 注：此版本暂不支持自定义pMt'的掩蔽效果判断，敬请期待后续版本。)\n祝使用愉快!");
        mspoint->setModal(false);
        mspoint->show();

    });

    connect(info,&QAction::triggered,
            [=]()
    {QMessageBox *mspoint = new QMessageBox(this);
        mspoint->setAttribute(Qt::WA_DeleteOnClose);
        mspoint->setWindowTitle("历史版本信息");
        mspoint->setText("v1.0：引入了络合部分计算\n\n"
                         "v1.1：引入了弱酸、弱碱的pH计算功能\n"
                         "v1.1.1：新增了两性物质和缓冲溶液的pH计算功能\n\n"
                         "v1.2：引入了掩蔽计算和判断功能\n"
                         "v1.2.1：优化了络合部分计算K'的输出，使输出详细的副反应系数α\n\n"
                         "v1.3：引入了常见数据的查询功能\n\n"
                         "v1.4：引入了酸碱分步准滴判断和终点误差计算功能\n"
                         "v1.4.1：修正了络合部分金属离子重复可能导致的错误\n"
                         "v1.4.2(当前版本)：计入了络合分步准滴部分掩蔽剂对非目标掩蔽离子的影响");
        mspoint->show();

    });


    //链接按钮与对应面板
    connect(ui->WelcomePage,&QRadioButton::clicked,
            [=]()
            {ui->Wins->setCurrentWidget(ui->Welcome);}
            );
    connect(ui->A1c,&QRadioButton::clicked,
            [=]()
            {ui->Wins->setCurrentWidget(ui->A1);}
            );
    connect(ui->A2c,&QRadioButton::clicked,
            [=]()
            {ui->Wins->setCurrentWidget(ui->A2);}
            );
    connect(ui->C1c,&QRadioButton::clicked,
            [=]()
            {ui->Wins->setCurrentWidget(ui->C1);}
            );
    connect(ui->C2c,&QRadioButton::clicked,
            [=]()
            {ui->Wins->setCurrentWidget(ui->C2);}
            );

    //样式表设置
    ui->OptionWin->setStyleSheet("QWidget{background-color:white;}");

    //起始页数据查询初始化设置
    QStringList listForSearch;
    listForSearch<<"弱酸Ka <化学式>"<<"弱碱Kb <化学式>"<<"酸碱指示剂 <名称(中/英)>"
                <<"KMY <金属离子>"<<"标准电极电位(酸) <电对>"<<"标准电极电位(碱) <电对>";
    ui->SearchTypeBox->addItems(listForSearch);
    ui->SearchTypeBox->setCurrentIndex(0);

    //起始页数据查询可选项提示设置
    QStringList listok;
    listok<<"NH4+"<<"H3AsO2"<<"H3BO2"<<"HNCO"<<"HCN"<<"HF"
              <<"H2O2"<<"HBrO"<<"HClO"<<"HIO"<<"HNO2"<<"HSCN"<<"HCOOH"<<"CH3COOH"
              <<"C2H5COOH"<<"(CH3)2CHCOOH"<<"CH3(CH2)2COOH"<<"ClCH2COOH"<<"Cl2CHCOOH"
             <<"Cl3CCOOH"<<"CH3CH(OH)COOH"<<"H2N(CH2)4CH(NH2)COOH"<<"C6H5COOH"
             <<"C6H5OH"<<"C4H3OCOOH"<<"CH3COCH2COOH"<<"CH3COCH2COCH3"
             <<"CH2=CHCOOH"<<"HOC6H4COOH"<<"H3AsO4"<<"H2B4O7"<<"H2CO3"<<"H2CrO4"
            <<"H2S"<<"H3PO4"<<"H4P2O7"<<"H3PO3"<<"H2SiO3"<<"H2SO4"
            <<"H2SO3"<<"H2S2O3"<<"H2NCH2COOH"<<"H2C2O4"
            <<"HOOCCOOH"<<"o-C6H4(COOH)2"<<"m-C6H4(COOH)2"<<"p-C6H4(COOH)2"
           <<"HOOCCH=CHCOOH"<<"HOOCCH2CHOHCOOH"<<"HOOCCH2COOH";
    QCompleter *comok = new QCompleter(listok,this);
    comok->setCaseSensitivity(Qt::CaseInsensitive);
    ui->TestLine->setCompleter(comok);


    //络合部分2初始化设置
    //设置默认输入Ratio为1
    ui->M1RLine->setText("1");
    ui->M2RLine->setText("1");
    ui->M3RLine->setText("1");
    ui->M4RLine->setText("1");

    //提示金属离子可选项
    QStringList listM;
    listM<<"Ag+"<<"Al3+"<<"Ba2+"<<"Be2+"<<"Bi3+"<<"Ca2+"<<"Cd2+"<<"Co2+"<<"Co3+"
        <<"Cr3+"<<"Cu2+"<<"Fe3+"<<"Fe2+"<<"Ga3+"<<"Hg2+"<<"In3+"<<"Li+"<<"Mg2+"<<
          "Mn2+"<<"Na+"<<"Ni2+"<<"Pb2+"<<"Pd2+"<<"Sn2+"<<"Sr2+"<<"Th4+"<<"TiO2+"
       <<"Tl3+"<<"U4+"<<"VO2+"<<"Y3+"<<"Zn2+"<<"Zr4+";
    QCompleter *comR = new QCompleter(listM,this);
    comR->setCaseSensitivity(Qt::CaseInsensitive);
    ui->M1Line->setCompleter(comR);
    ui->M2Line->setCompleter(comR);
    ui->M3Line->setCompleter(comR);
    ui->M4Line->setCompleter(comR);

    //提示掩蔽配体可选项
    QStringList listL;
    listL<<"F"<<"Cl"<<"Br"<<"I"<<"CN"<<"SCN"<<"NH3"<<"S2O3"
        <<"py"<<"甘氨酸"<<"乳酸"<<"eda"<<"phen"<<"None";
    ui->C2ShelterBox1->addItems(listL);
    ui->C2ShelterBox2->addItems(listL);
    ui->C2ShelterBox3->addItems(listL);
    ui->C2ShelterBox4->addItems(listL);

    //掩蔽选项初始化
    ui->C2ShelterBox1->setCurrentIndex(13);
    ui->C2ShelterBox2->setCurrentIndex(13);
    ui->C2ShelterBox3->setCurrentIndex(13);
    ui->C2ShelterBox4->setCurrentIndex(13);
    ui->C2ShelterCLine1->setReadOnly(true);
    ui->C2ShelterCLine2->setReadOnly(true);
    ui->C2ShelterCLine3->setReadOnly(true);
    ui->C2ShelterCLine4->setReadOnly(true);
    ui->C2ShelterIDBox->addItem("EBT");
    ui->C2ShelterIDBox->addItem("XO");
    ui->C2ShelterIDBox->addItem("紫脲酸铵");
    ui->C2ShelterIDBox->setCurrentIndex(0);


    //络合部分1初始化设置
    //设置Y初始浓度为0.02
    ui->C1YcLine->setText("0.02");
    //提示金属离子可选项
    ui->C1MLine->setCompleter(comR);
    ui->C1N1nLine->setCompleter(comR);
    ui->C1N2nLine->setCompleter(comR);
    ui->C1N3nLine->setCompleter(comR);

    //添加控制是否计算终点误差的选项于对应控件中
    ui->C1IndicatorCon->addItem("ON");
    ui->C1IndicatorCon->addItem("OFF");
    ui->C1IndicatorCon->setCurrentIndex(1);

    //添加指示剂选项
    ui->C1IndicatorBox->addItem("EBT");
    ui->C1IndicatorBox->addItem("XO");
    ui->C1IndicatorBox->addItem("紫脲酸铵");
    ui->C1IndicatorBox->addItem("自定义");
    ui->C1IndicatorBox->setCurrentIndex(0);

    //设置指示剂默认不可选
    QVariant v0(0);
    ui->C1IndicatorBox->setItemData(0,v0,Qt::UserRole - 1);
    ui->C1IndicatorBox->setItemData(1,v0,Qt::UserRole - 1);
    ui->C1IndicatorBox->setItemData(2,v0,Qt::UserRole - 1);
    ui->C1IndicatorBox->setItemData(3,v0,Qt::UserRole - 1);
    ui->C1IndicatorDLine->setReadOnly(true);
    //设置不可选选项变为灰色
    ui->C1IndicatorBox->setStyleSheet("QComboBox QAbstractItemView::item::!enabled{"
                                      "color:rgb(125,125,125);}");

    //提示配体可选项
    QString defstr("自定义");
    ui->C1L1Box->addItems(listL);
    ui->C1L1Box->addItem(defstr);
    ui->C1L2Box->addItems(listL);
    ui->C1L2Box->addItem(defstr);
    ui->C1L3Box->addItems(listL);
    ui->C1L3Box->addItem(defstr);
    ui->C1L1Box->setCurrentIndex(13);
    ui->C1L2Box->setCurrentIndex(13);
    ui->C1L3Box->setCurrentIndex(13);


    //酸碱部分1初始化设置
    //设置pH计算可选项
    QStringList listA1Ch;
    listA1Ch<<"一元弱酸"<<"一元弱碱"<<"多元弱酸"<<"多元弱碱"<<"两性物质1"<<"两性物质2"
           <<"缓冲溶液";
    ui->A1pHcom->addItems(listA1Ch);
    ui->A1pHcom->setCurrentIndex(0);

    //设置缓冲溶液计算可选项
    QStringList listA1B;
    listA1B<<"共轭酸"<<"共轭碱";
    ui->A1BufferAorBCom->addItems(listA1B);
    ui->A1BufferAorBCom->setCurrentIndex(0);

    //设置未选的其他计算选项不可操作
    ui->A1TwoInSNLine->setReadOnly(true);
    ui->A1TwoInSCLine->setReadOnly(true);
    ui->A1TwoSCaNLine->setReadOnly(true);
    ui->A1TwoSCaCNLine->setReadOnly(true);
    ui->A1TwoSCacLine->setReadOnly(true);
    ui->A1TwoSCbcLine->setReadOnly(true);
    ui->A1BufferAorBCom->setItemData(0,v0,Qt::UserRole -1);
    ui->A1BufferAorBCom->setItemData(1,v0,Qt::UserRole -1);
    ui->A1BufferAorBLine->setReadOnly(true);
    ui->A1BufferAcidCLine->setReadOnly(true);
    ui->A1BufferBaseCLine->setReadOnly(true);

    //一元/多元酸部分
    //提示可选项:一元弱酸,一元弱碱,多元弱酸,多元弱碱
    QStringList listWaS;
    listWaS<<"NH4+"<<"H3AsO2"<<"H3BO2"<<"HNCO"<<"HCN"<<"HF"
          <<"H2O2"<<"HBrO"<<"HClO"<<"HIO"<<"HNO2"<<"HSCN"<<"HCOOH"<<"CH3COOH"
          <<"C2H5COOH"<<"(CH3)2CHCOOH"<<"CH3(CH2)2COOH"<<"ClCH2COOH"<<"Cl2CHCOOH"
         <<"Cl3CCOOH"<<"CH3CH(OH)COOH"<<"H2N(CH2)4CH(NH2)COOH"<<"C6H5COOH"
         <<"C6H5OH"<<"C4H3OCOOH"<<"CH3COCH2COOH"<<"CH3COCH2COCH3"
         <<"CH2=CHCOOH"<<"HOC6H4COOH";
    QCompleter *comA1W = new QCompleter(listWaS,this);
    comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    ui->A1WabNLine->setCompleter(comA1W);


    //两性离子1部分
    //提示两性离子1可选项
    QStringList listA1AS;
    listA1AS<<"HCO3-"<<"H2PO4-"<<"HPO4_2-"<<"HSO3-"<<"HC2O4-"<<"HS-"<<
              "H2AsO4-"<<"HAsO4_2-"<<"H2PO3-"<<"HB4O7-";
    QCompleter *comA1AS = new QCompleter(listA1AS,this);
    comA1AS->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    ui->A1TwoInSNLine->setCompleter(comA1AS);

    //两性离子2部分
    //提示两性离子2中酸的可选项
    QStringList listA1ASa;
    listA1ASa<<"NH4+"<<"NH3NH2+"<<"NH3OH+";
    QCompleter *comA1ASa = new QCompleter(listA1ASa,this);
    comA1AS->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    ui->A1TwoSCaNLine->setCompleter(comA1ASa);


    //酸碱部分2初始化设置
    //设置准滴部分计算类型可选项
    QStringList listA2;
    listA2<<"多元弱酸"<<"二元弱碱"<<"混合弱酸"<<"混合弱碱";
    ui->A2TypeBox->addItems(listA2);
    ui->A2TypeBox->setCurrentIndex(0);

    //设置准滴部分初始提示可选项
    QStringList listWaM;
    listWaM<<"H3AsO4"<<"H2B4O7"<<"H2CO3"<<"H2CrO4"<<"H2S"<<"H3PO4"<<"H4P2O7"
           <<"H3PO3"<<"H2SiO3"<<"H2SO4"<<"H2SO3"<<"H2S2O3"<<"H2NCH2COOH"<<"H2C2O4"
          <<"HOOCCOOH"<<"o-C6H4(COOH)2"<<"m-C6H4(COOH)2"<<"p-C6H4(COOH)2"
           <<"HOOCCH=CHCOOH"<<"HOOCCH2CHOHCOOH"<<"HOOCCH2COOH";

    QCompleter *comA2 = new QCompleter(listWaM,this);
    comA2->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    ui->A2AorBNLine->setCompleter(comA2);

    //设置终点误差计算类型和所用指示剂可选项
    QStringList listTE,listIdc;
    listTE<<"强碱->一元弱酸"<<"强酸->一元弱碱";
    listIdc<<"百里酚蓝TB(第1次变色)"<<"百里酚蓝TB(第2次变色)"<<"甲基黄MY"<<"甲基橙MO"
           <<"溴百里酚蓝BTB"<<"中性红NR"<<"酚红PR"<<"酚酞PP"<<"百里酚酞TP"
           <<"溴酚蓝BPB"<<"溴甲酚绿BCG"<<"甲基红MR"<<"溴甲酚紫BCP";
    ui->A2IndicatorBox->addItems(listIdc);
    ui->A2TEModeBox->addItems(listTE);
    ui->A2IndicatorBox->setCurrentIndex(7);
    ui->A2TEModeBox->setCurrentIndex(0);
    //提示输入化学式可选项
    ui->A2TNLine->setCompleter(comA1W);

}

AnaChemCalculator::~AnaChemCalculator()
{
    delete ui;
}

//关闭窗口时作提示
void AnaChemCalculator::closeEvent(QCloseEvent *event)
{
    int dec = QMessageBox::question(this,"Question","确定关闭吗？");
    if(dec == QMessageBox::Yes)
    {
        event->accept();
    }
    else event->ignore();

};

//起始页数据查询部分
//数据查询具体操作+输出
void AnaChemCalculator::on_Testbutton_pressed()
{
    if(ui->TestLine->text().isEmpty()==true)
    {ui->Testoutput->insertPlainText("提示:没有输入任何信息,无法查询!\n");return;}

    QString data = ui->TestLine->text();
    int index = ui->SearchTypeBox->currentIndex();

    if(index==0)
    {searchKa_pKa(data.toStdString(),ui->Testoutput);
    }

    if(index==1)
    {searchKb_pKb(data.toStdString(),ui->Testoutput);
    }

    if(index==2)
    {searchIndicator(data,ui->Testoutput);
    }

    if(index==3)
    {searchKMY(data,ui->Testoutput);
    }

    if(index==4)
    {searchPotential(data,true,ui->Testoutput);
    }

    if(index==5)
    {searchPotential(data,false,ui->Testoutput);
    }

}

//设置数据可查项提示
void AnaChemCalculator::on_SearchTypeBox_currentIndexChanged(int index)
{
    QStringList listok;
    if(index==0)
    {listok<<"NH4+"<<"H3AsO2"<<"H3BO2"<<"HNCO"<<"HCN"<<"HF"
          <<"H2O2"<<"HBrO"<<"HClO"<<"HIO"<<"HNO2"<<"HSCN"<<"HCOOH"<<"CH3COOH"
          <<"C2H5COOH"<<"(CH3)2CHCOOH"<<"CH3(CH2)2COOH"<<"ClCH2COOH"<<"Cl2CHCOOH"
         <<"Cl3CCOOH"<<"CH3CH(OH)COOH"<<"H2N(CH2)4CH(NH2)COOH"<<"C6H5COOH"
         <<"C6H5OH"<<"C4H3OCOOH"<<"CH3COCH2COOH"<<"CH3COCH2COCH3"
         <<"CH2=CHCOOH"<<"HOC6H4COOH"<<"H3AsO4"<<"H2B4O7"<<"H2CO3"<<"H2CrO4"
        <<"H2S"<<"H3PO4"<<"H4P2O7"<<"H3PO3"<<"H2SiO3"<<"H2SO4"
        <<"H2SO3"<<"H2S2O3"<<"H2NCH2COOH"<<"H2C2O4"<<"HAc"
        <<"HOOCCOOH"<<"o-C6H4(COOH)2"<<"m-C6H4(COOH)2"<<"p-C6H4(COOH)2"
       <<"HOOCCH=CHCOOH"<<"HOOCCH2CHOHCOOH"<<"HOOCCH2COOH";
    QCompleter *com = new QCompleter(listok,this);
    com->setCaseSensitivity(Qt::CaseInsensitive);
    ui->TestLine->setCompleter(com);
    }

    if(index==1)
    {listok<<"NH3"<<"NH2OH"<<"CH3CONH2"<<"CH3(CH2)4NH2"<<"C6H5NH2"<<"o-H2NC5H4OCH3"
          <<"m-H2NC5H4OCH3"<<"p-H2NC5H4OCH3"<<"(C2H5)2NH"<<"(CH3)2NH"<<"Ph2NH"
          <<"H2NC2H4OH"<<"C2H5NH2"<<"CH3(CH2)5NH2"<<"(CH2)6N4"<<"CH3NH2"<<"NH2NH2"
          <<"Gln(谷氨酰胺)"<<"H2NCH2CH2NH2"<<"eda"<<"H2N(CH2)6NH2"<<"(H2NC6H4)2"
          <<"Ac-"<<"CH3COO-";
        QCompleter *com = new QCompleter(listok,this);
        com->setCaseSensitivity(Qt::CaseInsensitive);
        ui->TestLine->setCompleter(com);
    }

    if(index==2)
    {listok<<"百里酚蓝(第1次变色)"<<"百里酚蓝(第2次变色)"<<"TB(第1次变色)"<<"TB(第2次变色)"
          <<"甲基黄"<<"MY"<<"甲基橙"<<"MO"<<"溴百里酚蓝"
          <<"BTB"<<"中性红"<<"NR"<<"酚红"<<"PR"
          <<"酚酞"<<"PP"<<"百里酚酞"<<"TP"<<"溴酚蓝"<<"BPB"
            <<"溴甲酚绿"<<"BCG"<<"甲基红"<<"MR"<<"溴甲酚紫"<<"BCP";
        QCompleter *com = new QCompleter(listok,this);
        com->setCaseSensitivity(Qt::CaseInsensitive);
        ui->TestLine->setCompleter(com);
    }

    if(index==3)
    {listok<<"Ag+"<<"Al3+"<<"Ba2+"<<"Be2+"<<"Bi3+"<<"Ca2+"<<"Cd2+"<<"Co2+"<<"Co3+"
          <<"Cr3+"<<"Cu2+"<<"Fe3+"<<"Fe2+"<<"Ga3+"<<"Hg2+"<<"In3+"<<"Li+"<<"Mg2+"<<
            "Mn2+"<<"Na+"<<"Ni2+"<<"Pb2+"<<"Pd2+"<<"Sn2+"<<"Sr2+"<<"Th4+"<<"TiO2+"
         <<"Tl3+"<<"U4+"<<"VO2+"<<"Y3+"<<"Zn2+"<<"Zr4+";
        QCompleter *com = new QCompleter(listok,this);
        com->setCaseSensitivity(Qt::CaseInsensitive);
        ui->TestLine->setCompleter(com);
    }

    if(index==4)
    {listok<<"AgBr/Ag"<<"AgCl/Ag"<<"Ag2CrO4/Ag"<<"Ag+/Ag"
          <<"Al3+/Al"<<"HAsO2/As"<<"H3AsO4/HAsO2"<<"BiOCl/Bi"
         <<"BiO+/Bi"<<"Br2/Br-"<<"BrO3-/Br2"<<"Ca2+/Ca"
        <<"ClO4-/ClO3-"<<"Cl2/Cl-"<<"ClO3-/Cl-"<<"ClO3-/Cl2"
        <<"HClO/Cl2"<<"ClO3-/HClO2"<<"ClO2/HClO2"<<"HClO2/HClO"
        <<"Co3+/Co2+"<<"Cr2O72-/Cr3+"<<"Cu2+/Cu+"<<"Cu2+/Cu"
        <<"Cu+/Cu"<<"Fe2+/Fe"<<"Fe3+/Fe2+"<<"H+/H2"<<"Hg2Cl2/Hg"
        <<"Hg2+/Hg"<<"I2/I-"<<"IO3-/I2"<<"HIO/I2"<<"K+/K"<<"Mg2+/Mg"
        <<"Mn2+/Mn"<<"MnO4-/Mn2+"<<"MnO2/Mn2+"<<"Na+/Na"<<"NO3-/NO"
        <<"NO3-/NO2"<<"HNO2/NO"<<"NO2/NO"<<"NO3-/HNO2"<<"NO2/HNO2"
        <<"O2/H2O2"<<"H2O2/H2O"<<"O2/H20"<<"H3PO4/H3PO3"<<"Pb2+/Pb"
          <<"PbSO4/Pb"<<"PbO2/Pb2+"<<"H2SO3/S"<<"S/S2-"<<"Sn4+/Sn2+"<<"Zn2+/Zn";
        QCompleter *com = new QCompleter(listok,this);
        com->setCaseSensitivity(Qt::CaseInsensitive);
        ui->TestLine->setCompleter(com);
    }

    if(index==5)
    {listok<<"Ag2S/Ag"<<"Ag2O/Ag"<<"Al(OH)4-/Al"<<"AsO2-/As"
          <<"BrO3-/Br-"<<"BrO-/Br-"<<"ClO4-/ClO3-"<<"ClO3-/ClO2-"<<"ClO2-/ClO-"
         <<"ClO-/Cl-"<<"Co(OH)2/Co"<<"Co(OH)3/Co(OH)2"<<"Cr(OH)3/Cr"
        <<"CrO2-/Cr"<<"CrO4-/Cr(OH)3"<<"Cu2O/Cu"<<"Fe(OH)3/Fe(OH)2"
        <<"H2O/H2"<<"HgO/Hg"<<"IO3-/I-"<<"IO-/I-"<<"Mg(OH)2/Mg"
          <<"Mn(OH)2/Mn"<<"MnO4-/MnO2"<<"NO3-/NO2-"<<"O2/OH-"
         <<"S/S2-"<<"HSnO2-/Sn";
        QCompleter *com = new QCompleter(listok,this);
        com->setCaseSensitivity(Qt::CaseInsensitive);
        ui->TestLine->setCompleter(com);
    }

}

//清除数据查询输入栏
void AnaChemCalculator::on_SearchResetL_pressed()
{
    ui->TestLine->clear();
}

//清除数据查询输出结果
void AnaChemCalculator::on_SearchReset_pressed()
{
    ui->Testoutput->clear();
}


//络合部分2
//络合部分2计算
void AnaChemCalculator::on_C2Calculate_pressed()
{

    //传入金属离子等所需信息
    QString *Mn = new QString[4];
    QString *Mc = new QString[4];
    QString *Mr = new QString[4];
    Mn[0] = ui->M1Line->text();
    Mn[1] = ui->M2Line->text();
    Mn[2] = ui->M3Line->text();
    Mn[3] = ui->M4Line->text();

    Mc[0] = ui->M1cline->text();
    Mc[1] = ui->M2cline->text();
    Mc[2] = ui->M3cline->text();
    Mc[3] = ui->M4cline->text();

    Mr[0] = ui->M1RLine->text();
    Mr[1] = ui->M2RLine->text();
    Mr[2] = ui->M3RLine->text();
    Mr[3] = ui->M4RLine->text();

    int n=0,con[4];//分别对应:有效数据组数,输入位置
    for(int i=0;i<4;i++)
    {
        if(Mn[i].isEmpty()==false&&Mc[i].isEmpty()==false)
        {con[n]=i;//储存对应输入位置
         n++;
        }
    }
    if(n==0) {ui->C2Outcome->insertPlainText("输入的有效数据过少,无法计算！\n");return;}  

    //进一步传入到最终存储数组
    QString *TMn = new QString[n];
    for(int i=0;i<n;i++)
    {
        TMn[i]=Mn[con[i]];//金属离子符号传入最终储存的字符串数组
    }
    delete []Mn;

    //输入相同金属离子时报错
    for(int i=0;i<n;i++)
    {for(int k=i+1;k<n;k++)
        {if(TMn[k]==TMn[i])
           {ui->C2Outcome->insertPlainText(QString("提示:行%1和行%2"
                      "的金属离子重复!计算错误!\n").arg(con[i]+1).arg(con[k]+1));
            return;}
        }

    }

    double *TMc = new double[n];
    for(int i=0;i<n;i++)
    {
        TMc[i]=Mc[con[i]].toDouble();//金属离子浓度传入最终储存的double数组
    }
    delete []Mc;

    double *TMr = new double[n];
    for(int i=0;i<n;i++)
    {
        if(Mr[con[i]].isEmpty()==true)//修正:空字符串默认为1
        {TMr[i]=1;}
        else TMr[i]=Mr[con[i]].toDouble();//金属离子与Y浓度比传入
    }
    delete []Mr;

    //数据储存到MainR数组中(未排序)
    MainR *mpoint = new MainR[n];
    for(int i=0;i<n;i++)
    {
        mpoint[i].Modify(TMn[i],TMc[i],TMr[i]);
    }
    delete []TMn;
    delete []TMc;
    delete []TMr;

    //掩蔽处理判断
    bool *isShelterf = new bool[n];
    int *ShIndex = new int[n];
    double *cShelterf = new double[n];
    //初始化掩蔽剂浓度为0
    for(int i=0;i<n;i++)
    {cShelterf[i]=0;}
    //初始化掩蔽选项页码为13(即None,不掩蔽)
    for(int i=0;i<n;i++)
    {ShIndex[i]=13;}

    //判断是否掩蔽
    for(int i=0;i<n;i++)
    {
        isShelterf[i]=false;
        if(con[i]==0)
        {int index= ui->C2ShelterBox1->currentIndex();
            if(index<=12)
            {   if(ui->C2ShelterCLine1->text().isEmpty()==true)
                {ui->C2Outcome->insertPlainText("提示:Line1没有输入掩蔽剂"
                                                "浓度,将不作掩蔽处理!\n");
                }
                else
                {   isShelterf[i]=true;
                    ShIndex[i]=index;
                    cShelterf[i]=ui->C2ShelterCLine1->text().toDouble();
            }
            }
        }

        if(con[i]==1)
        {int index= ui->C2ShelterBox2->currentIndex();
            if(index<=12)
            {   if(ui->C2ShelterCLine2->text().isEmpty()==true)
                {ui->C2Outcome->insertPlainText("提示:Line2没有输入掩蔽剂"
                                                "浓度,将不作掩蔽处理!\n");
                }
                else
                {   isShelterf[i]=true;
                    ShIndex[i]=index;
                    cShelterf[i]=ui->C2ShelterCLine2->text().toDouble();
            }
            }
        }

        if(con[i]==2)
        {int index= ui->C2ShelterBox3->currentIndex();
            if(index<=12)
            {   if(ui->C2ShelterCLine3->text().isEmpty()==true)
                {ui->C2Outcome->insertPlainText("提示:Line3没有输入掩蔽剂"
                                                "浓度,将不作掩蔽处理!\n");
                }
                else
                {   isShelterf[i]=true;
                    ShIndex[i]=index;
                    cShelterf[i]=ui->C2ShelterCLine3->text().toDouble();
            }
            }
        }

        if(con[i]==3)
        {int index= ui->C2ShelterBox4->currentIndex();
            if(index<=12)
            {   if(ui->C2ShelterCLine4->text().isEmpty()==true)
                {ui->C2Outcome->insertPlainText("提示:Line4没有输入掩蔽剂"
                                                "浓度,将不作掩蔽处理!\n");
                }
                else
                {   isShelterf[i]=true;
                    ShIndex[i]=index;
                    cShelterf[i]=ui->C2ShelterCLine4->text().toDouble();
            }
            }
        }
    }




    //数据排序处理,再储存
    bool *isShelter = new bool[n];
    double *cShelter = new double[n];
    int *TShindex = new int[n];
    MainR *mTpoint = new MainR[n];
    int curMax,noshelterP=0;//分别对应当前最大位置,没有掩蔽的最小位置
    for(int i=0;i<n;i++)
    {if(isShelterf[i]==false) {break;}
    noshelterP++;
    }

    for(int i=0;i<n;i++)
    {curMax=noshelterP;

        for(int k=noshelterP;k<n;k++)
        {    if(mpoint[k]>mpoint[curMax]&&isShelterf[k]==false)
             {curMax=k;}
        }

        if(mpoint[curMax].GetlgK()==0)
        {for(int k=0;k<n;k++)
            {if(isShelterf[k]==true&&mpoint[k].GetlgK()>0)
                {curMax=k;}

            }
        }

    mTpoint[i]=mpoint[curMax];
    isShelter[i]=isShelterf[curMax];
    cShelter[i]=cShelterf[curMax];
    TShindex[i]=ShIndex[curMax];
    mpoint[curMax].setlgK(0);

    }
    delete []mpoint;
    delete []isShelterf;
    delete []cShelterf;
    delete []ShIndex;

    //引入pH和指示剂选项以进行掩蔽效果评价
    double pH=-1;
    int idcIndex=-1;
    if(ui->C2ShelterpHLine->text().isEmpty()==true
       &&(ui->C2ShelterBox1->currentIndex()!=13
          ||ui->C2ShelterBox2->currentIndex()!=13
          ||ui->C2ShelterBox3->currentIndex()!=13
          ||ui->C2ShelterBox4->currentIndex()!=13)
            )
    {ui->C2Outcome->insertPlainText("提示:没有设定pH,无法作进一步掩蔽效果评价!\n");}
    else
    {pH=ui->C2ShelterpHLine->text().toDouble();
    idcIndex=ui->C2ShelterIDBox->currentIndex();
    }

    ATJ(mTpoint,n,ui->C2Outcome,isShelter,cShelter,TShindex,pH,idcIndex);//判断是否能分步准确滴定
    ui->C2Outcome->insertPlainText("\n");
    delete []mTpoint;
    delete []isShelter;
    delete []cShelter;
    delete []TShindex;

}

//设置络合部分2有选择配体时解除浓度输入禁止
void AnaChemCalculator::on_C2ShelterBox1_currentIndexChanged(int index)
{
    if(index<=12)
    {ui->C2ShelterCLine1->setReadOnly(false);
    }
    else {ui->C2ShelterCLine1->setReadOnly(true);}


}

void AnaChemCalculator::on_C2ShelterBox2_currentIndexChanged(int index)
{
    if(index<=12)
    {ui->C2ShelterCLine2->setReadOnly(false);
    }
    else {ui->C2ShelterCLine2->setReadOnly(true);}
}

void AnaChemCalculator::on_C2ShelterBox3_currentIndexChanged(int index)
{
    if(index<=12)
    {ui->C2ShelterCLine3->setReadOnly(false);
    }
    else {ui->C2ShelterCLine3->setReadOnly(true);}
}

void AnaChemCalculator::on_C2ShelterBox4_currentIndexChanged(int index)
{
    if(index<=12)
    {ui->C2ShelterCLine4->setReadOnly(false);
    }
    else {ui->C2ShelterCLine4->setReadOnly(true);}
}

//络合部分2:Reset All
void AnaChemCalculator::on_C2Reset_pressed()
{
    //重置浓度比
    ui->M1RLine->setText("1");
    ui->M2RLine->setText("1");
    ui->M3RLine->setText("1");
    ui->M4RLine->setText("1");
    //清除化学式和浓度数据
    ui->M1Line->clear();
    ui->M2Line->clear();
    ui->M3Line->clear();
    ui->M4Line->clear();
    ui->M1cline->clear();
    ui->M2cline->clear();
    ui->M3cline->clear();
    ui->M4cline->clear();
    //重置掩蔽计算选项
    ui->C2ShelterBox1->setCurrentIndex(13);
    ui->C2ShelterBox2->setCurrentIndex(13);
    ui->C2ShelterBox3->setCurrentIndex(13);
    ui->C2ShelterBox4->setCurrentIndex(13);
    ui->C2ShelterCLine1->clear();
    ui->C2ShelterCLine2->clear();
    ui->C2ShelterCLine3->clear();
    ui->C2ShelterCLine4->clear();
    ui->C2ShelterCLine1->setReadOnly(true);
    ui->C2ShelterCLine2->setReadOnly(true);
    ui->C2ShelterCLine3->setReadOnly(true);
    ui->C2ShelterCLine4->setReadOnly(true);
    //清除输出
    ui->C2Outcome->clear();


}

//络合部分2重置掩蔽效果部分
void AnaChemCalculator::on_C2ResetSh_pressed()
{
    ui->C2ShelterIDBox->setCurrentIndex(0);
    ui->C2ShelterpHLine->clear();
}

//络合部分2重置输出
void AnaChemCalculator::on_C2ResetOP_pressed()
{
     ui->C2Outcome->clear();
}

//络合部分2重置各行
void AnaChemCalculator::on_C2ResetLine1_pressed()
{
    ui->M1Line->clear();
    ui->M1cline->clear();
    ui->M1RLine->setText("1");
    ui->C2ShelterBox1->setCurrentIndex(13);
    ui->C2ShelterCLine1->clear();
    ui->C2ShelterCLine1->setReadOnly(true);
}

void AnaChemCalculator::on_C2ResetLine2_pressed()
{
    ui->M2Line->clear();
    ui->M2cline->clear();
    ui->M2RLine->setText("1");
    ui->C2ShelterBox2->setCurrentIndex(13);
    ui->C2ShelterCLine2->clear();
    ui->C2ShelterCLine2->setReadOnly(true);
}

void AnaChemCalculator::on_C2ResetLine3_pressed()
{
    ui->M3Line->clear();
    ui->M3cline->clear();
    ui->M3RLine->setText("1");
    ui->C2ShelterBox3->setCurrentIndex(13);
    ui->C2ShelterCLine3->clear();
    ui->C2ShelterCLine3->setReadOnly(true);
}

void AnaChemCalculator::on_C2ResetLine4_pressed()
{
    ui->M4Line->clear();
    ui->M4cline->clear();
    ui->M4RLine->setText("1");
    ui->C2ShelterBox4->setCurrentIndex(13);
    ui->C2ShelterCLine4->clear();
    ui->C2ShelterCLine4->setReadOnly(true);
}

//络合部分1
//实现指示剂是否可选择的调控
void AnaChemCalculator::on_C1IndicatorCon_currentIndexChanged(int index)
{
    if(index==0)
    {QVariant v(1|32);
        ui->C1IndicatorBox->setItemData(0,v,Qt::UserRole - 1);
        ui->C1IndicatorBox->setItemData(1,v,Qt::UserRole - 1);
        ui->C1IndicatorBox->setItemData(2,v,Qt::UserRole - 1);
        ui->C1IndicatorBox->setItemData(3,v,Qt::UserRole - 1);
        ui->C1IndicatorDLine->setReadOnly(false);}
    else
    {QVariant v(0);
        ui->C1IndicatorBox->setItemData(0,v,Qt::UserRole - 1);
        ui->C1IndicatorBox->setItemData(1,v,Qt::UserRole - 1);
        ui->C1IndicatorBox->setItemData(2,v,Qt::UserRole - 1);
        ui->C1IndicatorBox->setItemData(3,v,Qt::UserRole - 1);
        ui->C1IndicatorDLine->setReadOnly(true);}
}

//重置络合部分1
void AnaChemCalculator::on_C1ResetAll_pressed()
{
    //重置主反应部分
    ui->C1MLine->clear();
    ui->C1McLine->clear();
    ui->C1YcLine->setText("0.02");

    //重置pH和共存离子部分
    ui->C1pHLine->clear();
    ui->C1N1nLine->clear();
    ui->C1N2nLine->clear();
    ui->C1N3nLine->clear();
    ui->C1N1cLine->clear();
    ui->C1N2cLine->clear();
    ui->C1N3cLine->clear();
    ui->C1L1cLine->clear();
    ui->C1L2cLine->clear();
    ui->C1L3cLine->clear();

    //重置给定配体部分
    ui->C1L1Box->setoldIndex(ui->C1L1Box->currentIndex());
    ui->C1L2Box->setoldIndex(ui->C1L2Box->currentIndex());
    ui->C1L3Box->setoldIndex(ui->C1L3Box->currentIndex());
    ui->C1L1Box->setCurrentIndex(13);
    ui->C1L2Box->setCurrentIndex(13);
    ui->C1L3Box->setCurrentIndex(13);
    //修正选择过程中产生的不可选选项,避免不可逆破坏
    QVariant v(1|32);
    ui->C1L1Box->setItemData(ui->C1L2Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L1Box->setItemData(ui->C1L3Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L2Box->setItemData(ui->C1L1Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L2Box->setItemData(ui->C1L3Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L3Box->setItemData(ui->C1L1Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L3Box->setItemData(ui->C1L2Box->getoldIndex(),v,Qt::UserRole -1);

    //重置终点误差部分
    ui->C1IndicatorCon->setCurrentIndex(1);
    ui->C1IndicatorBox->setCurrentIndex(0);
    ui->C1IndicatorDLine->clear();

    //重置自定义配体部分
    ui->C1DL1betaLine1->clear();
    ui->C1DL1betaLine2->clear();
    ui->C1DL1betaLine3->clear();
    ui->C1DL1betaLine4->clear();
    ui->C1DL1betaLine5->clear();
    ui->C1DL1betaLine6->clear();
    ui->C1DL2betaLine1->clear();
    ui->C1DL2betaLine2->clear();
    ui->C1DL2betaLine3->clear();
    ui->C1DL2betaLine4->clear();
    ui->C1DL2betaLine5->clear();
    ui->C1DL2betaLine6->clear();
    ui->C1DL3betaLine1->clear();
    ui->C1DL3betaLine2->clear();
    ui->C1DL3betaLine3->clear();
    ui->C1DL3betaLine4->clear();
    ui->C1DL3betaLine5->clear();
    ui->C1DL3betaLine6->clear();

    ui->C1Outcome->clear();

}

//重置配体和共存离子
void AnaChemCalculator::on_C1ResetSR_pressed()
{
    ui->C1N1nLine->clear();
    ui->C1N2nLine->clear();
    ui->C1N3nLine->clear();
    ui->C1N1cLine->clear();
    ui->C1N2cLine->clear();
    ui->C1N3cLine->clear();
    ui->C1L1cLine->clear();
    ui->C1L2cLine->clear();
    ui->C1L3cLine->clear();

    ui->C1L1Box->setoldIndex(ui->C1L1Box->currentIndex());
    ui->C1L2Box->setoldIndex(ui->C1L2Box->currentIndex());
    ui->C1L3Box->setoldIndex(ui->C1L3Box->currentIndex());
    ui->C1L1Box->setCurrentIndex(13);
    ui->C1L2Box->setCurrentIndex(13);
    ui->C1L3Box->setCurrentIndex(13);
    QVariant v(1|32);
    ui->C1L1Box->setItemData(ui->C1L2Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L1Box->setItemData(ui->C1L3Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L2Box->setItemData(ui->C1L1Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L2Box->setItemData(ui->C1L3Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L3Box->setItemData(ui->C1L1Box->getoldIndex(),v,Qt::UserRole -1);
    ui->C1L3Box->setItemData(ui->C1L2Box->getoldIndex(),v,Qt::UserRole -1);


    ui->C1DL1betaLine1->clear();
    ui->C1DL1betaLine2->clear();
    ui->C1DL1betaLine3->clear();
    ui->C1DL1betaLine4->clear();
    ui->C1DL1betaLine5->clear();
    ui->C1DL1betaLine6->clear();
    ui->C1DL2betaLine1->clear();
    ui->C1DL2betaLine2->clear();
    ui->C1DL2betaLine3->clear();
    ui->C1DL2betaLine4->clear();
    ui->C1DL2betaLine5->clear();
    ui->C1DL2betaLine6->clear();
    ui->C1DL3betaLine1->clear();
    ui->C1DL3betaLine2->clear();
    ui->C1DL3betaLine3->clear();
    ui->C1DL3betaLine4->clear();
    ui->C1DL3betaLine5->clear();
    ui->C1DL3betaLine6->clear();
}

//重置输出
void AnaChemCalculator::on_C1ResetOutput_pressed()
{
    ui->C1Outcome->clear();
}

//重置终点误差部分
void AnaChemCalculator::on_C1ResetTE_pressed()
{
    ui->C1IndicatorCon->setCurrentIndex(1);
    ui->C1IndicatorBox->setCurrentIndex(0);
    ui->C1IndicatorDLine->clear();
}

//络合部分1计算
void AnaChemCalculator::on_C1Calculate_pressed()
{
    //1.条件稳定常数计算部分
    //获取主反应信息
    QString *mRpoint = new QString;
    *mRpoint = ui->C1MLine->text();

    //主反应中金属离子数据缺乏时报错
    if(mRpoint->isEmpty()==true||ui->C1McLine->text().isEmpty()==true)
    {
        ui->C1Outcome->insertPlainText("缺乏主反应金属离子数据,无法计算！\n");
        return;
    }

    double *ratio = new double;
    if(ui->C1McLine->text().isEmpty()==false&&ui->C1YcLine->text().isEmpty()==false)
    {*ratio=ui->C1McLine->text().toDouble()/ui->C1YcLine->text().toDouble();}
    else
    {ui->C1Outcome->insertPlainText("提示:输入的浓度数据不全,已默认浓度比为1\n");
    *ratio=1;
    }

    MainR mR(*mRpoint,ui->C1McLine->text().toDouble(),*ratio);
    delete ratio;

    //获取副反应信息
    //获取pH
    double pH;
    bool inpH;//表示是否考虑pH
    if(ui->C1pHLine->text().isEmpty()==true)
    {ui->C1Outcome->insertPlainText("提示:没有输入pH,将忽略酸效应和水解效应,"
                                    "终点误差也将无法使用给定指示剂计算!\n");
        pH=7;
        inpH=false;
    }
    else
    {pH = ui->C1pHLine->text().toDouble();
        inpH=true;
    }

    //获取共存离子和配体
    QString *Nname = new QString[3];
    QString *Nc = new QString[3];
    QString *Lc = new QString[3];
    int *Ln = new int[3];

    Nname[0] = ui->C1N1nLine->text();
    Nname[1] = ui->C1N2nLine->text();
    Nname[2] = ui->C1N3nLine->text();
    Nc[0] = ui->C1N1cLine->text();
    Nc[1] = ui->C1N2cLine->text();
    Nc[2] = ui->C1N3cLine->text();
    Lc[0] = ui->C1L1cLine->text();
    Lc[1] = ui->C1L2cLine->text();
    Lc[2] = ui->C1L3cLine->text();
    Ln[0] = ui->C1L1Box->currentIndex();
    Ln[1] = ui->C1L2Box->currentIndex();
    Ln[2] = ui->C1L3Box->currentIndex();

    //判断其中有效信息所在
    int *eN = new int[3],*eL = new int[3];//储存有效信息序号
    int curN=0,curL=0;
    for(int i=0;i<3;i++)
    {
        if(Nname[i].isEmpty()==false&&Nc[i].isEmpty()==false)
        {eN[curN]=i;
        curN++;}

        if(Lc[i].isEmpty()==false&&(Ln[i]<=12||Ln[i]==14))
        {eL[curL]=i;
         curL++;
        }

    }

    //共存离子与主反应离子相同时报错
    for(int i=0;i<curN;i++)
    {if(Nname[eN[i]]==mRpoint[0])
        {ui->C1Outcome->insertPlainText(QString("提示:共存离子%1和主反应离子相同"
                        ",计算出错!\n").arg(eN[i]+1));
            return;}

    }
    delete mRpoint;

    //共存离子彼此相同时报错
    for(int i=0;i<curN;i++)
    {for(int k=i+1;k<curN;k++)
        {if(Nname[eN[k]]==Nname[eN[i]])
            {ui->C1Outcome->insertPlainText(QString("提示:共存离子%1和共存离子%2"
                      "相同,计算出错!\n").arg(eN[i]+1).arg(eN[k]+1));
                return;
            }

        }

    }

    //计算aYN
    if(curN!=0)
    {otherM *AllN = new otherM[curN];
    for(int i=0;i<curN;i++)
    {
        AllN->name = Nname[eN[i]];
        AllN->c = Nc[eN[i]].toDouble();
    }
    mR.GetaYN(AllN,curN);
    }
    delete []Nname;
    delete []Nc;
    delete []eN;

    //计算aYH
    if(inpH==true)
    {mR.GetaYH(pH);}

    //计算aML
    for(int i=0;i<curL;i++)
    {if(Ln[eL[i]]<=12)
          {Ligand temL;
           temL.c = Lc[eL[i]].toDouble();
           temL.L = IntToLigand(Ln[eL[i]]);
           mR.GetaML(&temL,1,ui->C1Outcome);
          }
    if(Ln[eL[i]]==14)
          {double definedBeta[6]={0,0,0,0,0,0};
          if(eL[i]==0)
              { if(ui->C1DL1betaLine1->text().isEmpty()==false)
                {definedBeta[0]=ui->C1DL1betaLine1->text().toDouble();
                }

                if(ui->C1DL1betaLine2->text().isEmpty()==false)
                {definedBeta[1]=ui->C1DL1betaLine2->text().toDouble();
                }

                if(ui->C1DL1betaLine3->text().isEmpty()==false)
                {definedBeta[2]=ui->C1DL1betaLine3->text().toDouble();
                }

                if(ui->C1DL1betaLine4->text().isEmpty()==false)
                {definedBeta[3]=ui->C1DL1betaLine4->text().toDouble();
                }

                if(ui->C1DL1betaLine5->text().isEmpty()==false)
                {definedBeta[4]=ui->C1DL1betaLine5->text().toDouble();
                }

                if(ui->C1DL1betaLine6->text().isEmpty()==false)
                {definedBeta[5]=ui->C1DL1betaLine6->text().toDouble();
                }
              }
          if(eL[i]==1)
              { if(ui->C1DL2betaLine1->text().isEmpty()==false)
                {definedBeta[0]=ui->C1DL2betaLine1->text().toDouble();
                }

                if(ui->C1DL2betaLine2->text().isEmpty()==false)
                {definedBeta[1]=ui->C1DL2betaLine2->text().toDouble();
                }

                if(ui->C1DL2betaLine3->text().isEmpty()==false)
                {definedBeta[2]=ui->C1DL2betaLine3->text().toDouble();
                }

                if(ui->C1DL2betaLine4->text().isEmpty()==false)
                {definedBeta[3]=ui->C1DL2betaLine4->text().toDouble();
                }

                if(ui->C1DL2betaLine5->text().isEmpty()==false)
                {definedBeta[4]=ui->C1DL2betaLine5->text().toDouble();
                }

                if(ui->C1DL2betaLine6->text().isEmpty()==false)
                {definedBeta[5]=ui->C1DL2betaLine6->text().toDouble();
                }
              }
          if(eL[i]==2)
              { if(ui->C1DL3betaLine1->text().isEmpty()==false)
                {definedBeta[0]=ui->C1DL3betaLine1->text().toDouble();
                }

                if(ui->C1DL3betaLine2->text().isEmpty()==false)
                {definedBeta[1]=ui->C1DL3betaLine2->text().toDouble();
                }

                if(ui->C1DL3betaLine3->text().isEmpty()==false)
                {definedBeta[2]=ui->C1DL3betaLine3->text().toDouble();
                }

                if(ui->C1DL3betaLine4->text().isEmpty()==false)
                {definedBeta[3]=ui->C1DL3betaLine4->text().toDouble();
                }

                if(ui->C1DL3betaLine5->text().isEmpty()==false)
                {definedBeta[4]=ui->C1DL3betaLine5->text().toDouble();
                }

                if(ui->C1DL3betaLine6->text().isEmpty()==false)
                {definedBeta[5]=ui->C1DL3betaLine6->text().toDouble();
                }
              }
          mR.GetaMLDefined(definedBeta,Lc[eL[i]].toDouble());
          }
    }
    delete []Lc;
    delete []Ln;
    delete []eL;

    //计算aMOH
    if(inpH==true)
    {
     //判断是否有自定义,如有优先按照自定义计算
     bool aOHdefined=false;
     double definedBeta[6]={0,0,0,0,0,0};
     if(ui->C1DOHBetaLine1->text().isEmpty()==false)
     {definedBeta[0]=ui->C1DOHBetaLine1->text().toDouble();
         aOHdefined=true;}

     if(ui->C1DOHBetaLine2->text().isEmpty()==false)
     {definedBeta[1]=ui->C1DOHBetaLine2->text().toDouble();
         aOHdefined=true;}

     if(ui->C1DOHBetaLine3->text().isEmpty()==false)
     {definedBeta[2]=ui->C1DOHBetaLine3->text().toDouble();
         aOHdefined=true;}

     if(ui->C1DOHBetaLine4->text().isEmpty()==false)
     {definedBeta[3]=ui->C1DOHBetaLine4->text().toDouble();
         aOHdefined=true;}

     if(ui->C1DOHBetaLine5->text().isEmpty()==false)
     {definedBeta[4]=ui->C1DOHBetaLine5->text().toDouble();
         aOHdefined=true;}

     if(ui->C1DOHBetaLine6->text().isEmpty()==false)
     {definedBeta[5]=ui->C1DOHBetaLine6->text().toDouble();
         aOHdefined=true;}

     if(aOHdefined==true)
     {mR.GetaMOHDefined(definedBeta,pH);}

     if(aOHdefined==false)
     {mR.GetaMOH(pH,ui->C1Outcome);}
    }

    //计算条件稳定常数K'
    mR.CalculateK_();

    //输出计算得到的各项信息
    mR.Putout(ui->C1Outcome);
    ui->C1Outcome->insertPlainText("\n");


    //2.终点误差计算部分
    //判断是否需要计算终点误差
    if(ui->C1IndicatorCon->currentIndex()==1)
    {return;}

    //自定义pMt'值时对应计算
    int idcn = ui->C1IndicatorBox->currentIndex();
    if(idcn==3)
    {
        if(ui->C1IndicatorDLine->text().isEmpty()==true)
        {
            ui->C1Outcome->insertPlainText("提示:缺乏自定义指示剂的pMt'值,无法计算TE!\n\n");
            return;
        }
        double pMt = ui->C1IndicatorDLine->text().toDouble();
        mR.GetTE(pMt,ui->C1Outcome);
        return;
    }

    //采用默认指示剂时对应计算
    if(inpH==false) {ui->C1Outcome->insertPlainText("提示:没有设置pH,无法按照"
                                                    "给定指示剂计算TE!\n\n");return;}
    indicator idc=EBT;
    switch(idcn)
    {case 0: idc=EBT;break;
     case 1: idc=XO;break;
     case 2: idc=indicator::MRX;break;
    }
    mR.GetTE(pH,idc,ui->C1Outcome);
    ui->C1Outcome->insertPlainText("\n");




}

//设置络合部分1不可多次选择同一给定配体(用到自定义MyComboBox中的数据oldIndex)
void AnaChemCalculator::on_C1L1Box_currentIndexChanged(int index)
{
    if(index<=12)
    {QVariant v(0);
        ui->C1L2Box->setItemData(index,v,Qt::UserRole -1);
        ui->C1L3Box->setItemData(index,v,Qt::UserRole -1);
    }
    int old = ui->C1L1Box->getoldIndex();
    if(old<=12)
    {QVariant v(1|32);
    ui->C1L2Box->setItemData(old,v,Qt::UserRole -1);
    ui->C1L3Box->setItemData(old,v,Qt::UserRole -1);
    }

}

void AnaChemCalculator::on_C1L2Box_currentIndexChanged(int index)
{
    if(index<=12)
    {QVariant v(0);
        ui->C1L1Box->setItemData(index,v,Qt::UserRole -1);
        ui->C1L3Box->setItemData(index,v,Qt::UserRole -1);
    }
    int old = ui->C1L2Box->getoldIndex();
    if(old<=12)
    {QVariant v(1|32);
    ui->C1L1Box->setItemData(old,v,Qt::UserRole -1);
    ui->C1L3Box->setItemData(old,v,Qt::UserRole -1);
    }

}

void AnaChemCalculator::on_C1L3Box_currentIndexChanged(int index)
{
    if(index<=12)
    {QVariant v(0);
        ui->C1L1Box->setItemData(index,v,Qt::UserRole -1);
        ui->C1L2Box->setItemData(index,v,Qt::UserRole -1);
    }
    int old = ui->C1L3Box->getoldIndex();
    if(old<=12)
    {QVariant v(1|32);
    ui->C1L1Box->setItemData(old,v,Qt::UserRole -1);
    ui->C1L2Box->setItemData(old,v,Qt::UserRole -1);
    }

}

//酸碱部分1
//设置选择不同pH选项选择激活对应输入区域
void AnaChemCalculator::on_A1pHcom_currentIndexChanged(int index)
{
    if(index<=3)
    {ui->A1WabNLine->setReadOnly(false);
     ui->A1WabCLine->setReadOnly(false);

     ui->A1TwoInSNLine->setReadOnly(true);
     ui->A1TwoInSCLine->setReadOnly(true);
     ui->A1TwoSCaNLine->setReadOnly(true);
     ui->A1TwoSCaCNLine->setReadOnly(true);
     ui->A1TwoSCacLine->setReadOnly(true);
     ui->A1TwoSCbcLine->setReadOnly(true);
     ui->A1BufferAorBLine->setReadOnly(true);
     ui->A1BufferAcidCLine->setReadOnly(true);
     ui->A1BufferBaseCLine->setReadOnly(true);
     QVariant v(0);
     ui->A1BufferAorBCom->setItemData(0,v,Qt::UserRole -1);
     ui->A1BufferAorBCom->setItemData(1,v,Qt::UserRole -1);

     //根据选择功能提示物质可选项
     if(index==0)
     {QStringList listWaS;
         listWaS<<"NH4+"<<"H3AsO2"<<"H3BO2"<<"HNCO"<<"HCN"<<"HF"
               <<"H2O2"<<"HBrO"<<"HClO"<<"HIO"<<"HNO2"<<"HSCN"<<"HCOOH"<<"CH3COOH"
               <<"C2H5COOH"<<"(CH3)2CHCOOH"<<"CH3(CH2)2COOH"<<"ClCH2COOH"<<"Cl2CHCOOH"
              <<"Cl3CCOOH"<<"CH3CH(OH)COOH"<<"H2N(CH2)4CH(NH2)COOH"<<"C6H5COOH"
              <<"C6H5OH"<<"C4H3OCOOH"<<"CH3COCH2COOH"<<"CH3COCH2COCH3"
              <<"CH2=CHCOOH"<<"HOC6H4COOH"<<"HAc";
         QCompleter *comA1W = new QCompleter(listWaS,this);
         comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
         ui->A1WabNLine->setCompleter(comA1W);
     }

     if(index==1)
     {QStringList listWbS;
         listWbS<<"NH3"<<"NH2OH"<<"CH3CONH2"<<"CH3(CH2)4NH2"<<"C6H5NH2"<<"o-H2NC5H4OCH3"
            <<"m-H2NC5H4OCH3"<<"p-H2NC5H4OCH3"<<"(C2H5)2NH"<<"(CH3)2NH"<<"Ph2NH"
           <<"H2NC2H4OH"<<"C2H5NH2"<<"CH3(CH2)5NH2"<<"(CH2)6N4"<<"CH3NH2"<<"Ac-"<<"CH3COO-";
         QCompleter *comA1W = new QCompleter(listWbS,this);
         comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
         ui->A1WabNLine->setCompleter(comA1W);
     }

     if(index==2)
     {QStringList listWaM;
         listWaM<<"H3AsO4"<<"H2B4O7"<<"H2CO3"<<"H2CrO4"<<"H2S"<<"H3PO4"<<"H4P2O7"
               <<"H3PO3"<<"H2SiO3"<<"H2SO4"<<"H2SO3"<<"H2S2O3"<<"H2NCH2COOH"<<"H2C2O4"
               <<"HOOCCOOH"<<"o-C6H4(COOH)2"<<"m-C6H4(COOH)2"<<"p-C6H4(COOH)2"
              <<"HOOCCH=CHCOOH"<<"HOOCCH2CHOHCOOH"<<"HOOCCH2COOH";

         QCompleter *comA1W = new QCompleter(listWaM,this);
         comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
         ui->A1WabNLine->setCompleter(comA1W);

     }

     if(index==3)
     {QStringList listWbM;
         listWbM<<"NH2NH2"<<"Gln(谷氨酰胺)"<<"H2NCH2CH2NH2"
               <<"eda"<<"H2N(CH2)6NH2"<<"(H2NC6H4)2";
         QCompleter *comA1W = new QCompleter(listWbM,this);
         comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
         ui->A1WabNLine->setCompleter(comA1W);
     }

    }

    if(index==4)
    {ui->A1TwoInSNLine->setReadOnly(false);
     ui->A1TwoInSCLine->setReadOnly(false);

     ui->A1WabNLine->setReadOnly(true);
     ui->A1WabCLine->setReadOnly(true);
     ui->A1TwoSCaNLine->setReadOnly(true);
     ui->A1TwoSCaCNLine->setReadOnly(true);
     ui->A1TwoSCacLine->setReadOnly(true);
     ui->A1TwoSCbcLine->setReadOnly(true);
     ui->A1BufferAorBLine->setReadOnly(true);
     ui->A1BufferAcidCLine->setReadOnly(true);
     ui->A1BufferBaseCLine->setReadOnly(true);
     QVariant v(0);
     ui->A1BufferAorBCom->setItemData(0,v,Qt::UserRole -1);
     ui->A1BufferAorBCom->setItemData(1,v,Qt::UserRole -1);

     QStringList listT;
      listT<<"HCO3-"<<"HPO4_2-"<<"HSO3-"<<"H2PO3-"<<"HS-"<<"HB4O7-"
           <<"HCrO4-"<<"HC2O4-"<<"H2AsO4-""HAsO4_2-";
         QCompleter *comA1W = new QCompleter(listT,this);
         comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
         ui->A1TwoInSNLine->setCompleter(comA1W);
    }

    if(index==5)
    {ui->A1TwoSCaNLine->setReadOnly(false);
     ui->A1TwoSCaCNLine->setReadOnly(false);
     ui->A1TwoSCacLine->setReadOnly(false);
     ui->A1TwoSCbcLine->setReadOnly(false);

     ui->A1TwoInSNLine->setReadOnly(true);
     ui->A1TwoInSCLine->setReadOnly(true);
     ui->A1WabNLine->setReadOnly(true);
     ui->A1WabCLine->setReadOnly(true);
     ui->A1BufferAorBLine->setReadOnly(true);
     ui->A1BufferAcidCLine->setReadOnly(true);
     ui->A1BufferBaseCLine->setReadOnly(true);
     QVariant v(0);
     ui->A1BufferAorBCom->setItemData(0,v,Qt::UserRole -1);
     ui->A1BufferAorBCom->setItemData(1,v,Qt::UserRole -1);

    }

    if(index==6)
    {ui->A1BufferAorBLine->setReadOnly(false);
     ui->A1BufferAcidCLine->setReadOnly(false);
     ui->A1BufferBaseCLine->setReadOnly(false);
     QVariant v(1|32);
     ui->A1BufferAorBCom->setItemData(0,v,Qt::UserRole -1);
     ui->A1BufferAorBCom->setItemData(1,v,Qt::UserRole -1);

     ui->A1WabNLine->setReadOnly(true);
     ui->A1WabCLine->setReadOnly(true);
     ui->A1TwoInSNLine->setReadOnly(true);
     ui->A1TwoInSCLine->setReadOnly(true);
     ui->A1TwoSCaNLine->setReadOnly(true);
     ui->A1TwoSCaCNLine->setReadOnly(true);
     ui->A1TwoSCacLine->setReadOnly(true);
     ui->A1TwoSCbcLine->setReadOnly(true);




    }
}

//酸碱部分1计算
void AnaChemCalculator::on_A1Calculate_pressed()
{
    double pH=7;
    int index = ui->A1pHcom->currentIndex();
    if(index<=3)
    {
        //缺乏数据时报错
        if(ui->A1WabNLine->text().isEmpty()==true
                ||ui->A1WabCLine->text().isEmpty()==true)
        {ui->A1OutPut->insertPlainText("提示:缺乏数据,无法计算!\n");return;}
        //导入浓度和化学式
        double c= ui->A1WabCLine->text().toDouble();
        string str = ui->A1WabNLine->text().toStdString();

        //分条件计算
        if(index==0)
        {bool type = true;
        double K1 = getK1(str,type);
        pH=CalculatepH1(c,K1,index);
        }

        if(index==1)
        {bool type = false;
         double K1 = getK1(str,type);
         pH=CalculatepH1(c,K1,index);
        }

        if(index==2)
        {int NofH = getnumofH(str);
         bool type = true;
            if(NofH==1)
            {ui->A1OutPut->insertPlainText("提示:输入的是一元酸\n");
                double K1 = getK1(str,type);
                pH=CalculatepH1(c,K1,0);
            }

            else
            {double K1 = getK1(str,type), K2 = getK2(str,type);
            if(2*K2/sqrt(c*K1)<=0.05)
            {pH=CalculatepH1(c,K1,0);}
            else
            {ui->A1OutPut->insertPlainText("提示:不满足近似式条件,暂不支持精确计算!\n");
            return;}
            }
        }

        if(index==3)
        {int NofH = getCpofH(str);
         bool type = false;
            if(NofH==1)
            {ui->A1OutPut->insertPlainText("提示:输入的是一元碱\n");
                double K1 = getK1(str,type);
                pH=CalculatepH1(c,K1,0);
            }

            else
            {double K1 = getK1(str,type), K2 = getK2(str,type);
            if(2*K2/sqrt(c*K1)<=0.05)
            {pH=CalculatepH1(c,K1,1);}
            else
            {ui->A1OutPut->insertPlainText("提示:不满足近似式条件,暂不支持精确计算!\n");
            return;}
            }
        }

    }

    if(index==4)
    {//缺乏数据时报错
        if(ui->A1TwoInSNLine->text().isEmpty()==true
                ||ui->A1TwoInSCLine->text().isEmpty()==true)
        {ui->A1OutPut->insertPlainText("提示:缺乏数据,无法计算!\n");return;}
        string str = ui->A1TwoInSNLine->text().toStdString();
        double C = ui->A1TwoInSCLine->text().toDouble(),KCa,Ka;
        KCa = getKAS(str,true);
        Ka = getKAS(str,false);

        if(KCa==0||Ka==0) {ui->A1OutPut->insertPlainText("提示:没有找到对应两性物质数据,"
                                                    "无法计算!\n");return;}
        pH = -log10(sqrt(KCa*(Ka*C+pow(10,-14))/(KCa+C)));
    }

    if(index==5)
    {//缺乏数据时报错
        if(ui->A1TwoSCaNLine->text().isEmpty()==true
                ||ui->A1TwoSCaCNLine->text().isEmpty()==true
                ||ui->A1TwoSCacLine->text().isEmpty()==true
                ||ui->A1TwoSCbcLine->text().isEmpty()==true)
        {ui->A1OutPut->insertPlainText("提示:缺乏数据,无法计算!\n");return;}

        string strA,strCA;
        strA = ui->A1TwoSCaNLine->text().toStdString();
        strCA = ui->A1TwoSCaCNLine->text().toStdString();
        double Ca,Cb,K1,K2;
        Ca = ui->A1TwoSCacLine->text().toDouble();
        Cb = ui->A1TwoSCbcLine->text().toDouble();

        //第一轮查找:已导入的酸数据
        K1 = getK1(strCA,true);
        K2 = getK1(strA,true);

        //酸数据中查找不到共轭酸Ka时在两性物质中进行二轮查找
        if(K1==0)
        {K1 = getKAS(strCA,false);
        }

        if(K1==0) {ui->A1OutPut->insertPlainText("提示:没有找到对应两性物质数据,"
                                                    "无法计算!\n");return;}
        pH=-log10(sqrt(K1*(K2*Ca+pow(10,-14))/(K1+Cb)));

    }

    if(index==6)
    {//缺乏数据时报错
        if(ui->A1BufferAorBLine->text().isEmpty()==true
                ||ui->A1BufferAcidCLine->text().isEmpty()==true
                ||ui->A1BufferBaseCLine->text().isEmpty()==true)
        {ui->A1OutPut->insertPlainText("提示:缺乏数据,无法计算!\n");return;}

     int mode = ui->A1BufferAorBCom->currentIndex();
     double Ca=ui->A1BufferAcidCLine->text().toDouble(),
            Cb=ui->A1BufferBaseCLine->text().toDouble(),K=0;

     //根据输入模式(共轭酸还是共轭碱)计算
     if(mode==0)
     {string strCA = ui->A1BufferAorBLine->text().toStdString();
         //第一轮查找:已导入的酸数据
         K = getK1(strCA,true);

         //酸数据中查找不到共轭酸Ka时在两性物质中进行二轮查找
         if(K==0)
         {K = getKAS(strCA,false);}
     }

     if(mode==1)
     {string strCB = ui->A1BufferAorBLine->text().toStdString();
         //第一轮查找:已导入的碱数据
         if(getK1(strCB,false)!=0)
         {K=pow(10,-14)/getK1(strCB,false);}
         else K=0;

         //碱数据中查找不到共轭碱Kb时在两性物质中二轮查找共轭酸Ka
         if(K==0)
         {K = getKAS(strCB,true);}
     }

     if(K==0) {ui->A1OutPut->insertPlainText("提示:没有找到对应两性物质数据,"
                                                 "无法计算!\n");return;}
        pH=-log10(K)+log10(Cb/Ca);
    }

    ui->A1OutPut->insertPlainText(QString("计算得到pH为%1\n").arg(pH));
}

//酸碱部分1重置设置
void AnaChemCalculator::on_A1ResetOutput_pressed()
{
    ui->A1OutPut->clear();
}

void AnaChemCalculator::on_A1ResetAandB_pressed()
{
    ui->A1WabCLine->clear();
    ui->A1WabNLine->clear();
}

void AnaChemCalculator::on_A1ResetTwoInS_pressed()
{
    ui->A1TwoInSNLine->clear();
    ui->A1TwoInSCLine->clear();
}

void AnaChemCalculator::on_A1ResetTwoS2_pressed()
{
    ui->A1TwoSCaNLine->clear();
    ui->A1TwoSCaCNLine->clear();
    ui->A1TwoSCacLine->clear();
    ui->A1TwoSCbcLine->clear();
}

void AnaChemCalculator::on_A1ResetBuffer_pressed()
{
    ui->A1BufferAorBCom->setCurrentIndex(0);
    ui->A1BufferAorBLine->clear();
    ui->A1BufferAcidCLine->clear();
    ui->A1BufferBaseCLine->clear();
}

//酸碱部分2-1:分步准滴判断
//重置选项实现
void AnaChemCalculator::on_A2ResetAorB_pressed()
{
    ui->A2AorBNLine->clear();
    ui->A2AorBCLine->clear();
}

void AnaChemCalculator::on_A2ResetMul_pressed()
{
    ui->A2TwoN1Line->clear();
    ui->A2TwoN2Line->clear();
    ui->A2TwoC1Line->clear();
    ui->A2TwoC2Line->clear();
}

void AnaChemCalculator::on_A2ResetOutput_pressed()
{
    ui->A2Output->clear();
}


//设置只有当前所选项可操作,同时提示可选项
void AnaChemCalculator::on_A2TypeBox_currentIndexChanged(int index)
{
    if(index<=1)
    {
        ui->A2AorBNLine->setReadOnly(false);
        ui->A2AorBCLine->setReadOnly(false);
        ui->A2TwoN1Line->setReadOnly(true);
        ui->A2TwoN2Line->setReadOnly(true);
        ui->A2TwoC1Line->setReadOnly(true);
        ui->A2TwoC2Line->setReadOnly(true);

        if(index==0)
        {QStringList listWaM;
            listWaM<<"H3AsO4"<<"H2B4O7"<<"H2CO3"<<"H2CrO4"<<"H2S"<<"H3PO4"<<"H4P2O7"
                  <<"H3PO3"<<"H2SiO3"<<"H2SO4"<<"H2SO3"<<"H2S2O3"<<"H2NCH2COOH"<<"H2C2O4"
                  <<"HOOCCOOH"<<"o-C6H4(COOH)2"<<"m-C6H4(COOH)2"<<"p-C6H4(COOH)2"
                 <<"HOOCCH=CHCOOH"<<"HOOCCH2CHOHCOOH"<<"HOOCCH2COOH";

            QCompleter *comA1W = new QCompleter(listWaM,this);
            comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
            ui->A2AorBNLine->setCompleter(comA1W);
        }

        if(index==1)
        {QStringList listWbM;
            listWbM<<"NH2NH2"<<"Gln(谷氨酰胺)"<<"H2NCH2CH2NH2"
                  <<"eda"<<"H2N(CH2)6NH2"<<"(H2NC6H4)2";
            QCompleter *comA1W = new QCompleter(listWbM,this);
            comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
            ui->A2AorBNLine->setCompleter(comA1W);
        }

    }

    if(index>=2)
    {
        ui->A2AorBNLine->setReadOnly(true);
        ui->A2AorBCLine->setReadOnly(true);
        ui->A2TwoN1Line->setReadOnly(false);
        ui->A2TwoN2Line->setReadOnly(false);
        ui->A2TwoC1Line->setReadOnly(false);
        ui->A2TwoC2Line->setReadOnly(false);

        if(index==2)
        {QStringList listWaS;
            listWaS<<"NH4+"<<"H3AsO2"<<"H3BO2"<<"HNCO"<<"HCN"<<"HF"
                  <<"H2O2"<<"HBrO"<<"HClO"<<"HIO"<<"HNO2"<<"HSCN"<<"HCOOH"<<"CH3COOH"
                  <<"C2H5COOH"<<"(CH3)2CHCOOH"<<"CH3(CH2)2COOH"<<"ClCH2COOH"<<"Cl2CHCOOH"
                 <<"Cl3CCOOH"<<"CH3CH(OH)COOH"<<"H2N(CH2)4CH(NH2)COOH"<<"C6H5COOH"
                 <<"C6H5OH"<<"C4H3OCOOH"<<"CH3COCH2COOH"<<"CH3COCH2COCH3"
                 <<"CH2=CHCOOH"<<"HOC6H4COOH"<<"HAc";
            QCompleter *comA1W = new QCompleter(listWaS,this);
            comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
            ui->A2TwoN1Line->setCompleter(comA1W);
            ui->A2TwoN2Line->setCompleter(comA1W);
        }

        if(index==3)
        {QStringList listWbS;
            listWbS<<"NH3"<<"NH2OH"<<"CH3CONH2"<<"CH3(CH2)4NH2"<<"C6H5NH2"<<"o-H2NC5H4OCH3"
               <<"m-H2NC5H4OCH3"<<"p-H2NC5H4OCH3"<<"(C2H5)2NH"<<"(CH3)2NH"<<"Ph2NH"
              <<"H2NC2H4OH"<<"C2H5NH2"<<"CH3(CH2)5NH2"<<"(CH2)6N4"<<"CH3NH2"<<"Ac-"<<"CH3COO-";
            QCompleter *comA1W = new QCompleter(listWbS,this);
            comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
            ui->A2TwoN1Line->setCompleter(comA1W);
            ui->A2TwoN2Line->setCompleter(comA1W);
        }

    }
}

//酸碱部分2-1计算
void AnaChemCalculator::on_A2Calculate1_pressed()
{
    int index=ui->A2TypeBox->currentIndex();

    if(index==0)
    {
        if(ui->A2AorBNLine->text().isEmpty()==true
                ||ui->A2AorBCLine->text().isEmpty()==true)
        {ui->A2Output->insertPlainText("提示:缺乏数据,无法计算!\n");
        return;
        }

        string formula = ui->A2AorBNLine->text().toStdString();
        double C = ui->A2AorBCLine->text().toDouble();
        double numofH = getnumofH(formula);

        if(numofH==0)
        {ui->A2Output->insertPlainText("提示:没有找到对应酸数据,无法计算!\n");
            return;}

        double K[4];
        K[0] = getK1(formula,true);
        K[1] = getK2(formula,true);
        K[2] = getK3(formula);
        K[3] = getK4(formula);

        if(C*K[0]>1e-8)
        {ui->A2Output->insertPlainText(QString("cKa1=%1>=1e-8 可以准确滴定第1个H\n").arg(C*K[0]));}
        else
        {ui->A2Output->insertPlainText(QString("cKa1=%1<1e-8 不能准确滴定第1个H,无需进一步计算讨论\n").arg(C*K[0]));
        return;}

        for(int i=1;i<4;i++)
        {
            if(K[i]==0) break;

            if(C*K[i]>=1e-8)
            {   if(K[i-1]/K[i]<1e4)
                {
                    ui->A2Output->insertPlainText(QString("Ka%1/Ka%2=%3<1e4 "
                           "不能分步滴定第%4个H!\n").arg(i).arg(i+1).arg(K[i-1]/K[i]).arg(i+1));
                }
                else
                {ui->A2Output->insertPlainText(QString("Ka%1/Ka%2=%3>=1e4 cKa%4=%5>=1e-8 "
                           "可以分步准确滴定第%6个H\n").arg(i).arg(i+1)
                              .arg(K[i-1]/K[i]).arg(i+1).arg(C*K[i]).arg(i+1));

                }
            }
            else
            {ui->A2Output->insertPlainText(QString("cKa%1=%2<1e-8 不能准确滴定第%3个H\n")
                                           .arg(i+1).arg(C*K[i]).arg(i+1));}

        }

    }

    if(index==1)
    {if(ui->A2AorBNLine->text().isEmpty()==true
                ||ui->A2AorBCLine->text().isEmpty()==true)
        {ui->A2Output->insertPlainText("提示:缺乏数据,无法计算!\n");
        return;
        }

        string formula = ui->A2AorBNLine->text().toStdString();
        double c = ui->A2AorBCLine->text().toDouble();
        int num = getCpofH(formula);
        if(num==0)
        {ui->A2Output->insertPlainText("提示:没有找到对应碱数据,无法计算!\n");
            return;}

        double K1 = getK1(formula,false),K2 = getK2(formula,false);
        if(c*K1>=1e-8)
        {
            ui->A2Output->insertPlainText(QString("cKb1=%1>=1e-8 "
                             "可以滴定第一级\n").arg(c*K1));
                if(c*K2>=1e-8)
                {
                    if(K1/K2>=1e4)
                    ui->A2Output->insertPlainText(QString("Kb1/Kb2=%1>=1e4 "
                                   "可以分步滴定!\n").arg(K1/K2));
                    else ui->A2Output->insertPlainText(QString("Kb1/Kb2=%1<1e4 "
                                   "不能准确滴定，只能形成一个突跃\n").arg(K1/K2));
                }
                else {ui->A2Output->insertPlainText(QString("cKb2=%1<1e-8 "
                               "只能准确滴定第一级,不能分步滴定\n").arg(c*K2));}
        }
        else
        {ui->A2Output->insertPlainText(QString("cKb1=%1<1e-8 "
                 "不能滴定第一级,无需进一步讨论\n").arg(c*K1));

        }
    }

    if(index==2)
    {   if(ui->A2TwoN1Line->text().isEmpty()==true
            ||ui->A2TwoN2Line->text().isEmpty()==true
            ||ui->A2TwoC1Line->text().isEmpty()==true
            ||ui->A2TwoC2Line->text().isEmpty()==true)
        {ui->A2Output->insertPlainText("提示:缺乏数据,无法计算!\n");
        return;}

        QString formula1 = ui->A2TwoN1Line->text(),
               formula2 = ui->A2TwoN2Line->text();

        if(formula1==formula2)
        {ui->A2Output->insertPlainText("提示:输入了相同的化学式!计算出错!\n");
        return;}

        int num1 = getnumofH(formula1.toStdString());
        int num2 = getnumofH(formula2.toStdString());
        if(num1==0)
        {ui->A2Output->insertPlainText(QString("提示:没有找到%1的数据"
                                        ",无法计算!\n").arg(formula1)); return;
        }
        if(num2==0)
        {ui->A2Output->insertPlainText(QString("提示:没有找到%1的数据"
                                        ",无法计算!\n").arg(formula2));return;
        }

        double c1= ui->A2TwoC1Line->text().toDouble(),
               c2= ui->A2TwoC2Line->text().toDouble(),
               K1= getK1(formula1.toStdString(),true),
               K2= getK1(formula2.toStdString(),true);

        if(K1>K2)
        {
            ui->A2Output->insertPlainText("Ka1>Ka2 ");
            if(c1*K1>=1e-8)
            {
                ui->A2Output->insertPlainText(QString("c1Ka1=%1>=1e-8 "
                                 "可以准确滴定较强酸!\n").arg(c1*K1));
                    if(c2*K2>=1e-8)
                    {
                        ui->A2Output->insertPlainText(QString("c2Ka2=%1>=1e-8 ")
                                     .arg(c2*K2));
                        if((c1*K1)/(c2*K2)>=1e4)
                        ui->A2Output->insertPlainText(QString("c1Ka1/c2Ka2=%2>=1e4 "
                                   "可以分步滴定!\n").arg((c1*K1)/(c2*K2)));
                        else ui->A2Output->insertPlainText(QString("c1Ka1/c2Ka2=%1<1e4 "
                                       "不能分步滴定,只能形成一个突跃\n").arg((c1*K1)/(c2*K2)));
                    }
                    else {ui->A2Output->insertPlainText(QString("c2Ka2=%1<1e-8 "
                                   "只能准确滴定较强酸,不能准确滴定较弱酸!\n").arg(c2*K2));}
            }
            else
            {ui->A2Output->insertPlainText(QString("c1*Ka1=%1<1e-8 "
                     "无法准确滴定较强酸!\n").arg(c1*K1));}
        }

        if(K1<K2)
        {
            ui->A2Output->insertPlainText("Ka1<Ka2 ");
            if(c2*K2>=1e-8)
            {
                ui->A2Output->insertPlainText(QString("c2Ka2=%1>=1e-8 "
                                 "可以准确滴定较强酸!\n").arg(c2*K2));
                    if(c1*K1>=1e-8)
                    {
                        ui->A2Output->insertPlainText(QString("c1Ka1=%1>=1e-8 ")
                                     .arg(c1*K1));
                        if((c2*K2)/(c1*K1)>=1e4)
                        ui->A2Output->insertPlainText(QString("c2Ka2/c1Ka1=%1>=1e4 "
                                       "可以分步滴定!\n").arg((c2*K2)/(c1*K1)));
                        else ui->A2Output->insertPlainText(QString("c2Ka2/c1Ka1=%1<1e4 "
                                       "不能分步滴定,只能形成一个突跃\n").arg((c2*K2)/(c1*K1)));
                    }
                    else {ui->A2Output->insertPlainText(QString("c1Ka1=%1<1e-8 "
                                   "只能准确滴定较强酸,不能准确滴定较弱酸!\n").arg(c1*K1));}
            }
            else
            {ui->A2Output->insertPlainText(QString("c2*Ka2=%1<1e-8 "
                     "无法准确滴定较强酸!\n").arg(c2*K2));}
        }
    }

    if(index==3)
    {if(ui->A2TwoN1Line->text().isEmpty()==true
                ||ui->A2TwoN2Line->text().isEmpty()==true
                ||ui->A2TwoC1Line->text().isEmpty()==true
                ||ui->A2TwoC2Line->text().isEmpty()==true)
            {ui->A2Output->insertPlainText("提示:缺乏数据,无法计算!\n");
            return;}

            QString formula1 = ui->A2TwoN1Line->text(),
                   formula2 = ui->A2TwoN2Line->text();

            if(formula1==formula2)
            {ui->A2Output->insertPlainText("提示:输入了相同的化学式!计算出错!\n");
            return;}

            int num1 = getnumofH(formula1.toStdString());
            int num2 = getnumofH(formula2.toStdString());
            if(num1==0)
            {ui->A2Output->insertPlainText(QString("提示:没有找到%1的数据"
                                            ",无法计算!\n").arg(formula1)); return;
            }
            if(num2==0)
            {ui->A2Output->insertPlainText(QString("提示:没有找到%1的数据"
                                            ",无法计算!\n").arg(formula2));return;
            }

            double c1= ui->A2TwoC1Line->text().toDouble(),
                   c2= ui->A2TwoC2Line->text().toDouble(),
                   K1= getK1(formula1.toStdString(),false),
                   K2= getK1(formula2.toStdString(),false);

            if(K1>K2)
            {
                ui->A2Output->insertPlainText("Kb1>Kb2 ");
                if(c1*K1>=1e-8)
                {
                    ui->A2Output->insertPlainText(QString("c1Kb1=%1>=1e-8 "
                                     "可以准确滴定较强碱!\n").arg(c1*K1));
                        if(c2*K2>=1e-8)
                        {
                            ui->A2Output->insertPlainText(QString("c2Kb2=%1>=1e-8 ")
                                         .arg(c2*K2));
                            if((c1*K1)/(c2*K2)>=1e4)
                            ui->A2Output->insertPlainText(QString("c1Kb1/c2Kb2=%1>=1e4 "
                                           "可以分步滴定!\n").arg(c1*K1/(c2*K2)));
                            else ui->A2Output->insertPlainText(QString("c1Kb1/c2Kb2=%1<1e4 "
                                           "不能分步滴定,只能形成一个突跃\n").arg(c1*K1/(c2*K2)));
                        }
                        else {ui->A2Output->insertPlainText(QString("c2Kb2=%1<1e-8 "
                                       "只能准确滴定较强碱,不能准确滴定较弱碱!\n").arg(c2*K2));}
                }
                else
                {ui->A2Output->insertPlainText(QString("c1*Kb1=%1<1e-8 "
                         "无法准确滴定较强碱!\n").arg(c1*K1));}
            }

            if(K1<K2)
            {
                ui->A2Output->insertPlainText("Kb1<Kb2 ");
                if(c2*K2>=1e-8)
                {
                    ui->A2Output->insertPlainText(QString("c2Kb2=%1>=1e-8 "
                                     "可以准确滴定较强碱!\n").arg(c1*K1));
                        if(c1*K1>=1e-8)
                        {
                            ui->A2Output->insertPlainText(QString("c1Kb1=%1>=1e-8 ")
                                         .arg(c1*K1));
                            if(c2*K2/(c1*K1)>=1e4)
                            ui->A2Output->insertPlainText(QString("c2Kb2/c1Kb1=%1>=1e4 "
                                           "可以分步滴定!\n").arg(c2*K2/(c1*K1)));
                            else ui->A2Output->insertPlainText(QString("c2Kb2/c1Kb1=%1<1e4 "
                                           "不能分步滴定,只能形成一个突跃\n").arg(c2*K2/(c1*K1)));
                        }
                        else {ui->A2Output->insertPlainText(QString("c1Kb1=%1<1e-8 "
                                       "只能准确滴定较强碱,不能准确滴定较弱碱!\n").arg(c1*K1));}
                }
                else
                {ui->A2Output->insertPlainText(QString("c2*Kb2=%1<1e-8 "
                         "无法准确滴定较强碱!\n").arg(c2*K2));}
            }

    }
}

//酸碱部分2-2:终点误差计算
//设置不同滴定模式下计算模式可选项提示
void AnaChemCalculator::on_A2TEModeBox_currentIndexChanged(int index)
{
    if(index==0)
    { QStringList listWaS;
        listWaS<<"NH4+"<<"H3AsO2"<<"H3BO2"<<"HNCO"<<"HCN"<<"HF"
              <<"H2O2"<<"HBrO"<<"HClO"<<"HIO"<<"HNO2"<<"HSCN"<<"HCOOH"<<"CH3COOH"
              <<"C2H5COOH"<<"(CH3)2CHCOOH"<<"CH3(CH2)2COOH"<<"ClCH2COOH"<<"Cl2CHCOOH"
             <<"Cl3CCOOH"<<"CH3CH(OH)COOH"<<"H2N(CH2)4CH(NH2)COOH"<<"C6H5COOH"
             <<"C6H5OH"<<"C4H3OCOOH"<<"CH3COCH2COOH"<<"CH3COCH2COCH3"
             <<"CH2=CHCOOH"<<"HOC6H4COOH";
        QCompleter *comA1W = new QCompleter(listWaS,this);
        comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
        ui->A2TNLine->setCompleter(comA1W);
    }

    if(index==1)
    {QStringList listWbS;
        listWbS<<"NH3"<<"NH2OH"<<"CH3CONH2"<<"CH3(CH2)4NH2"<<"C6H5NH2"<<"o-H2NC5H4OCH3"
           <<"m-H2NC5H4OCH3"<<"p-H2NC5H4OCH3"<<"(C2H5)2NH"<<"(CH3)2NH"<<"Ph2NH"
          <<"H2NC2H4OH"<<"C2H5NH2"<<"CH3(CH2)5NH2"<<"(CH2)6N4"<<"CH3NH2"<<"Ac-"<<"CH3COO-";
        QCompleter *comA1W = new QCompleter(listWbS,this);
        comA1W->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
        ui->A2TNLine->setCompleter(comA1W);
    }

}

//重置实现
void AnaChemCalculator::on_A2ResetTE_pressed()
{
    ui->A2TNLine->clear();
    ui->A2TCLine->clear();
    ui->A2TtCLine->clear();
}

//酸碱部分2-2计算
void AnaChemCalculator::on_A2Calculate2_pressed()
{
    if(ui->A2TNLine->text().isEmpty()==true||
    ui->A2TCLine->text().isEmpty()==true||
    ui->A2TtCLine->text().isEmpty()==true)
    {
       ui->A2Output->insertPlainText("提示:信息不足,无法计算TE!\n");
       return;
    }

    QString formula = ui->A2TNLine->text();
    double ratio = ui->A2TCLine->text().toDouble()/ui->A2TtCLine->text().toDouble();
    double Csp = ui->A2TCLine->text().toDouble()/(1+ratio);
    int index = ui->A2IndicatorBox->currentIndex();
    bool type = ui->A2TEModeBox->currentIndex();
    double TE = CalculateTE_A(Csp,formula,index,type);

    if(TE!=-1) {ui->A2Output->insertPlainText(QString("计算得到TE=%1%\n").arg(100*TE));}
    else {ui->A2Output->insertPlainText("提示:找不到所滴定物质对应的数据,无法计算!\n");}
}
