#include "calc.h"
#include "ui_calc.h"

Calc::Calc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc)
{
    ui->setupUi(this);
}

Calc::~Calc()
{
    delete ui;
}

void Calc::on_DC_total_but_clicked()
{
    QString qstr_DC_packaging = ui->DC_packaging_line->text();
    QString qstr_DC_delivery = ui->DC_delivery_line->text();
    QString qstr_DC_loading = ui->DC_loading_line->text();
    QString qstr_DC_feels = ui->DC_fees_line->text();
    QString qstr_DC_storageRoom = ui->DC_storageRoom_line->text();
    QString qstr_DC_advertising = ui->DC_advertising_line->text();
    QString qstr_DC_entertainment = ui->DC_entertainment_line->text();

    //если придется передавать в string вормате, то float оставим для
    //возможности считать локально

    fl_DC_packaging = qstr_DC_packaging.toFloat();
    fl_DC_delivery = qstr_DC_delivery.toFloat();
    fl_DC_loading = qstr_DC_loading.toFloat();
    fl_DC_feels = qstr_DC_feels.toFloat();
    fl_DC_storageRoom = qstr_DC_storageRoom.toFloat();
    fl_DC_advertising = qstr_DC_advertising.toFloat();
    fl_DC_entertainment = qstr_DC_entertainment.toFloat();

    fl_DC_total = (fl_DC_packaging+fl_DC_delivery+fl_DC_loading+fl_DC_feels+fl_DC_storageRoom+fl_DC_advertising+fl_DC_entertainment);

    ui->DC_total_line->setText(QString("%1").arg(fl_DC_total));
}

void Calc::on_NPC_total_but_clicked()
{
    QString qstr_NPC_plain = ui->NPC_plain_line->text();
    QString qstr_NPC_devProd = ui->NPC_defProd_line->text();
    QString qstr_NPC_damProp = ui->NPC_damProp_line->text();
    QString qstr_NPC_forfeits = ui->NPC_forfeits_line->text();
    QString qstr_NPC_lossMat = ui->NPC_lossMat_line->text();

    //если придется передавать в string вормате, то float оставим для
    //возможности считать локально

    fl_NPC_plain = qstr_NPC_plain.toFloat();
    fl_NPC_devProd = qstr_NPC_devProd.toFloat();
    fl_NPC_damProp = qstr_NPC_damProp.toFloat();
    fl_NPC_forfeits = qstr_NPC_forfeits.toFloat();
    fl_NPC_lossMat = qstr_NPC_lossMat.toFloat();

    fl_NPC_total = (fl_NPC_plain+fl_NPC_devProd+fl_NPC_damProp+fl_NPC_forfeits+fl_NPC_lossMat);

    ui->NPC_total_line->setText(QString("%2").arg(fl_NPC_total));
}

void Calc::on_WC_total_but_clicked()
{
    QString qstr_WC_deprecation = ui->WC_depreciation_line->text();
    QString qstr_WC_heating = ui->WC_heating_line->text();
    QString qstr_WC_deductionWage = ui->WC_deductionWage_line->text();
    QString qstr_WC_deductionSalary = ui->WC_deductionSalary_line->text();

    //если придется передавать в string вормате, то float оставим для
    //возможности считать локально

    fl_WC_deprecation = qstr_WC_deprecation.toFloat();
    fl_WC_heating = qstr_WC_heating.toFloat();
    fl_WC_deductionWage = qstr_WC_deductionWage.toFloat();
    fl_WC_deductionSalary = qstr_WC_deductionSalary.toFloat();

    fl_WC_total = (fl_WC_deprecation + fl_WC_heating + fl_WC_deductionWage + fl_WC_deductionSalary);

    ui->WC_total_line->setText(QString("%3").arg(fl_WC_total));
}

void Calc::on_PE_total_but_clicked()
{
    QString qstr_PE_sfProducts = ui->PE_sfProducts_line->text();
    QString qstr_PE_primProd = ui->PE_primProd_line->text();
    QString qstr_PE_auxiliaryProd = ui->PE_auxiliaryProd_line->text();
    QString qstr_PE_overheadCosts = ui->PE_overheadCosts_line->text();
    QString qstr_PE_marriage = ui->PE_marriage_line->text();
    QString qstr_PE_prodService = ui->PE_prodService_line->text();


}
