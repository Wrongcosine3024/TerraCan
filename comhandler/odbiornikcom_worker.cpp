#include "odbiornikcom_worker.h"

OdbiornikWorker::OdbiornikWorker(QObject *parent){
    watek = new QThread(this);
    radio = new OdbiornikCom(nullptr);
    radio->moveToThread(watek);
    connect(radio, &OdbiornikCom::parsedData, this, &OdbiornikWorker::newData);
    connect(this, &OdbiornikWorker::spodlaczCom, radio, &OdbiornikCom::podlaczCom);
    connect(radio, &OdbiornikCom::connResult, this, &OdbiornikWorker::connResult);
    connect(this, &OdbiornikWorker::srozlaczCom, radio, &OdbiornikCom::rozlaczCom);
    connect(watek, &QThread::finished, radio, &QObject::deleteLater);
    watek->start();
}

OdbiornikWorker::~OdbiornikWorker(){
    watek->quit();
    watek->wait();
    radio->deleteLater();
}

void OdbiornikWorker::podlaczCom(const QString &portname){
    emit spodlaczCom(portname);
}

void OdbiornikWorker::rozlaczCom(){
    emit srozlaczCom();
}