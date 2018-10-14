#define BUTTON_PIN A7
#define DEBUG false

#include "Ausgabe.h"
#include "Spiel.h"

/*
 * Spielfeldwerte:
 *    0 => Noch niemand hat das Feld belegt
 *    1 => Spieler 1 hat das Feld belegt
 *    2 => Spieler 2 hat das Feld belegt
 */

int Spielfeld[3][3]; // [Zeile][Spalte]
int AktiverSpieler = 0;

//  Zu Beginn eines Spiels werden alle Variablen auf ihren Standardwert gesetzt
void ResetSpiel( void )
{
  // Alle Elemente des Spielfelds werden mit 0 belegt
  for (int Zeile = 0; Zeile < 3; Zeile++)
  {
    for (int Spalte = 0; Spalte < 3; Spalte++)
    {
      Spielfeld[Zeile][Spalte] = 0;
    }
  }
  
  AktiverSpieler = 1; // Spieler 1 beginnt
}

/*
 * Rückgabewerte:
 * 255 => Unentschieden
 * 0  => Spiel läuft noch, niemand hat gewonnen
 * 1  => Spieler 1 hat gewonnen
 * 2  => Spieler 2 hat gewonnen
 */
int PruefeGewinner()
{
  // Wir multiplizieren die Werte aller Zeilen, Spalten und Diagonalen.
  // 
  // Wenn einer der Ergebniswerte 1 ist, wissen wir, dass alle 3 Werte 1 waren
  // (1*1*1) und somit Spieler 1 gewonnen hat
  //
  // Wenn einer der Ergebniswerte 8 ist, wissen wir, dass alle 3 Werte 2 waren
  // (2*2*2) und somit Spieler 2 gewonnen hat
  //
  // Wenn alle Ergebnisse einer Zeile oder Spalte miteinander multipliziert
  // einen Wert != 0 ergeben, wissen wir, dass alle Felder belegt sind

  int GesamtProdukt = 1;
  int Produkt = 1;
  
  for (int Zaehler = 0; Zaehler < 3; Zaehler++) {
    // Spalte überprüfen
    Produkt = Spielfeld[Zaehler][0] * Spielfeld[Zaehler][1] * Spielfeld[Zaehler][2];
    if (Produkt == 1)
    {
      return 1;
    }
    else if (Produkt == 8)
    {
      return 2;
    }
    
    // Wir multiplizieren alle Werte aller Spalten miteinander
    GesamtProdukt = GesamtProdukt * Produkt;

    // Zeile überprüfen
    Produkt = Spielfeld[0][Zaehler] * Spielfeld[1][Zaehler] * Spielfeld[2][Zaehler];
    if (Produkt == 1)
    {
      return 1;
    }
    else if (Produkt == 8)
    {
      return 2;
    }

  }

  // Diagonalen überprüfen
  {
    Produkt = Spielfeld[0][0] * Spielfeld[1][1] * Spielfeld[2][2];
    if (Produkt == 1)
    {
      return 1;
    }
    else if (Produkt == 8)
    {
      return 2;
    }
    
    Produkt = Spielfeld[0][2] * Spielfeld[1][1] * Spielfeld[2][0];
    if (Produkt == 1)
    {
      return 1;
    }
    else if (Produkt == 8)
    {
      return 2;
    }
  }

  if (GesamtProdukt == 0)
  {
    // Mindestens ein Feld wurde noch nicht besetzt
    return 0;
  }
  else
  {
    // Alle Felder wurden besetzt und weil wir im Code an diese Stelle gelangt sind,
    // hat niemand gewonnen, folglich ist die Runde unentschieden
    return 255;
  }
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
  if (taste != KeineKoordinaten && Spielfeld[taste.X][taste.Y] == 0 && AktiverSpieler != 0)
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
    if (AktiverSpieler == 1)
    {
      AktiverSpieler = 2;
    }
    else
    {
      AktiverSpieler = 1;
    }
  }

  ZeigeSpielfeld();
  AusgabeStatusLED(); // An der Status-LED wird gezeigt, welcher Spieler gerade an der Reihe ist

  // Es wird geprüft, ob es schon einen Gewinner gibt
  int Gewinner = PruefeGewinner();

  // Aktiviere passende Animation, falls das Spiel durch Sieg oder Unentschieden beendet wurde
  if(Gewinner != 0)
  {
    if(DEBUG)
    {
      Serial.print("Gewinner:");
      Serial.println(Gewinner);
    }
    
    AktiverSpieler = 0;
    if((Gewinner > 0) && (Gewinner <= 2)) // es gibt einen Gewinner
    {
      SpielGewonnen(Gewinner);
    }
    else // Spiel läuft nicht mehr es gibt aber auch keinen Gewinner
    {
      SpielUnentschieden();
    }
  }
}
