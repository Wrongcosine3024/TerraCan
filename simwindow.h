#ifndef SIMWINDOW_H
#define SIMWINDOW_H

#include <QDialog>
#include <qtextdocument.h>

namespace Ui {
class simwindow;
}

class simwindow : public QDialog
{
    Q_OBJECT

public:
    explicit simwindow(QWidget *parent = nullptr);
    void podepnijDokument(QTextDocument *wspoldzielonyDokument);
    void odswiez();
    ~simwindow();
public slots:
    void clearconsole();
private:
    Ui::simwindow *ui;
};

#endif // SIMWINDOW_H
