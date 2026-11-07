#include "chart3d.h"

Chart3D::Chart3D(QObject *parent, QWidget *widget)
    : QObject{parent}
{
    wykres = new Q3DScatter();
    wykres_widget = widget;

}
