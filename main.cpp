#include "anachemcalculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnaChemCalculator w;
    w.resize(1350,650);
    w.show();
    return a.exec();
}
