#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <qsettings.h>

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDialog)
{
    QSettings ustawienia;
    ui->setupUi(this);
    ui->masa->setValue(ustawienia.value("TerraCan/masa", 0.32).toDouble());
    ui->opor->setValue(ustawienia.value("TerraCan/opor", 0.8).toDouble());          //wpolczynnik oporu czyli Cd
    ui->powierchnia->setValue(ustawienia.value("TerraCan/Cd", 0.0045).toDouble());  //powierzchnia a nie wspolczynnik oporu !!!

    ui->dzielnik->setValue(ustawienia.value("TerraCan/dzielnik", 100).toInt());
    ui->logCount->setValue(ustawienia.value("TerraCan/logcount", 100000).toInt());
    ui->chute->setValue(ustawienia.value("TerraCan/chute",0).toFloat());
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_buttonBox_accepted()
{
    QSettings ustawienia;
    ustawienia.setValue("TerraCan/masa", ui->masa->value());
    ustawienia.setValue("TerraCan/opor", ui->opor->value());
    ustawienia.setValue("TerraCan/Cd", ui->powierchnia->value());

    ustawienia.setValue("TerraCan/dzielnik", ui->dzielnik->value());
    ustawienia.setValue("TerraCan/logcount", ui->logCount->value());
    ustawienia.setValue("TerraCan/chute", ui->chute->value());
}

