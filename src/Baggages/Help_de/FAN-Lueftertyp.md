### Lüftertyp

Bauart des angeschlossenen Lüfters. Sie bestimmt die Kennlinie, die Zahl der
belegten DAC-Kanäle und ob der Lüfter überhaupt umkehrbar ist.

* **e²60** — Pendelpartner möglich.
* **ego** — Pendelt intern und braucht keinen Partner; Stufe 4 ist der Abluftstoß.
* **RA 15-60** — 0 V ist aus, kein Pendelbetrieb.
* **generisch bipolar** — beliebiger Lüfter mit Stellsignal um 5 V; alle Spannungen frei
  parametrierbar.
* **generisch unipolar** — beliebiger Lüfter mit 0–10 V; 0 V ist aus.

> Für e²60, ego und RA 15-60 liegen gemessene Kennlinien in der Firmware. Sie erscheinen nur, wenn
> „Kennlinie anpassen" eingeschaltet ist.

> Ein unbekannter Typ (nur aus einem beschädigten Parameterabbild möglich) gilt als bipolar:
> Der Kanal bleibt auf 5,00 V, der Lüfter läuft nicht, Fehlercode 3.
