#include "simwindow.h"
#include "ui_simwindow.h"
#include <QScrollBar>
#include <qtimer.h>


simwindow::simwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::simwindow)
{
    ui->setupUi(this);
}

simwindow::~simwindow()
{
    delete ui;
}

void simwindow::clearconsole(){
    ui->sim_console->clear();
}

void simwindow::odswiez(){
    ui->sim_console->repaint();
}

void simwindow::podepnijDokument(QTextDocument *wspoldzielonyDokument) {
    ui->sim_console->setDocument(wspoldzielonyDokument);
}