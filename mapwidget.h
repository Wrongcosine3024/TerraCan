#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QObject>
#include <marble/MarbleWidget.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataLineString.h>
#include <marble/GeoDataPlacemark.h>

class MapWidget : public QObject
{
    Q_OBJECT
public:
    explicit MapWidget(QObject *parent = nullptr);
    Marble::MarbleWidget *map_widget;

private:
    Marble::GeoDataDocument *route_document;
    Marble::GeoDataLineString *trajektoria;
    //QString offline_path = QCoreApplication::applicationDirPath() + "/marbledata";
    QString offline_path = "C:/Users/frank/Documents/CanSat27/TerraCan_MissionControl/marble/data";
    QString plugins_path = "C:/Users/frank/Documents/CanSat27/TerraCan_MissionControl/marble/plugins";
signals:
};

#endif // MAPWIDGET_H
