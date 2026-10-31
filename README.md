# OFM-KwlControl — Lüftungsmodul für OpenKNX

OpenKNX-Funktionsmodul für dezentrale Pendellüfter mit 0–10-V-Stellsignal. Es
enthält die gesamte Lüftungslogik und die ETS-Teilapplikation. Entwickelt an e²60,
ego und RA 15-60.

Dieses Repository ist eine **Bibliothek**, keine Firmware. Gebaut wird im
Geräteprojekt [OAM-KwlControl](https://github.com/gosanman/OAM-KwlControl), das
dieses Modul unter `lib/OFM-KwlControl` einbindet. Hier liegen außerdem die
Anwenderdokumentation und die Logiktests.

Version: **0.6** (siehe [library.json](library.json))

> **5,00 V ist Stillstand. 0 V ist Volllast.** Das Stellsignal der bipolaren Geräte
> ist um 5 V aufgebaut. Ein DAC-Code 0 heißt volle Drehzahl, nicht „aus". Vor dem
> Anschließen bitte [Bevor Sie anschließen](doc/Applikationsbeschreibung-Kwl.md#bevor-sie-anschließen)
> lesen.

## Was das Modul tut

Zwei OpenKNX-Module mit drei Ebenen:

| Modul | Präfix | Ebene | Anzahl |
|---|---|---|---|
| `KwlRoomModule` | `ROOM` | **Raum:** Sensoren, Betriebsarten, Bedienung, Führungen | bis 8 |
| `KwlFanModule` | `FAN` | **Verbund:** Stufenregel, Takt, Richtung, Wärmerückgewinnung durch Pendeln | bis 8 |
| | | **Lüfter:** Typ, Kennlinie, Stellkanal, Kalibrierung, Leitungskompensation | bis 12 |

Ein Lüfter gehört zu genau einem Raum und einem Verbund. Das Lüftermodul liest die
Anforderungen der Räume direkt im Prozess, nicht über den Bus. Die ETS blendet nur
so viele Lüfter ein, wie die gewählte Platine Kanäle hat.

Die wirksame Stufe (0…4) entsteht an genau einer Stelle (`StageArbiter`), mit fester
Vorfahrt: Sperre → Schutz → Zu-/Abluftanforderung → Verbund → Hand → Automatik.
Darunter liegen die Betriebsart-Ebenen Zwangsobjekte → Nacht → Zwangsbetriebsart →
Betriebsart → Standard.

## Quellen

| Datei | Inhalt |
|---|---|
| `KwlRoomModule`, `KwlRoom` | Raum-Kanäle: Eingänge, Betriebsarten, Führungen, Rückmeldungen |
| `KwlFanModule`, `KwlFan` | Lüfter-Kanäle, Verbünde, Start- und Fehlerverhalten |
| `KwlGroup` | Verbund: Stufenregel, Takt, Phase, Nachströmung |
| `StageArbiter` | Vorfahrt und Rücksetzen, die einzige Quelle der wirksamen Stufe |
| `StageLadder`, `StageMap` | Grenzwert-Treppe für Führungen, Prozent ↔ Stufe mit Hysterese |
| `TempControl`, `MoistAir` | Temperaturführung, Dampfdruck und Feuchtevergleich |
| `KwlCurve`, `CableComp` | Kennlinie Typ × Stufe × Richtung → Volt, Leitungskompensation |
| `DirectionGuard` | Richtungswechsel nur über 5,00 V und Totzeit |
| `ChannelPlan` | Prüfung der Kanalbelegung: außerhalb der Platine, ego-Paar, Doppelbelegung |
| `KwlOutput`, `IDacDrive`, `Drive/` | Ausgabeschicht und GP8413-Treiber (`MockDacDrive` für Tests) |
| `SendCondition`, `StatusText` | Sendebedingung mit Totband, Klartext-Status |
| `Room.*.xml`, `Fan.*.xml` | ETS-Teilapplikation für den OpenKNXproducer |
| `Baggages/` | Hilfetexte (`Help_de/`), Icons und Bilder der ETS |

## Einbinden

Das Geräteprojekt muss bereitstellen:

- `knxprod.h` aus dem OpenKNXproducer, mit den Präfixen `ROOM` und `FAN`.
- `hardware.h` mit dem Board-Header. Das Modul erwartet `FANDRV_BOARD_ID`,
  `FANDRV_BOARD_CHANNELS`, `FANDRV_I2C_PORT`, `FANDRV_I2C_SDA`, `FANDRV_I2C_SCL`,
  `FANDRV_DAC_ADDR_A`, `FANDRV_DAC_ADDR_B` und `FANDRV_DAC_LEFT_ALIGNED`. Pins,
  Adressen und das Busformat der DACs stehen nur dort, nie im Modulcode.
- Einen RP2040 (der Treiber nutzt das Pico-SDK direkt für I²C mit Zeitlimit).

Registrierung in `main.cpp`, wie im OAM:

```cpp
openknx.addModule(1, openknxKwlRoomModule);
openknx.addModule(2, openknxKwlFanModule);
```

## Tests

```
pio test -e native
```

Prüft die hardwareunabhängige Logik ohne Arduino, KNX und Board-Header: Kennlinie,
Volt → DAC-Code mit Klemmung, Leitungskompensation, Vorfahrt, Grenzwert-Treppe,
Verbund, Richtungswechsel, Kanalbelegung, Sendebedingung, Klartext. Das ist die
Schicht, in der ein Fehler einen Lüfter auf Vollgas stellt; sie muss grün sein,
bevor Hardware-Code geändert wird. Voraussetzung ist ein GCC im Pfad (unter Windows
MinGW).

Das Zusammenspiel der Module prüft der Simulator im OAM-Repo (`sim/`).

## Bilder der ETS

Die Bilder in `src/Baggages/` werden erzeugt, nicht von Hand gezeichnet. Aufruf aus
dem Wurzelverzeichnis, Python mit Pillow:

```
python graphics/make_images.py     Hardware-Icons und Klemmenbild
python graphics/make_ramp.py       Diagramm Richtungswechsel
python graphics/make_modes.py      Diagramme der Seite Betriebsarten
python graphics/mdi.py <svg> ...   Modul-Icons aus graphics/mdi/
```

Die Diagramme sind 512 px breit, damit sie in der ETS auch mit Windows-Skalierung
nicht abgeschnitten werden.

## Dokumentation

[doc/Applikationsbeschreibung-Kwl.md](doc/Applikationsbeschreibung-Kwl.md) — für
Anwender: Inbetriebnahme, Betriebsarten, Vorfahrt, Führungen, Verbund und Takt,
Bedienfelder, alle Kommunikationsobjekte, Fehlercodes, Konsole und was das Gerät
nicht kann. Sie ist die Quelle der Hilfetexte in `src/Baggages/Help_de/`.

[doc/technical/Ablaufdiagramme.md](doc/technical/Ablaufdiagramme.md) — für
Entwickler: Schichten, Lebenszyklus, Start, Hauptschleife, Raumschleife, Vorfahrt,
Verbund, Takt und Totzeit, Abluftanforderung, Ausgabe bis zum DAC-Code, Master und
Slave, Fehlercodes und LED.

## Lizenz

GPL-3.0, siehe [LICENSE](LICENSE).
