#ifndef CHART3D_H
#define CHART3D_H

#include <QObject>
#include <QVector3D>
#include <QWidget>
#include <QtDataVisualization>

class Chart3D : public QObject
{
    Q_OBJECT
public:
    explicit Chart3D(QObject *parent = nullptr, QWidget *widget = nullptr);
    Q3DScatter *wykres;
    QWidget *wykres_widget;

signals:
};

#endif // CHART3D_H
