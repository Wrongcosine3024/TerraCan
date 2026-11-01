#include "mainwindow.h"
#include "physicsengine.h"
#include "settingdialog.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDateTime>
#include <qsettings.h>
#include <qthread.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //COM init
    comWorker = new OdbiornikWorker(this);
    connect(comWorker, &OdbiornikWorker::newData, this, &MainWindow::refreshConsole);
    connect(comWorker, &OdbiornikWorker::connResult, this, &MainWindow::connResult);
    comlistrefresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::comlistrefresh(){      //odswiezanie listy portow com
    ui->com_list->clear();
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {     // listowanie portów com po kliknieciu
        QString etykieta = info.portName() + " (" + info.description() + ')';
        QListWidgetItem *item = new QListWidgetItem(etykieta);
        item->setData(Qt::UserRole, info.portName());
        ui->com_list->addItem(item);
    }
}

void MainWindow::on_com_list_itemClicked(QListWidgetItem *item)     //wybór portów com
{
    QString selectedCOM;
    selectedCOM = item->data(Qt::UserRole).toString();
    ui->port_label->setText(selectedCOM);
}


void MainWindow::on_COM_connect_toggled(bool checked)               // łączenie z portem com
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QListWidgetItem *item = ui->com_list->currentItem();
    QString portname = NULL;
    if(item){
        portname = item->data(Qt::UserRole).toString();
    }
    if(checked){
        ui->COM_connect->setText("ŁĄCZENIE...");
        ui->COM_connect->setEnabled(false);
        comWorker->podlaczCom(portname);

        //ui->COM_connect->setText("POŁĄCZONO!");
        //ui->konsola->appendPlainText("(LOG) " + timestamp + ": Połączono z portem " + portname);
    }else{
        comWorker->rozlaczCom();
    }
}

void MainWindow::connResult(bool result, QString result_string){
    if(result){
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        ui->konsola->appendPlainText("(LOG) " + timestamp +": " + result_string);
        ui->COM_connect->blockSignals(true);
        ui->COM_connect->setText("ROZŁĄCZ");
        ui->COM_connect->setChecked(true);
        ui->COM_connect->setEnabled(true);
        ui->COM_connect->blockSignals(false);
        ui->com_list->setDisabled(1);
    } else {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        ui->konsola->appendPlainText("(LOG) " + timestamp +": " + result_string);
        ui->COM_connect->blockSignals(true);
        ui->COM_connect->setChecked(false);
        ui->com_list->setDisabled(false);
        ui->COM_connect->setEnabled(true);
        ui->COM_connect->blockSignals(false);
        ui->COM_connect->setText("POŁĄCZ");
        ui->com_list->setDisabled(0);
    }
}

void MainWindow::refreshConsole(const TELEMETRY &telemetria){
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->konsola->appendPlainText("(LOG) " + timestamp + ": Nr.Pakietu " + QString::number(telemetria.SEQ) + " Czas pracy mikrokontrolera: " + QString::number(telemetria.timestamp/1000) + " Temperatura: " + QString::number(telemetria.temperature) + " Ciśnienie: " + QString::number(telemetria.pressure));
}

void MainWindow::on_refresh_clicked()
{
    comlistrefresh();
}

void MainWindow::on_clear_clicked()
{
    ui->sim_output->clear();
}

void MainWindow::physConsole(QString line){
    ui->sim_output->appendPlainText(line);
}

void MainWindow::on_startSIM_clicked()
{
    QSettings ustawienia;
    //ui setup
    ui->simbtn1->setDisabled(true);
    ui->simbtn2->setDisabled(true);
    ui->numbers_grid->setDisabled(true);
    ui->startSIM->setText("W trakcie...");

    //phys thread setup
    QThread *physicsThread = new QThread(this);
    PhysicsEngine *physics_engine = new PhysicsEngine;
    //ustawianie danych od uzytkownika
    physics_engine->TerraCan.mass = ustawienia.value("TerraCan/mass", 0.32).toDouble();
    physics_engine->TerraCan.A = ustawienia.value("TerraCan/Cd", 0.0045).toDouble();
    physics_engine->TerraCan.Cd = ustawienia.value("TerraCan/opor", 0.8).toDouble();

    physics_engine->x_start = ui->pos_x->value();
    physics_engine->y_start = ui->pos_y->value();
    physics_engine->z_start = ui->pos_z->value();
    physics_engine->vx_start = ui->vel_x->value();
    physics_engine->vy_start = ui->vel_y->value();
    physics_engine->vz_start = ui->vel_z->value();
    physics_engine->dt = ui->dt->value();
    double tt = ui->timeout->value()/ui->dt->value();
    physics_engine->timeout = qRound(tt);

    physics_engine->moveToThread(physicsThread);
    connect(physicsThread, &QThread::started, physics_engine, &PhysicsEngine::derive);
    connect(physicsThread, &QThread::finished, physics_engine, &QObject::deleteLater);
    connect(physicsThread, &QThread::finished, physicsThread, &QObject::deleteLater);
    connect(physics_engine, &PhysicsEngine::skonczylem, physicsThread, &QThread::quit);

    connect(physics_engine, &PhysicsEngine::krokPoliczony, this, &MainWindow::physConsole);
    connect(physicsThread, &QThread::finished, this, [this]() {
        ui->simbtn1->setDisabled(false);
        ui->simbtn2->setDisabled(false);
        ui->numbers_grid->setDisabled(false);
        ui->startSIM->setText("Uruchom SIM");
    });
    physicsThread->start();
}


void MainWindow::on_actionUstawienia_triggered()
{
    SettingDialog okno_ustawien(this);
    if(okno_ustawien.exec() == QDialog::Accepted){
        ;
    }
}

