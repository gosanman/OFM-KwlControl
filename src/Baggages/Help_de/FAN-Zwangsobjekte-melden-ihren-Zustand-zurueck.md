### Zwangsobjekte melden ihren Zustand zurück

Ein Zwangsobjekt kann enden, ohne dass ein Telegramm kommt: wenn seine Laufzeit abläuft oder
wenn Nacht, Zwangsbetriebsart oder Betriebsart es zurücksetzen. Ein Bedienfeld, das „Aus“ (Ruhe)
gesendet hat, zeigt dann weiter „Aus“, obwohl die Lüftung längst wieder läuft.

* **Nein** (Vorgabe): Die Zwangsobjekte 1 bis 3 sind reine Eingänge.
* **Ja**: Die Objekte werden zum Ein-/Ausgang. Beendet die Lüftung einen Zwang selbst, sendet das
  Objekt den neuen Zustand – mit der eingestellten Polarität, bei „wirken bei 0“ also eine 1.
  Ein empfangener Wert wird nicht zurückgeschickt, und beim Start wird nichts gesendet. Das
  Objekt ist lesbar.

Damit kann der Status des Bedienfelds an dieselbe Gruppenadresse wie der Befehl.

Nur einschalten, wenn die Gruppenadresse zu diesem Raum gehört. Auf einer zentralen Adresse würde
die Rückmeldung alle anderen Teilnehmer mitschalten.
