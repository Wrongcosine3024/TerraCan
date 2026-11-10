# 🛰️ TerraCan Mission Control

[![Qt](https://img.shields.io/badge/Qt-6.x-41CD52?logo=qt&logoColor=white)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-20-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-Build-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

**TerraCan Mission Control** to wielowątkowa, wydajna stacja naziemna (Ground Station) stworzona w C++ i Qt6 dla misji **"TerraCan"**. Projekt jest rozwijany przez zespół w ramach europejskiego konkursu ESERO-Polska CanSat 2026/2027.

Aplikacja odpowiada za dwukierunkową komunikację z satelitą, walidację binarnych ramek danych, wizualizację trajektorii w przestrzeni 3D, mapowanie pozycji offline oraz symulację parametrów lotu w czasie rzeczywistym.

---

## ✨ Główne funkcjonalności

*   **Niezawodna telemetria (QSerialPort):** Odbiór danych z prędkością 115200 bps realizowany w dedykowanym wątku roboczym (`OdbiornikWorker`). Błyskawiczne dekodowanie ramek binarnych zabezpieczonych preambułą (`0xAA`) i weryfikowanych sumą kontrolną CRC.
*   **Mapy i nawigacja offline (KDE Marble):** Wbudowany, skompilowany ze źródeł silnik map KDE Marble zintegrowany bezpośrednio w Qt Widgets (bez QML). Rysowanie trasy lotu (`GeoDataLineString`) i pozycjonowanie na mapie (`GeoDataPlacemark`) z użyciem niestandardowych ikon w trybie całkowicie offline.
*   **Wizualizacja trajektorii 3D:** Renderowanie wykresów punktowych 3D w czasie rzeczywistym z użyciem `QtGraphs` / `QtDataVisualization`. Implementacja posiada 1:1 proporcjonalne skalowanie osi, customowe stylowanie (cienie, kolory) oraz inteligentny system throttlingu dla optymalizacji zużycia zasobów komputera.
*   **Wbudowana symulacja fizyki (3-DOF):** Moduł numerycznej symulacji spadku (wykorzystujący całkowanie Eulera i model gęstości atmosfery ISA), działający w odizolowanym wątku obliczeniowym. Pozwala na rygorystyczne testowanie stacji i zachowań aerodynamicznych przed właściwym startem.
*   **Zarządzanie bezpieczeństwem danych (Disaster Recovery):** Zrzut odbieranych punktów na bieżąco do plików **CSV i KML**, co gwarantuje pełne odzyskanie danych telemetrycznych w przypadku awarii zasilania laptopa w terenie.
*   **Rozbudowane UI/UX:** Interfejs Qt Widgets oparty na customowych arkuszach stylów (QSS) zaprojektowanych dla maksymalnej czytelności w warunkach polowych. Architektura UI zapewnia brak lagów dzięki przeniesieniu wszystkich ciężkich operacji (I/O, symulacja, parsowanie) do `QThreadPool` i `QtConcurrent`.

## ⚙️ Architektura Systemu i Hardware

TerraCan to cyfrowe serce systemu, ściśle współpracujące z hardwarem misji:
*   **Łączność LoRa:** Obsługa standardu 433 MHz (moduły SX1278) przy paśmie 125 kHz i mocy 20 dBm (100 mW).
*   **Tracker Antenowy:** Integracja ze zautomatyzowaną, dwuosiową stacją śledzącą (Antenna Tracker) napędzaną przez mikrokontroler ESP32, silniki krokowe NEMA 17 i precyzyjne sterowniki TMC2209.
*   **Architektura wątków:** Rygorystyczne zarządzanie drzewem obiektów Qt i przypisaniami wątków (`moveToThread`). Chroniony dostęp do zasobów za pomocą `QMutex` i `QMutexLocker`, co zapobiega crashom i wyciekom pamięci.

## 🚀 Budowanie Projektu

Projekt wykorzystuje system **CMake** oraz kompilator **MinGW**. Wymaga zainstalowanego środowiska Qt6 oraz przygotowanej biblioteki KDE Marble.

```bash
# 1. Klonowanie repozytorium
git clone https://github.com/TwojUsername/TerraCan.git
cd TerraCan

# 2. Tworzenie katalogu budowania
mkdir build && cd build

# 3. Generowanie plików Makefile z podaniem ścieżki Qt
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"

# 4. Wielowątkowa kompilacja
cmake --build . -j 10
```
> **Ważne:** Aby moduł map offline wyrenderował kafelki, upewnij się, że poprawnie skopiowałeś biblioteki współdzielone (`.dll`) do folderu wykonywalnego, a zmienna `marble_data` ma prawidłowo wskazaną ścieżkę lokalną przez `MarbleDirs::setMarbleDataPath()`.

## 👥 Zespół

Oprogramowanie powstaje jako fundamentalny element misji **TerraCan**, której dodatkowym celem obok badań atmosferycznych (ciśnienie/temperatura 1 Hz) jest:
*   Trójwymiarowa rekonstrukcja fotogrametryczna terenu z fazy opadania.
*   Obliczanie wskaźnika wegetacji NDVI (przy użyciu analizy spektralnej NIR/NoIR).
*   Analiza termiczna i geologiczna.
