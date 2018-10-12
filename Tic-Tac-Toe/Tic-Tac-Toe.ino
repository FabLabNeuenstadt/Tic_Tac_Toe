#define BUTTON_PIN A7
#define DEBUG false

#include "Ausgabe.h"
#include "Spiel.h"

// Werte: 0 => Noch niemand hat das Feld belegt
//        1 => Spieler 1 hat das Feld belegt
//        2 => Spieler 2 hat das Feld belegt
Spieler Spielfeld[3][3]; // [x][y]
Spieler AktiverSpieler = Spieler::None;

//  Zu Beginn eines Spiels werden alle Variablen auf ihren Standardwert gesetzt
void ResetSpiel( void )
{
  // Alle Elemente des Spielfelds werden mit 0 belegt
  for (int x = 0; x < 3; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      Spielfeld[x][y] = Spieler::None;
    }
  }
  
  AktiverSpieler = Spieler::A; // Spieler 1 beginnt
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
  SetupEA();
  ResetSpiel();
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
