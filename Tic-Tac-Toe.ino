#define BUTTON_PIN A7
#define DEBUG 0

struct RGB {
  byte r;
  byte g;
  byte b;
};

// Die 9 Felder des Spielfelds.
// Werte: 0 => Noch niemand das Feld belegt
//        1 => Spieler 1 hat das Feld belegt
//        2 => Spieler 2 hat das Feld belegt
short Spielfeld[9];
short AktiverSpieler = 0;

void SetLED( int num, byte r, byte g, byte b) {
  int anode; // gemeinsame Anode der LED
  RGB kathode;
  if (num >= 0 && num <= 8)
  {
    // Spielfeld-LED
    int zeile = num / 3;
    int spalte = num % 3;
    switch(zeile)
    {
      case 0: anode = 10; break;
      case 1: anode = 9; break;
      case 2: anode = 6; break;
    }
    switch(spalte)
    {
      case 0: kathode.r =  2; kathode.g =  3; kathode.b =  4; break;
      case 1: kathode.r =  8; kathode.g =  7; kathode.b =  5; break;
      case 2: kathode.r = 12; kathode.g = A0; kathode.b = 13; break;
    }
    digitalWrite(kathode.r, (r ? LOW : HIGH));
    digitalWrite(kathode.g, (g ? LOW : HIGH));
    digitalWrite(kathode.b, (b ? LOW : HIGH));
    // Der PIN, der die Anode ansteuert, muss wegen Transistoren invertiert werden
    digitalWrite(anode, LOW);
    delay(1);
    digitalWrite(anode, HIGH);
    digitalWrite(kathode.r, HIGH);
    digitalWrite(kathode.g, HIGH);
    digitalWrite(kathode.b, HIGH);
  }
  else if (num == 9)
  {
    // Status-LED
    anode = 11;
    kathode.r = 8;
    kathode.g = 7;
    kathode.b = 5;
    digitalWrite(kathode.r, (r ? LOW : HIGH));
    digitalWrite(kathode.g, (g ? LOW : HIGH));
    digitalWrite(kathode.b, (b ? LOW : HIGH));
    // Der PIN, der die Anode ansteuert, muss wegen Transistoren invertiert werden
    analogWrite(anode, HIGH);
    delay(1);
    analogWrite(anode, LOW);
    digitalWrite(kathode.r, HIGH);
    digitalWrite(kathode.g, HIGH);
    digitalWrite(kathode.b, HIGH);
  }
}

// Alle Pins, die mit den LEDs verbunden sind, auf Ausgang setzen
void InitIO( void )
{
  for (int pin = 2; pin < 15; pin++)
  {
    pinMode(pin, OUTPUT);
  }
}

// Um alle LEDs auszuschalten, müssen die damit verbundenen Anoden auf HIGH (falls durch einen Transistor invertiert) bzw. LOW gesetzt werden.
void AllesAus( void )
{
  digitalWrite(6, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
}

//  Zu Beginn eines Spiels werden alle Variablen auf ihren Standardwert gesetzt
void InitVariablen( void )
{
  // Alle Elemente des Arrays Spielfeld werden mit 0 belegt
  for (int i = 0; i < 9; i++)
  {
    Spielfeld[i] = 0;
  }
  
  // Wenn nichts anderweitig festgesetzt wurde, beginnt Spieler 1
  if (!AktiverSpieler)
  {
    AktiverSpieler = 1;
  }

}

// Diese Funktion liefert zurück, welche Taste atuell gedrückt wird.
// Alle Tasten bilden einen Spannungsteiler, mit dem das Verhältnis der Widerstände zueinander über einen AD-Wandler ausgelesen wird.
// Falls keine Taste gedrückt wird, zieht der Pulldown-Widerstand die Spannung auf 0V
int TasteGedrueckt( void )
{
  // 10 bedeutet, dass keine Taste gedrückt wird
  int Taste = 10;

  int ADC_Wert = analogRead( BUTTON_PIN );

  // Im Folgenden werden die Werte des AD-Wandlers verglichen. Daraus wird dann ermittelt, welche Taste gedrückt ist.
  
  if ( (ADC_Wert > 850) && (ADC_Wert < 860) )
  {
    Taste = 0;
  }
  if ( (ADC_Wert > 725) && (ADC_Wert < 740) )
  {
    Taste = 1;
  }
  if ( (ADC_Wert > 635) && (ADC_Wert < 645) )
  {
    Taste = 2;
  }
  if ( (ADC_Wert > 455) && (ADC_Wert < 470) )
  {
    Taste = 3;
  }
  if ( (ADC_Wert > 500) && (ADC_Wert < 515) )
  {
    Taste = 4;
  }
  if ( (ADC_Wert > 560) && (ADC_Wert < 575) )
  {
    Taste = 5;
  }
  if ( (ADC_Wert > 415) && (ADC_Wert < 430) )
  {
    Taste = 6;
  }
  if ( (ADC_Wert > 385) && (ADC_Wert < 395) )
  {
    Taste = 7;
  }
  if ( (ADC_Wert > 355) && (ADC_Wert < 370) )
  {
    Taste = 8;
  }
  
  if (DEBUG && Taste != 10)
  {
    Serial.print("Gemessener Wert: ");
    Serial.print(ADC_Wert);
    Serial.print(" - Gedrückte Taste: ");
    Serial.println( Taste );
  }

  return Taste;
}

// Diese Funktion kümmert sich um die Ausgabe der Status-LED
void AusgabeStatusLED( void )
{
  // Je nachdem, welcher Spieler gerade dran ist, leuchtet die LED in einer bestimmten Farbe
  switch (AktiverSpieler)
  {
    case 0: SetLED(9,   0,   0,   0); break; // Alle Farben aus
    case 1: SetLED(9, 255,   0,   0); break; // Rot
    case 2: SetLED(9,   0, 255,   0); break; // Grün
  }
}

// Diese Funktion gibt den Inhalt des Arrays Spielfeld auf der LED Matrix aus.
void AusgabeLEDs()
{
  for (int feld = 0; feld < 9; feld++)
  {
    switch ( Spielfeld[feld] )
    {
      case 0: SetLED(feld,   0,   0,   0); break; // Alle Farben aus
      case 1: SetLED(feld, 255,   0,   0); break; // Rot
      case 2: SetLED(feld,   0, 255,   0); break; // Grün
    }
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

// Falls das Spiel gewonnen wurde, zeige eine dazugehörige Animation
void SpielGewonnen( int Gewinner )
{
  bool neuesSpiel = false;

  // Zuerst bleibt die Anzeige für 2 Sekunden auf dem aktuellen Spielstand
  unsigned long Counter = millis();
  while((millis() - 2000) < Counter)
  {
    AusgabeLEDs();
  }

  // Solange keine Taste gedrückt wird, wird nun diese Animation angezeigt
  do
  {
    // Teil 1
    for(int feld = 0; feld < 9; feld++)
    {
      Spielfeld[feld] = Gewinner;
    }
    Spielfeld[4] = 0; // Mittlere LED

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt() != 10)
      {
        neuesSpiel = true;
      }
    }

    // Teil 2
    for(int feld = 0; feld < 9; feld++)
    {
      Spielfeld[feld] = 0;
    }
    Spielfeld[4] = Gewinner; // Mittlere LED

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt() != 10)
      {
        neuesSpiel = true;
      }
    }
  }while(!neuesSpiel);

  // Eine Taste wurde gedrückt, ein neues Spiel beginnt
  InitVariablen();
}

// Falls das Spiel unentschieden endet, zeige eine dazugehörige Animation
void SpielUnentschieden( void )
{
  bool neuesSpiel = false;

  // Zuerst bleibt die Anzeige für 2 Sekunden auf dem aktuellen Spielstand
  unsigned long Counter = millis();
  while((millis() - 2000) < Counter)
  {
    AusgabeLEDs();
  }

  // Solange keine Taste gedrückt wird, wird nun diese Animation angezeigt
  do
  { 
    // Teil 1
    Spielfeld[0]=1;
    Spielfeld[2]=1;
    Spielfeld[6]=1;
    Spielfeld[8]=1;
    Spielfeld[4]=0;
    Spielfeld[1]=2;
    Spielfeld[3]=2;
    Spielfeld[5]=2;
    Spielfeld[7]=2;

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt() != 10)
      {
        neuesSpiel = true;
      }
    }

    // Teil 2
    Spielfeld[0]=2;
    Spielfeld[2]=2;
    Spielfeld[6]=2;
    Spielfeld[8]=2;
    Spielfeld[4]=0;
    Spielfeld[1]=1;
    Spielfeld[3]=1;
    Spielfeld[5]=1;
    Spielfeld[7]=1;

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt() != 10)
      {
        neuesSpiel = true;
      }
    }
  }while(!neuesSpiel);
  
  InitVariablen();
}

// Die Setup-Funktion wird beim ersten Start aufgerufen
void setup() {
  InitIO();
  AllesAus();
  InitVariablen();
  if (DEBUG)
  {
    Serial.begin(9600);
  }
}

// Die Loop Funktion wird endlos durchlaufen
void loop() {

  // ermittle, ob eine Tastee geddrückt wurde
  int Taste = TasteGedrueckt();
  int Gewinner;

  // Falls eine Taste gedrückt wurde und dieses Feld noch unbelegt ist, markiere es mit dem aktuellen Spieler
  if ((Taste != 10) && (Spielfeld[ Taste ] == 0))
  {
    Spielfeld[ Taste ] = AktiverSpieler;

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
