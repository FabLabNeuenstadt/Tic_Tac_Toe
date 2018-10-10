void SpielGewonnen(Spieler Gewinner)
{
  InitVariablen();
}

void SpielUnentschieden()
{
  InitVariablen();
}

/*// Falls das Spiel gewonnen wurde, zeige eine dazugehörige Animation
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
      Ausgabe[feld] = FarbeSpieler[Gewinner];
    }
    Ausgabe[4] = FarbeSpieler[0]; // Mittlere LED

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt())
      {
        neuesSpiel = true;
      }
    }

    // Teil 2
    for(int feld = 0; feld < 9; feld++)
    {
      Ausgabe[feld] = FarbeSpieler[0];
    }
    Ausgabe[4] = FarbeSpieler[Gewinner]; // Mittlere LED

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt())
      {
        neuesSpiel = true;
      }
    }
  }while(!neuesSpiel);

  // Eine Taste wurde gedrückt, ein neues Spiel beginnt
  AllesAus();
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
    Ausgabe[0]=FarbeSpieler[1];
    Ausgabe[2]=FarbeSpieler[1];
    Ausgabe[6]=FarbeSpieler[1];
    Ausgabe[8]=FarbeSpieler[1];
    Ausgabe[4]=FarbeSpieler[0];
    Ausgabe[1]=FarbeSpieler[2];
    Ausgabe[3]=FarbeSpieler[2];
    Ausgabe[5]=FarbeSpieler[2];
    Ausgabe[7]=FarbeSpieler[2];

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt())
      {
        neuesSpiel = true;
      }
    }

    // Teil 2
    Ausgabe[0]=FarbeSpieler[2];
    Ausgabe[2]=FarbeSpieler[2];
    Ausgabe[6]=FarbeSpieler[2];
    Ausgabe[8]=FarbeSpieler[2];
    Ausgabe[4]=FarbeSpieler[0];
    Ausgabe[1]=FarbeSpieler[1];
    Ausgabe[3]=FarbeSpieler[1];
    Ausgabe[5]=FarbeSpieler[1];
    Ausgabe[7]=FarbeSpieler[1];

    Counter = millis();
    while((millis() - 1000) < Counter)
    {
      AusgabeLEDs();

      if(TasteGedrueckt())
      {
        neuesSpiel = true;
      }
    }
  }while(!neuesSpiel);

  AllesAus();
  InitVariablen();
}*/
