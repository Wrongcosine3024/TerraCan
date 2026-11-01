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
    ui->opor->setValue(ustawienia.value("TerraCan/opor", 0.8).toDouble());
    ui->powierchnia->setValue(ustawienia.value("TerraCan/Cd", 0.0045).toDouble());
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
}

