### Klartext-Status senden

Schaltet ein Textobjekt für diesen Raum frei („Status Klartext“, DPT 16.001, 14 Zeichen). Es
sagt, welche Stufe gerade läuft und warum, und sendet bei jeder Änderung.

| Text | Bedeutung |
|---|---|
| S0 Sperre | Fenster oder Kamin sperrt |
| S0 Frostschutz / S1 Hitzeschutz | Schutz greift |
| S4 Bad | Abluftanforderung läuft |
| S2 Zuluft | ein Partner lüftet ab, dieser Raum strömt nach |
| S2 Verbund | Slave folgt dem Master |
| S3 Hand | Handstufe, endet nach der eingestellten Zeit |
| S3 Feuchte, S2 CO2, S2 Luftgüte, S3 Kühlen, S2 Warmluft | Automatik: diese Führung treibt die Stufe |
| S2 Komfort, S1 Standby, S1 Nacht … | Automatik auf der Grundstufe der Betriebsart |

Die Reihenfolge ist dieselbe wie bei der Stufenbildung: Das erste Zutreffende gewinnt. Im
Nachlauf nennt der Text weiter die Führung, die ihn ausgelöst hat.
