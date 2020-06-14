#ifndef ANACHEMCALCULATOR_H
#define ANACHEMCALCULATOR_H

#include <QMainWindow>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class AnaChemCalculator; }
QT_END_NAMESPACE

class AnaChemCalculator : public QMainWindow
{
    Q_OBJECT

public:
    AnaChemCalculator(QWidget *parent = nullptr);
    ~AnaChemCalculator();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    //自定义槽函数(借助ui)
    void on_C2Calculate_pressed();

    void on_Testbutton_pressed();

    void on_C2Reset_pressed();

    void on_C2ResetOP_pressed();

    void on_C2ResetLine1_pressed();

    void on_C2ResetLine2_pressed();

    void on_C2ResetLine3_pressed();

    void on_C2ResetLine4_pressed();

    void on_C1IndicatorCon_currentIndexChanged(int index);

    void on_C1ResetAll_pressed();

    void on_C1ResetSR_pressed();

    void on_C1ResetOutput_pressed();

    void on_C1ResetTE_pressed();

    void on_C1Calculate_pressed();


    void on_C1L1Box_currentIndexChanged(int index);

    void on_C1L2Box_currentIndexChanged(int index);

    void on_C1L3Box_currentIndexChanged(int index);

    void on_A1pHcom_currentIndexChanged(int index);

    void on_C2ShelterBox1_currentIndexChanged(int index);

    void on_C2ShelterBox2_currentIndexChanged(int index);

    void on_C2ShelterBox3_currentIndexChanged(int index);

    void on_C2ShelterBox4_currentIndexChanged(int index);

    void on_A1Calculate_pressed();

    void on_A1ResetOutput_pressed();

    void on_A1ResetAandB_pressed();

    void on_A1ResetTwoInS_pressed();

    void on_A1ResetTwoS2_pressed();

    void on_A1ResetBuffer_pressed();

    void on_C2ResetSh_pressed();

    void on_SearchTypeBox_currentIndexChanged(int index);

    void on_SearchReset_pressed();

    void on_SearchResetL_pressed();

    void on_A2ResetAorB_pressed();

    void on_A2ResetMul_pressed();

    void on_A2ResetOutput_pressed();

    void on_A2TypeBox_currentIndexChanged(int index);

    void on_A2Calculate1_pressed();

    void on_A2TEModeBox_currentIndexChanged(int index);

    void on_A2ResetTE_pressed();

    void on_A2Calculate2_pressed();

private:
    Ui::AnaChemCalculator *ui;
};
#endif // ANACHEMCALCULATOR_H
