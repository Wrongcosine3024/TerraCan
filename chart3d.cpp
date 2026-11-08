#include "chart3d.h"
#include <qsettings.h>

Chart3D::Chart3D(QObject *parent, QWidget *widget)
    : QObject{parent}
{
    wykres = new Q3DScatter();
    //konfiguracja skali
    wykres->setAspectRatio(1.0f);
    wykres->setHorizontalAspectRatio(1.0f);

    seria = new QScatter3DSeries;

    // Konfiguracja osi Y - WYSOKOŚĆ
    QValue3DAxis *osPionowa = new QValue3DAxis;
    osPionowa->setTitle("Wysokość Z [m]");
    osPionowa->setTitleVisible(true);
    wykres->setAxisY(osPionowa);

    // Konfiguracja osi X i Z (Podłoga)
    QValue3DAxis *osX = new QValue3DAxis;
    osX->setTitle("Pozycja X [m]");
    osX->setTitleVisible(true);
    wykres->setAxisX(osX);

    QValue3DAxis *osZ = new QValue3DAxis;
    osZ->setTitle("Pozycja Y [m]");
    osZ->setTitleVisible(true);
    wykres->setAxisZ(osZ);

    wykres->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    seria->setMesh(QAbstract3DSeries::MeshSphere);
    seria->setItemSize(0.05f);
    QLinearGradient gradientOpadania;
    gradientOpadania.setColorAt(1.0, Qt::cyan);
    gradientOpadania.setColorAt(0.5, Qt::green);
    gradientOpadania.setColorAt(0.0, Qt::red);
    seria->setBaseGradient(gradientOpadania);
    seria->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    wykres->addSeries(seria);

    kontener3D = QWidget::createWindowContainer(wykres);
    kontener3D->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Chart3D::plotTrajectory(std::vector<Output> wyniki, bool czyZnalezionoPLadowania){
    if(wyniki.empty()) return;
    QSettings settings;
    int dzielnik = (settings.value("TerraCan/dzielnik", 100).toInt())/10;

    float minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;

    QScatterDataArray *punkty = new QScatterDataArray;
    punkty->reserve((wyniki.size()/dzielnik) + 1);

    for(int i = 0; i<wyniki.size(); i+=dzielnik){
        const Output klatka = wyniki[i];
        *punkty << QScatterDataItem(QVector3D(klatka.pos.x,klatka.pos.z,klatka.pos.y));

        // Zbieranie minimów i maksimów do skalowania
        if (i == 0) {
            minX = maxX = klatka.pos.x;
            minY = maxY = klatka.pos.z; // graficzne Y to fizyczna wysokość Z
            minZ = maxZ = klatka.pos.y; // graficzne Z to fizyczne Y
        } else {
            if (klatka.pos.x < minX) minX = klatka.pos.x;
            if (klatka.pos.x > maxX) maxX = klatka.pos.x;

            if (klatka.pos.z < minY) minY = klatka.pos.z;
            if (klatka.pos.z > maxY) maxY = klatka.pos.z;

            if (klatka.pos.y < minZ) minZ = klatka.pos.y;
            if (klatka.pos.y > maxZ) maxZ = klatka.pos.y;
        }
    }

    if(czyZnalezionoPLadowania){    //dodanie pkt lądowania
        const Output klatka = wyniki.back();
        *punkty << QScatterDataItem(QVector3D(klatka.pos.x,klatka.pos.z,klatka.pos.y));
    }

    float rozpietoscX = maxX - minX;
    float rozpietoscY = maxY - minY;
    float rozpietoscZ = maxZ - minZ;

    float maxRozpietosc = std::max({rozpietoscX, rozpietoscY, rozpietoscZ});

    // 10% marginesu
    maxRozpietosc *= 1.1f;

    float srodekX = (maxX + minX) / 2.0f;
    float srodekY = (maxY + minY) / 2.0f;
    float srodekZ = (maxZ + minZ) / 2.0f;

    wykres->axisX()->setRange(srodekX - maxRozpietosc / 2.0f, srodekX + maxRozpietosc / 2.0f);
    wykres->axisY()->setRange(srodekY - maxRozpietosc / 2.0f, srodekY + maxRozpietosc / 2.0f);
    wykres->axisZ()->setRange(srodekZ - maxRozpietosc / 2.0f, srodekZ + maxRozpietosc / 2.0f);

    seria->dataProxy()->resetArray(punkty);
}
