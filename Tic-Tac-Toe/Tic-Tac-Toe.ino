#define BUTTON_PIN A7
#define DEBUG true

struct RGB {
  byte R;
  byte G;
  byte B;
};

byte anodePin[3];
byte kathodeRPin[3];
byte kathodeGPin[3];
byte kathodeBPin[3];

// Die 9 Felder des Spielfelds.
// Werte: 0 => Noch niemand das Feld belegt
//        1 => Spieler 1 hat das Feld belegt
//        2 => Spieler 2 hat das Feld belegt
enum Spieler {
  None = 0b00000000,
  A = 0b00000001,
  B = 0b00000010
};
Spieler Spielfeld[3][3]; // [x][y]

RGB Bild[3][3];

struct Koordinaten {
  byte X;
  byte Y;

  static bool Equals(Koordinaten *left, Koordinaten *right)
  {
    return left->X == right->X && left->Y == right->Y;
  }
  bool operator ==(Koordinaten right)
  {
      return Equals(this, &right);
  }
  bool operator !=(Koordinaten right)
  {
      return !Equals(this, &right);
  }
};
Koordinaten KeineKoordinaten = {255,255};

Spieler AktiverSpieler = Spieler::None;

// Alle Pins, die mit den LEDs verbunden sind, auf Ausgang setzen
void InitEA( void )
{
  for (int pin = 2; pin < 14; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  pinMode(A0, OUTPUT);
}

//  Zu Beginn eines Spiels werden alle Variablen auf ihren Standardwert gesetzt
void InitVariablen( void )
{
  // Alle Elemente des Spielfelds werden mit 0 belegt
  for (int x = 0; x < 3; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      Spielfeld[x][y] = Spieler::None;
    }
  }
  
  // Wenn nichts anderweitig festgesetzt wurde, beginnt Spieler 1
  AktiverSpieler = Spieler::A;
}

// Rückgabewerte:
// 255 => Unentschieden
// 0  => Spiel läuft noch, niemand hat gewonnen
// 1  => Spieler 1 hat gewonnen
// 2  => Spieler 2 hat gewonnen
Spieler PruefeGewinner()
{
  // Spalten prüfen:
  for(int x = 0; x < 3; x++)
  {
    Spieler gewinner = Spielfeld[x][0] & Spielfeld[x][1] & Spielfeld[x][2];
    if(gewinner != Spieler::None)
      return gewinner;
  }

  // Zeilen prüfen:
  for(int y = 0; y < 3; y++)
  {
    Spieler gewinner = Spielfeld[0][y] & Spielfeld[1][y] & Spielfeld[2][y];
    if(gewinner != Spieler::None)
      return gewinner;
  }

  // Diagonalen prüfen:
  {
    Spieler gewinner;
    gewinner = Spielfeld[0][0] & Spielfeld[1][1] & Spielfeld[2][2];
    if(gewinner != Spieler::None)
      return gewinner;
    gewinner = Spielfeld[0][2] & Spielfeld[1][1] & Spielfeld[2][0];
    if(gewinner != Spieler::None)
      return gewinner;
  }

  // Prüfe ob alle Felder gefüllt und dennoch kein Gewinner -> Unentschieden:
  for(int x = 0; x < 3; x++)
  {
    for(int y = 0; y < 3; y++)
    {
      if(Spielfeld[x][y] == Spieler::None)
        return 0;
    }
  }
  // Jedes Feld ist durch einen Spieler belegt, aber kein Gewinner steht fest:
  return 255;
}

// Die Setup-Funktion wird beim ersten Start aufgerufen
void setup()
{
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
  
  InitEA();
  InitVariablen();
  LedAusgabe();

  if (DEBUG)
  {
    Serial.begin(9600);
    Serial.println("Hallo vom TicTacToe!");
  }
}

// Die Loop Funktion wird endlos durchlaufen
void loop()
{
  Koordinaten taste = TasteGedrueckt();
  if (taste != KeineKoordinaten && Spielfeld[taste.X][taste.Y] == Spieler::None && AktiverSpieler != Spieler::None)
  {
    if(DEBUG)
    {
      Serial.print("Taste gedrückt:");
      Serial.print(taste.X);
      Serial.println(taste.Y);
    }
    
    // Der Platz der gedrückten Taste auf dem Spielfeld ist noch frei
    // -> Belegen des Platzes mit dem aktuellen Spieler.
    Spielfeld[taste.X][taste.Y] = AktiverSpieler;

    // nächsten aktiven Spieler auswählen:
    if (AktiverSpieler == Spieler::A)
    {
      AktiverSpieler = Spieler::B;
    }
    else
    {
      AktiverSpieler = Spieler::A;
    }
  }

  ZeigeSpielfeld();
  AusgabeStatusLED(); // An der Status-LED wird gezeigt, welcher Spieler gerade an der Reihe ist

  // Es wird geprüft, ob es schon einen Gewinner gibt
  Spieler Gewinner = PruefeGewinner();

  // Aktiviere passende Animation, falls das Spiel durch Sieg oder Unentschieden beendet wurde
   if(Gewinner != Spieler::None)
  {
    if(DEBUG)
    {
      Serial.print("Gewinner:");
      Serial.println(Gewinner);
    }
    
    AktiverSpieler = Spieler::None;
    if(Gewinner > 0) // es gibt einen Gewinner
    {
      SpielGewonnen(Gewinner);
    }
    else // Spiel läuft nicht mehr es gibt aber auch keinen Gewinner
    {
      SpielUnentschieden();
    }
  }
}
