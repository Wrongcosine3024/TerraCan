#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlistwidget.h>
#include "odbiornikcom.h"

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
    void logMessage(const QString &message);

private slots:
    void on_com_list_itemClicked(QListWidgetItem *item);

    void on_COM_connect_toggled(bool checked);

    void refreshConsole(const TELEMETRY &telemetria);

private:
    Ui::MainWindow *ui;
    OdbiornikCom *radio;
};
#endif // MAINWINDOW_H
