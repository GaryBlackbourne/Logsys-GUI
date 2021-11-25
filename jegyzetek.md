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

## nov.23

A Qt classokon és widgeteken alapszik, a driver kezdeményező és bezáró függvényeit ezeknek a konstruktorába és destruktorába célszerű berakni.

Implementáltam a reset jelet vezérlő logikát, és egy gombhoz kötöttem a fő panelen. az eszköz RST lába nem vált feszültséget ennek a hatására, lehet hogy itt valami más van a háttérben.

Az INIT gomb egyenlőre kötelezően szükséges minden egyébhez, érdemes lenne valahogy egy hotpuggal implementálni ezt a működést, hogy ne kelljen nyomkodni, illetve ne lehessen elfelejteni. Kis segítség: https://stackoverflow.com/questions/8604081/can-linux-signal-my-qt-program-when-a-particular-usb-device-is-connected

A fő működés különböző `QWidget`-ek betöltésével lesz megvalósítva. Külön widget tartozik a konfigurációhoz, illetve a Bitbang, vagy USART kommunikációhoz.
Ha az inicializáció igényli, akkor lehet célszerű átszerkeszteni hogy három különböző widget tartozzon ugyanahhoz az eszközhöz. Vagy alkalmasint egy paraméterezhető konstruktorral lehet majd inicializálni a megfelelő felületet.

## nov.24

A Xilinx ISE linux alá történő telepítése szükséges az `impact` futtatásához. Ezt a binárist használja a logsys driver, hogy `bit` fileból `svf` file-t csináljon. Az ISE telepítéséhez Csókás Bence útmutatóját követtem, sajnos a `Qt4` már nem támogatott a 20.04-es Ubuntu alapú rendszereken, így a végleges beüzemelés sajnos további trükközést igényelhet. Az ise indítása terminálból lehetséges, a `/opt/Xilinx/14.7/ISE_DS/ISE/bin/lin64/ise` bináris indításával. A hibát a license manager dobta fel.

 Az impact viszont elérhető, így ezt használhatjuk. Az útmutató az alapértelmezett telepítési útvonalat javasolja, és a driver ez alapján keresi meg az `impact`-ot, ehhez egy környezeti változóra van szüksége:

```
XILINX=/opt/Xilinx/14.7/ISE_DS/ISE
```

A fenti változóban a XILINX verziója eltérhet a különböző telepítések között, erre érdemes figyelni.

A Xilinx nem ismeri a megfelelő package elnevezéseket, így a licensz ativálásához szükséges `xlcm` bináris futtatásához a következő tweakre van szükségünk:

```
cd /usr/lib/x86_64-linux-gnu
sudo ln -s libQtNetwork.so.4 libQt_Network.so

```
Ugyanis a Xilinx rossz néven keresi a libQtNetwork.so binárist.

Bekerült a konfigurációt kezelő widget az alklamazásba, majdnem egy az egyben az eredeti logsys-GUI-t másolja. A konfigurációhoz használt fileokat a GUI felismeri, `.bit` file esetén `impact` segítségével készít egy svf filet, amit programozásra használ. A programozáshoz használt függvények a `libxsvf` libraryból vannak, így a widget konstruktorához és destruktorához kapcsolt `begin` és `end` függvények nem teszik lehetővé a működést, ezt át kellett tenni a querry függvények mellé.

A driver helyes működéséhez az environment variable-k között szerepelnie kell a fent említett `XILINX` változónak. Ennek a hiányában a driver sajnos nem találja meg az impactot, és a `.bit` file alapú konfiguráció lehetetlenné válik. Ezt a QtCreator esetében a project fülön (nem a .pro file!!) a run panelen lehet beállítani, éles tesztelésre szükség van!

Todo: meg kell valósítani hogy a programozás valóban a kiválasztott hardverrel történjen, ez egyenlőre nincs így, a selectornak semmi hatása sincs.

## nov.25

TODO ban marad a logsys eszköz kiválasztása a kártya feltöltésnél, mert látszólag nincs lehetőség az eszköz kiválasztására a driverben, hanem hardcodeolva az FPGA a target. Mindazonáltal a JTAG ID-val kapcsolatban ez egy hasznos linknek tűnik: https://www.fpga4fun.com/JTAG3.html

A `libusb` inicializáció és bezárás átkerült a `mainwindow` konstruktorába, hiszen az usb eszközre a program kezdetétől fogva szükségünk van. A `mainwindow` tartalmaz tagváltozóként egy `backThread` típusu pointert ami saját osztály, publikus leszármazottja a `QThread` osztálynak.

Ez a `QThread` osztály alkalmas a Qt környezetben a threadkezelésre. Felüldefíniálható rajta egy `run()` függvény, amiben a thread valódi funkcióit valósíthatjuk meg. A thread a `start()` függvény hívásával indítható.

A `backThread` feladata az usb hotplug funkció megvalósítása (libusb függvény periodikus hívásával), illetve a megfelelő mérések ekérdezése (majdani implementáció alkalmával). A thread egy végtelen ciklusban végzi a feladatát, a thread megállítására a végtelen ciklust hajtó bool tagváltozó módosításával van lehetőség. A `backThread` konstruktora és destruktora felelős a logsys programozó felismeréséért, és felcsatolásáért, (`logsys_usb_open()`, `logsys_usb_close()`) valamint a hotplughoz szükséges callback függvény regisztrációjáért, és deregisztrációjáért.

GUI-ban szükséges kilépési, és egyéb hibaüzenetek kiírása a `QDebug` osztály segítségével lehetséges. Ennek a folyamatnak a legegyszerűbb megoldása, hogy egy általános debug objektumhoz interfészt biztosító függvényt hívunk, és mivel ezen az osztályon defíniálva van stream operátor, így a használata is felettébb egyszerű:
``` C++
qDebug() << "Hibaüzenet!";

```
