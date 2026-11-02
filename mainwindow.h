#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlistwidget.h>
#include "odbiornikcom_worker.h"
#include "physicshandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void comlistrefresh();

private slots:
    void on_com_list_itemClicked(QListWidgetItem *item);

    void on_COM_connect_toggled(bool checked);

    void refreshConsole(const TELEMETRY &telemetria);

    void on_refresh_clicked();

    void connResult(bool result, QString result_string);

    void on_clear_clicked();

    void on_startSIM_clicked();

    void physConsole(QString line);

    void on_actionUstawienia_triggered();

private:
    Ui::MainWindow *ui;
    OdbiornikWorker *comWorker;
    PhysicsHandler *pHandler = nullptr;
};
#endif // MAINWINDOW_H
