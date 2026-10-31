### Objekt Handbetrieb

Bedeutung des Objekts „Handbetrieb aktiv“, als Eingang und als Ausgang.

* **1 = Handbetrieb** (Vorgabe): 1 hält die gerade laufende Stufe als Handstufe fest, 0 beendet
  den Handbetrieb. Das Objekt meldet 1, solange der Handbetrieb läuft.
* **1 = Automatik**: für Bedienfelder mit einer Taste „Automatik“. 1 beendet den Handbetrieb, 0
  hält die laufende Stufe fest. Das Objekt heißt dann „Automatik aktiv“ und meldet 1, solange die
  Automatik bestimmt.

Das Objekt meldet auch, wenn der Handbetrieb von selbst endet, nach Ablauf von „Handbetrieb
beendet nach“ oder durch einen Betriebsartwechsel. Ein Bedienfeld zeigt damit immer den
richtigen Zustand.
