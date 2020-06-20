#ifndef CALC_H
#define CALC_H

#include <QWidget>

namespace Ui {
class Calc;
}

class Calc : public QWidget
{
    Q_OBJECT

public:
    explicit Calc(QWidget *parent = nullptr);
    ~Calc();

private slots:
    void on_DC_total_but_clicked();

    void on_NPC_total_but_clicked();

    void on_WC_total_but_clicked();

    void on_PE_total_but_clicked();

private:
        Ui::Calc *ui;

        //(type)_(box)_(name)
        //example: fl_DC_packaging - type = float, box = PE, name = packaging

        //Distribution Costs
        float fl_DC_packaging;
        float fl_DC_delivery;
        float fl_DC_loading;
        float fl_DC_feels;
        float fl_DC_storageRoom;
        float fl_DC_advertising;
        float fl_DC_entertainment;
        float fl_DC_total;

        //Non-Production Costs
        float fl_NPC_plain;
        float fl_NPC_devProd;
        float fl_NPC_damProp;
        float fl_NPC_forfeits;
        float fl_NPC_lossMat;
        float fl_NPC_total;

        //Workshop Costs
        float fl_WC_deprecation;
        float fl_WC_heating;
        float fl_WC_deductionWage;
        float fl_WC_deductionSalary;
        float fl_WC_total;

        //Plant Expenses
        float fl_PE_sfProducts;
        float fl_PE_primProd;
        float fl_PE_auxiliaryProd;
        float fl_PE_overheadCosts;
        float fl_PE_marriage;
        float fl_PE_prodService;
        float fl_PE_total;
};

#endif // CALC_H
