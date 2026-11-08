#include "mapwidget.h"
#include <marble/GeoDataStyle.h>
#include <marble/GeoDataIconStyle.h>
#include <marble/GeoDataLabelStyle.h>

MapWidget::MapWidget(QObject *parent)
    : QObject{parent}
{
    //sciezka do mapy offline
    Marble::MarbleDirs::setMarbleDataPath(offline_path);
    Marble::MarbleDirs::setMarblePluginPath(plugins_path);

    map_widget = new Marble::MarbleWidget;
    map_widget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    map_widget->setProjection(Marble::Mercator);

    route_document = new Marble::GeoDataDocument;

    aktualna_pozycja = new Marble::GeoDataPlacemark;
    aktualna_pozycja->setCoordinate(Marble::GeoDataCoordinates(19.94, 50.06, 0.0, Marble::GeoDataCoordinates::Degree));
    aktualna_pozycja->setName("TerraCan");

    pktL = new Marble::GeoDataPlacemark;
    pktL->setCoordinate(Marble::GeoDataCoordinates(0, 0, 0.0, Marble::GeoDataCoordinates::Degree));
    pktL->setName("Symulacja - przewidywane lądowanie");

    //stylowanie pkt lądowania
    // Marble::GeoDataStyle::Ptr stylPinezki(new Marble::GeoDataStyle());
    // Marble::GeoDataIconStyle ikonaStyl;
    // ikonaStyl.setColor(Qt::red);
    // //ikonaStyl.setIconPath("data/bitmaps/default_location.png");
    // stylPinezki->setIconStyle(ikonaStyl);
    // Marble::GeoDataLabelStyle tekstStyl;
    // tekstStyl.setAlignment(Marble::GeoDataLabelStyle::Right);
    // stylPinezki->setLabelStyle(tekstStyl);

    //pktL->setStyle(stylPinezki);

    znacznik_linii = new Marble::GeoDataPlacemark;
    znacznik_linii->setName("Trajektoria");

    trajektoria = new Marble::GeoDataLineString;

    //stylowanie trajektorii
    Marble::GeoDataStyle::Ptr styleLinii(new Marble::GeoDataStyle);
    Marble::GeoDataLineStyle pedzelLinii(Qt::red); // Kolor linii
    pedzelLinii.setWidth(4);                       // Grubość w pikselach
    styleLinii->setLineStyle(pedzelLinii);
    znacznik_linii->setStyle(styleLinii);

    znacznik_linii->setGeometry(trajektoria);

    route_document->append(aktualna_pozycja);
    route_document->append(pktL);
    route_document->append(znacznik_linii);
    map_widget->model()->treeModel()->addDocument(route_document);
}
