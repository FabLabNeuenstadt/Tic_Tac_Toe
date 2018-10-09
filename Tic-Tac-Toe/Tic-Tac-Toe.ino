#define BUTTON_PIN A7
#define DEBUG 0

struct RGB {
  byte r;
  byte g;
  byte b;
};

byte anodePin[3];
byte kathodeRPin[3];
byte kathodeGPin[3];
byte kathodeBPin[3];

// Die 9 Felder des Spielfelds.
// Werte: 0 => Noch niemand das Feld belegt
//        1 => Spieler 1 hat das Feld belegt
//        2 => Spieler 2 hat das Feld belegt
short Spielfeld[9];

RGB Ausgabe[10];

RGB FarbeSpieler[3];

short AktiverSpieler = 0;

int TasteZuLED( int taste ) {
    switch (taste) {
      case 1: return 3;
      case 2: return 2;
      case 3: return 1;
      case 4: return 4;
      case 5: return 5;
      case 6: return 6;
      case 7: return 9;
      case 8: return 8;
      case 9: return 7;
    }
    return 0;
}

int TasteZuSpielfeld( int taste ) {
  return TasteZuLED(taste)-1;
}

// Alle Pins, die mit den LEDs verbunden sind, auf Ausgang setzen
void InitEA( void )
{
  for (int pin = 2; pin < 14; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  pinMode(A0, OUTPUT);
}

// Um alle LEDs auszuschalten, müssen die damit verbundenen Anoden auf HIGH (falls durch einen Transistor invertiert) bzw. LOW gesetzt werden.
void AllesAus( void )
{
  // Anoden
  for (int zeile = 0; zeile < 3; zeile++) {
    digitalWrite(anodePin[zeile], HIGH);
  }
  // Anode für D10
  digitalWrite(11, LOW);

  // Kathoden
  for (int spalte = 0; spalte < 3; spalte++) {
    digitalWrite(kathodeRPin[spalte], HIGH);
    digitalWrite(kathodeGPin[spalte], HIGH);
    digitalWrite(kathodeBPin[spalte], HIGH);
  }
}

//  Zu Beginn eines Spiels werden alle Variablen auf ihren Standardwert gesetzt
void InitVariablen( void )
{
  // Alle Elemente des Arrays Spielfeld werden mit 0 belegt
  for (int i = 0; i < 10; i++)
  {
    Spielfeld[i] = 0;
    Ausgabe[i] = FarbeSpieler[0];
  }
  
  // Wenn nichts anderweitig festgesetzt wurde, beginnt Spieler 1
  if (!AktiverSpieler)
  {
    AktiverSpieler = 1;
  }

}

// Diese Funktion kümmert sich um die Ausgabe der Status-LED
void AusgabeStatusLED( void )
{
  // Je nachdem, welcher Spieler gerade dran ist, leuchtet die LED in einer bestimmten Farbe
  SetzeLED(10, FarbeSpieler[AktiverSpieler].r, FarbeSpieler[AktiverSpieler].g, FarbeSpieler[AktiverSpieler].b);
}

// Diese Funktion gibt den Inhalt des Arrays Ausgabe auf der LED Matrix aus.
void AusgabeLEDs()
{
  // Da die LEDs im Vergleich zum Spielfeld um 90° gedreht sind, müssen wir hier ein wenig rechnen
  for (int zeile = 0; zeile < 3; zeile++) {
    SetzeZeile(zeile, Ausgabe[zeile], Ausgabe[zeile+3], Ausgabe[zeile+6]);
  }
}

// Nach jedem Spielzug muss geprüft werden ob es einen Gewinner gibt
// Dazu werden die Werte in dem Array zeilenweise, spaltenweise und diagonal multipliziert
// Ein Produkt ist 1, genau dann wenn alle drei Faktoren 1 sind, alle Felder von Spieler 1 belegt wurden und dieser gewonnen hat.
// Ein Produkt ist 8, genau dann wenn alle drei Faktoren 2 sind, alle Felder von Spieler 2 belegt wurden und dieser gewonnen hat.

// Rückgabewerte:
// -1 => Unentschieden
// 0  => Spiel läuft noch, niemand hat gewonnen
// 1  => Spieler 1 hat gewonnen
// 2  => Spieler 2 hat gewonnen

int PruefeGewinner( void )
{
  int Produkt = 1;

  // Prüfe alle 3 Zeilen und Spalten
  for(int i = 0; i < 3; i++)
  {
    int Spalte = i;
    Produkt = Spielfeld[ 0 + Spalte ] * Spielfeld[ 3 + Spalte ] * Spielfeld[ 6 + Spalte ];
    if(Produkt == 1)
    {
      return 1;
    }
    if(Produkt == 8)
    {
      return 2;
    }
    
    int Zeile = i;
    Produkt = Spielfeld [ 0 + Zeile * 3 ] * Spielfeld[ 1 + Zeile * 3 ] * Spielfeld[ 2 + Zeile * 3 ];
    if(Produkt == 1)
    {
      return 1;
    }
    if(Produkt == 8)
    {
      return 2;
    }
  }

  // Prüfe die 2 Diagonalen
  Produkt = Spielfeld[0] * Spielfeld[4] * Spielfeld[8];
  if(Produkt == 1)
  {
    return 1;
  }
  if(Produkt == 8)
  {
    return 2;
  }

  Produkt = Spielfeld[2] * Spielfeld[4] * Spielfeld[6];
  if(Produkt == 1)
  {
    return 1;
  }
  if(Produkt == 8)
  {
    return 2;
  }

  // Prüfe, ob alle Felder belegt sind. Dies ist genau dann der Fall, wenn keins der Felder 0 ist
  // und somit das Produkt aller Felder nicht 0 ist
  for(int feld = 0; feld < 9; feld++)
  {
    Produkt *= Spielfeld[feld];
  }
  if(Produkt != 0)
  {
    return -1;
  }

  return 0;
}

// Die Setup-Funktion wird beim ersten Start aufgerufen
void setup() {
  InitEA();
  AllesAus();
  InitVariablen();

  // Kathoden der Spalten sind gleich
  kathodeRPin[0] = 12;
  kathodeRPin[1] =  8;
  kathodeRPin[2] =  2;
  kathodeGPin[0] = A0;
  kathodeGPin[1] =  7;
  kathodeGPin[2] =  3;
  kathodeBPin[0] = 13;
  kathodeBPin[1] =  5;
  kathodeBPin[2] =  4;

  // Anoden der Zeilen sind gleich
  anodePin[0] = 10;
  anodePin[1] =  9;
  anodePin[2] =  6;

  FarbeSpieler[0].r =   0;
  FarbeSpieler[0].g =   0;
  FarbeSpieler[0].b =   0;

  FarbeSpieler[1].r = 255;
  FarbeSpieler[1].g =   0;
  FarbeSpieler[1].b =   0;

  FarbeSpieler[2].r =   0;
  FarbeSpieler[2].g = 255;
  FarbeSpieler[2].b =   0;

  if (DEBUG)
  {
    Serial.begin(9600);
  }
}

// Die Loop Funktion wird endlos durchlaufen
void loop() {

  // ermittle, ob eine Taste gedrückt wurde
  int Taste = TasteGedrueckt();
  int Gewinner;

  // Falls eine Taste gedrückt wurde und dieses Feld noch unbelegt ist, markiere es mit dem aktuellen Spieler
  if ((Taste) && (Spielfeld[ TasteZuSpielfeld(Taste) ] == 0))
  {
    Spielfeld[ TasteZuSpielfeld(Taste) ] = AktiverSpieler;
    Ausgabe[ TasteZuSpielfeld(Taste) ] = FarbeSpieler[AktiverSpieler];

    // Nach jedem gültigen Spielzug ist der neue Spieler an der Reihe. Dafür wird der Spieler zuerst inkrementiert und danach ein Überlauf geprüft
    AktiverSpieler++;
    if (AktiverSpieler > 2)
    {
      AktiverSpieler = 1;
    }

    if (DEBUG)
    {
      Serial.println("Spielfeld:");
      for(int feld = 0; feld < 9; feld++)
      {
        if (feld%3 == 2) {
          // Wir sind am Ende einer Zeile
          Serial.println(Spielfeld[feld]);
        } else {
          Serial.print(Spielfeld[feld]);
        }
      }
      Serial.print("Aktiver Spieler: ");
      Serial.println(AktiverSpieler);
    }
  }


  // An der Status-LED wird gezeigt, welcher Spieler gerade an der Reihe ist
  AusgabeStatusLED();

  // Der aktuelle Spielstand wird auf der LED Matrix ausgegeben
  AusgabeLEDs();

  // Es wird geprüft, ob es schon einen Gewinner gibt
  Gewinner = PruefeGewinner();

  if (DEBUG && Gewinner)
  {
    Serial.print("Status Gewinner: ");
    Serial.println(Gewinner);
  }

  // Aktiviere passende Animation, falls das Spiel durch Sieg oder Unentschieden beendet wurde
  if(Gewinner > 0)
  {
    SpielGewonnen(Gewinner);
  }
  else if(Gewinner == -1)
  {
    SpielUnentschieden();
  }

}
