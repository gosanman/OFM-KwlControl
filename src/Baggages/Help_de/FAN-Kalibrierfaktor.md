### Kalibrierfaktor

Korrigiert den Skalenfehler dieses DAC-Kanals. 10000 bedeutet 1,0000,
also keine Korrektur.

Der Wert stammt aus der Inbetriebnahmemessung: Ausgang auf einen bekannten Sollwert stellen,
nachmessen, Verhältnis eintragen. Er wirkt auf alle Stufen dieses Kanals gleichmäßig.

Der Faktor gehört zum einzelnen Ausgang. Der Konfigurationstransfer lässt ihn deshalb aus: Beim
Kopieren, Tauschen, Zurücksetzen oder Importieren eines Lüfters bleibt er unverändert.

> Ein Wert außerhalb von 9800 … 10200 kann nur aus einem beschädigten Parameterabbild kommen.
> Die Firmware fährt den Lüfter dann nicht, hält seinen Kanal auf 5,00 V und meldet
> Fehlercode 3.
