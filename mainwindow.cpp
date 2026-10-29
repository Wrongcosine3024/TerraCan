#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    radio = new OdbiornikCom(this);
    ui->setupUi(this);
    ui->com_list->clear();
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {     // listowanie portów com
        QString etykieta = info.portName() + " (" + info.description() + ')';
        radio->avalaiblePorts.push_back(info.portName());
        ui->com_list->addItem(etykieta);
    }
    connect(radio, &OdbiornikCom::parsedData, this, &MainWindow::refreshConsole);
}

MainWindow::~MainWindow()
{
    radio->serialPort->close();
    delete ui;
}

void MainWindow::on_com_list_itemClicked(QListWidgetItem *item)     //wybór portów com
{
    QString selectedCOM;
    selectedCOM = item->text();
    radio->index = ui->com_list->row(item);
    ui->port_label->setText(selectedCOM);
}


void MainWindow::on_COM_connect_toggled(bool checked)               // łączenie z portem com
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    if(checked){
        if(!radio->openCOM(radio->avalaiblePorts[radio->index])){
            ui->COM_connect->setChecked(false);
        }else{
            ui->COM_connect->setText("POŁĄCZONO!");
            ui->konsola->appendPlainText("(LOG) " + timestamp + ": Połączono z portem " + radio->avalaiblePorts[radio->index]);
            ui->com_list->setDisabled(1);
        }
    }else{
        radio->serialPort->close();
        ui->COM_connect->setText("NIE POŁĄCZONO");
        ui->konsola->appendPlainText("(LOG) " + timestamp + ": Rozłączono z portem " + radio->avalaiblePorts[radio->index]);
        ui->com_list->setDisabled(0);
    }
}

void MainWindow::refreshConsole(const TELEMETRY &telemetria){
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->konsola->appendPlainText("(LOG) " + timestamp + ": Nr.Pakietu " + QString::number(telemetria.SEQ) + " Timestamp mikrokontrolera: " + QString::number(telemetria.timestamp) + " Temperatura: " + QString::number(telemetria.temperature) + " Ciśnienie: " + QString::number(telemetria.pressure));
}