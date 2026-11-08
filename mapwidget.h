#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QObject>
#include <marble/MarbleWidget.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataLineString.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/MarbleDirs.h>
#include <marble/MarbleModel.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/GeoDataLineStyle.h>


class MapWidget : public QObject
{
    Q_OBJECT
public:
    explicit MapWidget(QObject *parent = nullptr);
    Marble::MarbleWidget *map_widget;
    Marble::GeoDataDocument *route_document;
    Marble::GeoDataLineString *trajektoria;
    Marble::GeoDataPlacemark *aktualna_pozycja;
    Marble::GeoDataPlacemark *pktL;
    Marble::GeoDataPlacemark* znacznik_linii;

private:

    //QString offline_path = QCoreApplication::applicationDirPath() + "/marbledata";
    QString offline_path = "C:/Users/frank/Documents/CanSat27/TerraCan_MissionControl/marble/data";
    QString plugins_path = "C:/Users/frank/Documents/CanSat27/TerraCan_MissionControl/marble/plugins";
signals:
};

#endif // MAPWIDGET_H
