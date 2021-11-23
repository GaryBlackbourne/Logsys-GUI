# Logsys GUI fejlesztés linux driverhez

## nov.19

Project létrehozáskor Qt 5-öt használunk mert az a legujabb és az van telepítve, de amikor qmake-t hívunk akkor még nem tudja ezt a project file. Ezért mindenképpen bele kell pakolni a következő sort a .pro fileba (ha QtCreatort használunk akkor elvileg nem para):

`greaterThan(QT_MAJOR_VERSION, 4): QT += widgets`

QtCreatoros project sajna nem includeol mindent, ugyhogy a project fileban meg kell adni a következő include path-t:

`INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include`

Logsys a shared ibrarykat a `/usr/local/lib` könyvtárba pakolja, innen kell berántani majd, ha össze lehet lőni a qt-s projectel

C könvtár használata elvileg így lehetséges:

``` C
extern "C"
{
  #include "Label.h"
}
```
(forrás: https://stackoverflow.com/questions/19996323/use-c-shared-library-in-c-program?rq=1)

TODO: usbtest alapján kis C++ programot írni a Vcc kapcsolgatására, hogy megnézzük mennek e a shared libraryk

## nov. 20

A fordításhoz fontos az argumentek sorrendje. Csoki makefileja alapján, g++ al fordítani így lehet:

`g++ <C++ compile args> <source file> <linker args> -o <kimeneti állomány>`

forrás: https://stackoverflow.com/questions/1517138/trying-to-include-a-library-but-keep-getting-undefined-reference-to-messages

libusb linkeléséhez szükséges flageket megkaphatjuk innen is: `pkg-config --libs libusb-1.0`

Ez gyakorlatban a `-lusb-1.0` linker kapcsolóval lehet a libusb-t linkelni.

Fontos hogy a driver legyen telepítve, azaz a `liblogsys-drv.so` legyen valahol a linker default path-ben, különben ezt is külön kell specifikálni. A linker default path-t megtudhatjuk a következő parancsal: `ld --verbose | grep SEARCH_DIR | tr -s ' ;' \\012`. Amennyiben a driver telepítve van, úgy az `-l` kapcsolóval specifikálhatjuk a linkelni kívánt shared object filet. (`-llogsys-drv`)

## nov.22

A Qt projectben a QtCreator segít a header fileok hozzáadásában (jobb katt -> add existing files). A projektben használt librarykhoz a header fileok szükségesek, tehát a logsys driver `incude/logsys` mappáját egy az egyben át kell másolni a Qt project mappába.
Ezek után az `add existing files` parancsal hozzáadhatjuk őket a projekthez (de rendezve maradnak, juhuuu!)

A `libusb` library használatához a packaget fel kell telepíteni akármilyen package managerből, de ez a logsys drivernek is függősége, szóval remélhetőleg már telepítve van.

A Qt C++ alapú, ugyhogy a fent említett includeolást kell használnunk. (extern)

A linkeléshez a Qt `.pro` filejában a következő sort kell beírni a `libusb` és a `logsys-drv`linkeléséhez:

```
LIBS += -lusb-1.0 -llogsys-drv
```

Ezek után a forrásfileokban már használhatjuk mind a libusb, mind a logsys függvényeit, és struktúráit. 

todo: x gomb lekezelése: https://stackoverflow.com/questions/17480984/qt-how-do-i-handle-the-event-of-the-user-pressing-the-x-close-button
