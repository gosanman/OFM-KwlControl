### Nacht und Zwangsbetriebsart melden ihren Zustand zurück

Wie bei den Zwangsobjekten, für die Objekte „Nacht“ und „Zwangsbetriebsart“.

* **Nein** (Vorgabe): reine Eingänge.
* **Ja**: Ein-/Ausgang. Setzt eine Zwangsbetriebsart oder Betriebsart die Nacht zurück, sendet
  „Nacht“ eine 0. Läuft die Zwangsbetriebsart ab oder setzt eine Betriebsart sie zurück, sendet
  „Zwangsbetriebsart“ eine 0. Empfangene Werte werden nicht zurückgeschickt, beim Start wird
  nichts gesendet.

**Vorsicht bei zentralen Adressen.** Hängt „Nacht“ an einer Tag/Nacht-Adresse für das ganze Haus,
würde die 0 dort alle anderen Geräte auf Tag schalten. Dann diese Einstellung auf „Nein“ lassen –
oder das Objekt über eine eigene Adresse bzw. das Logikmodul (internes KO) versorgen.
