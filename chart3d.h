#ifndef CHART3D_H
#define CHART3D_H

#include <QObject>
#include <QVector3D>
#include <QWidget>
#include <QtDataVisualization>
#include "physicsutils.h"
#include <vector>

class Chart3D : public QObject
{
    Q_OBJECT
public:
    explicit Chart3D(QObject *parent = nullptr, QWidget *widget = nullptr);

    // Funkcja zwracająca gotowy widget do wpięcia w układ
    QWidget* getWidget() const { return kontener3D; }
private:
    Q3DScatter *wykres;
    QScatter3DSeries *seria;
    QWidget *kontener3D;
public slots:
    void plotTrajectory(std::vector<Output> wyniki, bool czyZnalezionoPLadowania);

signals:
};

#endif // CHART3D_H
