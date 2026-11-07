#include "mapwidget.h"
#include <marble/MarbleDirs.h>

MapWidget::MapWidget(QObject *parent)
    : QObject{parent}
{
    //sciezka do mapy offline
    Marble::MarbleDirs::setMarbleDataPath(offline_path);
    Marble::MarbleDirs::setMarblePluginPath(plugins_path);

    map_widget = new Marble::MarbleWidget;
    map_widget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    map_widget->setProjection(Marble::Mercator);

}
