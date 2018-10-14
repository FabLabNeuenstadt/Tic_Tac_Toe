# Einführung in (Arduino)-Programmierung

## Allgemein Programmierung:

Algorithmus, Rechenvorschrift erklären

Basics:
* ((Kommentare))
* Variablen
* Funktionen
* Operatoren
* Verzweigungen
* (Wiederholungen)

Wichtig: Abstraktion, generische Vorgehensweise

Computer machen grundsätzlich das gleiche wie ihr mit dem Taschenrechner, nur unglaublich schnell
1 GHz => 1 Milliarde (1 mit 9 Nullen!) Takte pro Sekunde
Mein Telefon: Quad Core, 2x 2,2 GHz, 2x 1,6 GHz, also (theoretisch 7,6e9) mit vielen Abstraktionsebenen
Arduino Nano (den wir später programmieren): 16 MHz, viel weniger, aber immerhin noch 1,6e7 Takte pro Sekunde, aber dafür wenig Abstraktionsebenen


## Beispiele in Pseudocode:

### Alle Zahlen bis 100 zusammenzählen:

#### Trivialer Ansatz:

```
Ergebnis = 1 + 2 + 3 + … + 100
```

#### Optimierung nach Gauß:

Erste + Letzte Zahl = Zweite + Vorletzte Zahl = Dritte + Drittletzte Zahl …
50 solcher Paare

```
Ergebnis = 101 * 50
```

### Allgemeinere Version: Alle Zahlen bis „Endzahl“ zusammenzählen

#### Trivialer Ansatz:

```
Funktion Zusammenzählen(Endzahl):
Ergebnis = 0
Sei Zähler nacheinander alle Zahlen von 1 bis Endzahl:
	Ergebnis = Ergebnis + Zähler
Gebe Ergebnis zurück

// ... Später ...

BisHundert = Zusammenzählen(100)
BisZweiHundert = Zusammenzählen(200)
```

#### Optimierung nach Gauß:

```
Funktion Zusammenzählen_Gauß(Endzahl):
// Gauß‘sche Summenformel
Ergebnis = (Endzahl * (Endzahl + 1)) / 2
Gebe Ergebnis zurück

// ... Später ...

BisHundert = Zusammenzählen_Gauß(100)
BisEineMillion = Zusammenzählen_Gauß(1000000)
```

Bemerkenswert: Egal wie groß die Zahl ist, die zweite, optimierte Funktion berechnet sie in einem Schritt, während die erste so viele Schritte braucht, wie groß die Zahl ist.

### Fibonacci-Zahlen

1, 1, 2, 3, 4, 8, 13, …

Oder, kürzer: Fang an mit 1, 1. Um die jeweils nächste Zahl zu berechnen, zähle die zwei Vorgängerzahlen zusammen

```
Funktion Fibonacci(Endzahl):
Wenn Endzahl = 1 oder Endzahl = 2:
	Gebe 1 zurück
Ansonsten:
	Gebe Fibonacci(Endzahl – 1) + Fibonacci(Endzahl – 2) zurück

// ... Später ...

AchteFibonacci = Fibonacci(8) // 21
```

Funktionen können sich selbst aufrufen. Aber Achtung, Endlosschleifengefahr!

## Weg von Pseudocode, hin zu C++

(Achtung, leider wird es jetzt ein wenig Englisch stellenweise)

### Allgemein

Groß/Kleinschreibung ist wichtig: Zahl ist nicht gleich zahl.
Bitte auf Umlaute, Sonderzeichen in Funktions- und Variablennamen verzichten. Idealerweise nur a-zA-Z.

Jeder Befehl wird durch ein ; abgeschlossen.

```C++
zahl = zahl + 10;
```

Codeblöcke werden von geschweiften Klammern umschlossen: `{}`

Programmierer fangen bei `0` an zu zählen, nicht bei `1`.

### Kommentare

Kommentare sind Briefe an euch selbst in der Zukunft.
Kommentare sollen beschreiben, warum Code etwas tut und nicht, was Code tut.
```C++
Befehl1; // Alles nach zwei Schrägstrichen wird für den Ablauf ignoriert

/*
Für größere Kommentare gibt es auch die Möglichkeit, mehrere Zeilen zu benutzen.
Alles, was zwischen Schrägstrich Stern und Stern Schrägstrich steht, wird für den Ablauf ignoriert.
*/
```

### Variablen
Variablen müssen definiert (deklariert) werden.

Der Übersicht halber gesammelt. Globale, lokale Variablen. Faustregel: So wenig globale Variablen wie möglich.

Verschiedene wichtige Variablentypen:
```C++
int Ganzzahl = 3;
float Kommazahl = 2.5;
string Zeichenfolge = „Test“;
bool Wahrheit = true;
```

Allgemein: Computer „mögen“ Ganzzahlen.
Faustregel: So oft wie möglich die Strukturen in Ganzzahlen abbilden macht Programme schneller :)

Um Listen von Variablen zu benutzen, gibt es sogenannte Arrays (Felder).

```C++
int DreiZahlen[3];
DreiZahlen[0] = 1;
DreiZahlen[1] = 5;
DreiZahlen[2] = 10;
```

Das geht auch mehrdimensional:

```C++
int Spielfeld[3][3];
// Spielfeld[0][0] entspricht dem Wert oben links
```

### Funktionen

Aufbau:
```C++
int Funktionsname(int Variable1, float Variable2)
{
	Befehl1;
	Befehl2;
	return 1;
}
```

Statt einem Variablentyp, kann am Anfang auch `void` stehen, das bedeutet, dass die Funktion keinen Wert zurückgibt.

Sobald die Ausführung einer Funktion auf den Befehl `return` stößt, wird das, was dahinter steht, als Rückgabewert zurückgegeben (außer, die Funktion hat den Typ `void`) und die Funktion nicht weiter ausgeführt.

Es gibt zwei spezielle Funktionen bei der Arduino-Programmierung:

```C++
void setup()
{
	// Diese Funktion wird einmalig beim Start oder Reset ausgeführt
}
```

und

```C++
void loop()
{
	// Diese Funktion wird nach setup() in einer Endlosschleife ausgeführt
}
```

### Operatoren
```C++
int zahl1 = 4;
int zahl2 = 9;
int summe = zahl1 + zahl2; // 13
int differenz = zahl1 – zahl2; // -5
int produkt = zahl1 * zahl2; // 36
int division = zahl1 / zahl2; // Achtung, Ganzzahlen, es wird abgerundet, also 2
int rest = zahl1 % zahl2; // 1

zahl1++; // zahl1 ist jetzt 5
zahl2--; // zahl2 ist jetzt 8

// Vergleichsoperatoren

zahl1 == 5; // wahr
zahl2 > 5; // wahr
zahl2 < 2; // falsch
zahl1 >= 6; // falsch
zahl2 <= 10; // wahr

// Logische Verknüpfungen
true && false; // UND: false
true || false; // OR: true
```

Außerdem sollten Klammern verwendet werden, um zu kennzeichnen, um die Reihenfolge der Auswertung zu beeinflussen bzw. klarzustellen.

### Verzweigungen

```C++
if (zahl1 > 5)
{
	zahl1 = 5;
}
else if (zahl1 < 0)
{
	zahl1 = 0;
}
else
{
	// nichts tun
}

switch (zahl1)
{
	case 1:
		Befehl1;
		break;
	case 2:
		Befehl2;
		break;
	default:
		Befehl_Allgemein;
		break;
}
```

### Wiederholungen
2 Hauptmöglichkeiten, Dinge zu wiederholen, for und while:

```C++
for (int Zaehler = 0; Zaehler <= Maximalwert; Zaehler++)
{
	// Dieser Block wird mehrfach ausgeführt
	// Die Variable Zaehler wird dabei hochgezählt
}

while (true)
{
	// Dieser Block wird immer wieder ausgeführt, solange
	// der Wert in den Klammern wahr ist.
	// In diesem Fall also bricht die Schleife niemals ab
}
```
