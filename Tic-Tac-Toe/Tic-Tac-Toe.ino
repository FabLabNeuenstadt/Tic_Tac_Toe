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
  //TODO: Alle Elemente des Spielfelds werden mit 0 belegt

  //TODO: Variable AktiverSpieler setzen  
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
  //TODO: Überprüfen, ob es einen Gewinner gibt und den Rückgabewert entsprechend
  //      der obigen Definition auswählen
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
  // Die Variable taste hat nun 2 mögliche Zustände:
  // Entweder gilt (taste == KeineKoordinaten), dann wurde keine Taste gedrückt
  // Oder über taste.X und taste.Y kann die gedrückte Taste herausgefunden werden.
  
  //TODO: Überprüfen, ob eine Taste gedrückt wurde und ob das Feld der
  //      gedrückten Taste bereits belegt ist.
  //      Wenn ja, Feld entsprechend besetzen und aktiven Spieler wechseln

  // Diese Funktionen sind für die Ausgabe des Spielfelds und des aktiven Spielers zuständig
  ZeigeSpielfeld();
  AusgabeStatusLED();

  // Es wird geprüft, ob es schon einen Gewinner gibt
  int Gewinner = PruefeGewinner();

  //TODO: Aktiviere passende Animation, falls das Spiel beendet wurde
  //      Gewinner: Animation mit der Funktion SpielGewonnen(Gewinner) abspielen
  //      Unentschieden: Animation mit der Funktion SpielUnentschieden() abspielen
}
