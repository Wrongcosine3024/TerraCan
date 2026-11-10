#include "mainwindow.h"
#include "settingdialog.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDateTime>
#include <qsettings.h>
#include <qthread.h>
#include <marble/GeoDataCoordinates.h>
#include <marble/GeoDataPoint.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataDocument.h>
#include <marble/MarbleModel.h>
#include <marble/GeoDataTreeModel.h>
#include "simwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //COM init
    comStatus = 0;
    comWorker = new OdbiornikWorker(this);
    connect(comWorker, &OdbiornikWorker::newData, this, &MainWindow::refreshUI);
    connect(comWorker, &OdbiornikWorker::connResult, this, &MainWindow::connResult);
    comlistrefresh();

    //map init
    map_widget_class = new MapWidget(this);
    ui->map_layout->addWidget(map_widget_class->map_widget);
    map_widget_class->map_widget->centerOn(19.93, 50.06);
    map_widget_class->map_widget->setZoom(2500);

    //chart init
    wykres = new Chart3D;
    ui->wykres3D_layout->addWidget(wykres->getWidget());

    //auto mode setup
    autoModeTimeout = new QTimer(this);
    connect(autoModeTimeout, &QTimer::timeout, this, &MainWindow::autoMode);
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
    // w zaleznosci od wyniku polaczenia z portem com updateowany jest interfejs
    if(result){
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        ui->konsola->appendPlainText("(LOG) " + timestamp +": " + result_string);
        ui->COM_connect->blockSignals(true);
        ui->COM_connect->setText("ROZŁĄCZ");
        ui->COM_connect->setChecked(true);
        ui->COM_connect->setEnabled(true);
        ui->COM_connect->blockSignals(false);
        ui->com_list->setDisabled(1);
        comStatus = 1;
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
        comStatus = 0;
    }
}

void MainWindow::refreshUI(const TELEMETRY &telemetria){
    aktualna_ramka = telemetria;
    //aktualizacja UI po odebraniu telemetrii
    //log do konsoli
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->konsola->appendPlainText("(LOG) " + timestamp + ": Nr.Pakietu " + QString::number(telemetria.SEQ) + " Czas pracy mikrokontrolera: " + QString::number(telemetria.timestamp/1000) + " Temperatura: " + QString::number(telemetria.temperature) + " Ciśnienie: " + QString::number(telemetria.pressure));

    //update mapy
    Marble::GeoDataCoordinates newPoint(telemetria.GPS_long, telemetria.GPS_lat, telemetria.GPS_alt, Marble::GeoDataCoordinates::Degree);

    map_widget_class->aktualna_pozycja->setCoordinate(newPoint);
    map_widget_class->trajektoria->append(newPoint);
    map_widget_class->map_widget->model()->treeModel()->updateFeature(map_widget_class->aktualna_pozycja);
    map_widget_class->map_widget->model()->treeModel()->updateFeature(map_widget_class->znacznik_linii);
    //map_widget_class->map_widget->centerOn(newPoint);

    //update zakladki telemetria
    switch (telemetria.state) {
    case 0:
        ui->state->setText("PRELAUNCH");
        break;
    default:
        break;
    }
    ui->pressure->setText(QString::number(telemetria.pressure));
    ui->temperature->setText(QString::number(telemetria.temperature));
    ui->latitude->setText(QString::number(telemetria.GPS_lat));
    ui->longitude->setText(QString::number(telemetria.GPS_long));
}

void MainWindow::on_refresh_clicked()
{
    comlistrefresh();
}

void MainWindow::on_clear_clicked()
{
    ui->sim_output->clear();
    if(sim_console != nullptr){
        sim_console->clearconsole();
    }
}

void MainWindow::physConsole(QString line){
    ui->sim_output->appendPlainText(line);
}

void MainWindow::on_startSIM_clicked()
{
    if(autoModeTimeout->isActive()){
        autoModeTimeout->stop();

        if (pHandler != nullptr) {
            pHandler->zatrzymaj();
            pHandler->deleteLater();
            pHandler = nullptr;
        }

        ui->clear->setDisabled(false);
        ui->simbtn2->setDisabled(false);
        ui->startSIM->setText("Uruchom SIM");
        ui->sim_output->appendPlainText("Wyłączono tryb automatyczny");
    } else {
        if(ui->auto_mode->isChecked()){
            if(comStatus){
                autoModeTimeout->start(10000);
                ui->startSIM->setText("Tryb AUTO");
                ui->clear->setDisabled(true);
                ui->simbtn2->setDisabled(true);
                ui->sim_output->appendPlainText("Aktywowano tryb automatyczny, symulacja będzie aktualizowana z bieżącymi parametrami co 10 sekund.");
            }else{
                ui->sim_output->appendPlainText("Tryb automatyczny wymaga aktywnego połączenia z portem COM. Symulacja zakończona.");
            }
        } else {
            if(pHandler != nullptr){        //pHandler zainicjalizowany czyli przycisk aktywowany 2 raz

                //usuniecie phandlera
                pHandler->zatrzymaj();
                pHandler->deleteLater();
                pHandler = nullptr;

                //reset ui
                ui->sim_output->appendPlainText("Symulacja przerwana");

                ui->clear->setDisabled(false);
                ui->simbtn2->setDisabled(false);
                ui->numbers_grid->setDisabled(false);
                ui->startSIM->setText("Uruchom SIM");
            }else{
                //odebranie wejscia z aplikacji
                startData input = {static_cast<float>(ui->pos_x->value()),
                                   static_cast<float>(ui->pos_y->value()),
                                   static_cast<float>(ui->pos_z->value()),
                                   static_cast<float>(ui->vel_x->value()),
                                   static_cast<float>(ui->vel_y->value()),
                                   static_cast<float>(ui->vel_z->value()),
                                   static_cast<float>(ui->dt->value()),
                                   static_cast<float>(ui->timeout->value())};
                pHandler = new PhysicsHandler(this, input); //inicjalizacja workera watku fizyki
                //blokada ui
                ui->clear->setDisabled(true);
                ui->simbtn2->setDisabled(true);
                ui->numbers_grid->setDisabled(true);
                ui->startSIM->setText("...(zatrzymaj)");

                emit pHandler->policz();

                connect(pHandler, &PhysicsHandler::krokPoliczony, this, &MainWindow::physConsole);
                connect(pHandler, &PhysicsHandler::obliczone, wykres, &Chart3D::plotTrajectory);
                connect(pHandler, &PhysicsHandler::obliczone, this, [this](std::vector<Output> wyniki,bool result){
                    if(!wyniki.empty()){
                        simOutput = wyniki;
                        ui->sim_output->appendPlainText("Zapisano wyniki symulacji.");
                    }
                    //watek zakonczyl prace - usuniecie watku
                    pHandler->deleteLater();
                    pHandler = nullptr;
                    //podanie punktu ladowania (jesli znaleziono)
                    if(result){
                        ui->sim_output->appendPlainText("Znaleziono punkt lądowania! X: " + QString::number(wyniki.back().pos.x) + " Y: " + QString::number(wyniki.back().pos.y));
                    }
                    //reset ui
                    ui->clear->setDisabled(false);
                    ui->simbtn2->setDisabled(false);
                    ui->numbers_grid->setDisabled(false);
                    ui->startSIM->setText("Uruchom SIM");
                });
            }
        }
    }
}

void MainWindow::autoMode(){

    //odebranie wejscia z aplikacji
    startData input = {0,0,aktualna_ramka.GPS_alt,aktualna_ramka.GPS_speed_N,aktualna_ramka.GPS_speed_E,aktualna_ramka.GPS_speed_V,0.01,1000};
    //startData input = {0,0,aktualna_ramka.GPS_alt,40,80,aktualna_ramka.GPS_speed_V,0.01,1000};
    pHandler = new PhysicsHandler(this, input); //inicjalizacja workera watku fizyki

    emit pHandler->policz();

    connect(pHandler, &PhysicsHandler::krokPoliczony, this, &MainWindow::physConsole);
    connect(pHandler, &PhysicsHandler::obliczone, wykres, &Chart3D::plotTrajectory);
    connect(pHandler, &PhysicsHandler::obliczone, this, [this](std::vector<Output> wyniki,bool result){
        if(!wyniki.empty()){
            simOutput = wyniki;
            ui->sim_output->appendPlainText("Zapisano wyniki symulacji.");
        }
        QObject* senderObj = sender();
        if (senderObj) {
            senderObj->deleteLater();
        }

        // zmienna członkowska = nullptr TYLKO wtedy gdy ten sygnał pochodzi od aktualnie przypisanego handlera
        // (Zapobiega to usuwaniu referencji do nowego wątku przez stary, opóźniony sygnał)
        if (senderObj == pHandler) {
            pHandler = nullptr;
        }
        if(result){
            ui->sim_output->appendPlainText("Znaleziono punkt lądowania! X: " + QString::number(wyniki.back().pos.x) + " Y: " + QString::number(wyniki.back().pos.y));
            Marble::GeoDataPlacemark *pktL;
            // przybliżoną stała - ile metrów ma jeden stopień na równiku
            const double METRY_NA_STOPIEN = 111320.0;

            // szerokość geograficzna (Y)
            // zmiana latitude jest w miarę stała niezależnie od koordynatow
            double deltaLat = wyniki.back().pos.y / METRY_NA_STOPIEN;
            double lat1 = aktualna_ramka.GPS_lat + deltaLat;

            // długość geograficzna (X)
            // należy uwzględnić "zwężanie się" Ziemi za pomocą cosinusa z latitude.
            // funkcja cos() przyjmuje radiany, a nie stopnie!
            double latRad = aktualna_ramka.GPS_lat * (M_PI / 180.0);
            double metryNaStopienLon = METRY_NA_STOPIEN * std::cos(latRad);

            double deltaLon = wyniki.back().pos.x / metryNaStopienLon;
            double lon1 = aktualna_ramka.GPS_long + deltaLon;

            Marble::GeoDataCoordinates L(lon1,lat1,0.0,Marble::GeoDataCoordinates::Degree);
            map_widget_class->pktL->setCoordinate(L);
            map_widget_class->map_widget->model()->treeModel()->updateFeature(map_widget_class->pktL);
        }
    });

}


void MainWindow::on_actionUstawienia_triggered()
{
    SettingDialog okno_ustawien(this);
    if(okno_ustawien.exec() == QDialog::Accepted){
        QSettings settings;
        ui->konsola->setMaximumBlockCount(settings.value("TerraCan/logcount", 100000).toInt());
    }
}

void MainWindow::on_pushButton_clicked()
{
    //int index = ui->stackedWidget->currentIndex();
    //ui->stackedWidget->setCurrentIndex(index + 1);
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_powrot_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_auto_mode_toggled(bool checked)
{
    if(checked){
        ui->numbers_grid->setDisabled(true);
    } else {
        ui->numbers_grid->setDisabled(false);
    }
}


void MainWindow::on_new_window_toggled(bool checked)
{
    if(checked){
        ui->new_window->setDisabled(true);
        sim_console = new simwindow(this);
        sim_console->setAttribute(Qt::WA_DeleteOnClose);
        sim_console->podepnijDokument(ui->sim_output->document());
        sim_console->show();
        connect(sim_console, &simwindow::destroyed, this, [this](){
            ui->new_window->setCheckState(Qt::Unchecked);
            ui->new_window->setDisabled(false);
        });
    }
}

