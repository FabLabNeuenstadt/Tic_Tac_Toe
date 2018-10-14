int Gewinner;

void ZeigeAnimation(void (*Animation)(byte Schritt), byte SchrittZahl, unsigned long SchrittDauer = 250)
{
  byte AnimationsSchritt = 0;
  unsigned long BeginLetzterSchritt = millis();

  unsigned long ZeitTastenfreigabe = millis() + 500;

  while(true)
  {
    unsigned long AktuelleZeit = millis();
    
    LeereBild();
  
    (*Animation)(AnimationsSchritt); // Zeige eigentliche Animation über übergebene Funktion
  
    if(AktuelleZeit >= BeginLetzterSchritt + SchrittDauer)
    {
      AnimationsSchritt++;
      if(AnimationsSchritt>SchrittZahl-1)
        AnimationsSchritt = 0; 
  
      BeginLetzterSchritt = AktuelleZeit;
    }
  
    LedAusgabe();
  
    if(TasteGedrueckt() != KeineKoordinaten && AktuelleZeit >= ZeitTastenfreigabe)
    {
      ResetSpiel();
      delay(500);
      return;
    }
  }
}

void GewonnenAnimation(byte Schritt)
{
  switch(Schritt)
    {
    case 0:
    break;
    case 1:
      Bild[1][1] = GetSpielerfarbe(Gewinner);
    break;
    case 2:
      for(byte x = 0; x < 3; x++)
        for(byte y = 0; y < 3; y++)
          if(x!=1 || y!=1) // alle außer außer die mittlere LED
            Bild[x][y] = GetSpielerfarbe(Gewinner);
    break;
    }
}

void SpielGewonnen(int gewinner)
{
  Gewinner = gewinner;
  ZeigeAnimation(GewonnenAnimation, 3);
}

void UnentschiedenAnimation(byte Schritt)
{
  switch(Schritt)
    {
    case 0:
    break;
    case 1:
      Bild[1][1] = {255,255,255};
    break;
    case 2:
      for(byte x = 0; x < 3; x++)
        for(byte y = 0; y < 3; y++)
          if(x!=1 || y!=1) // alle außer außer die mittlere LED
            Bild[x][y] = {255,255,255};
    break;
    }
}

void SpielUnentschieden()
{
  ZeigeAnimation(UnentschiedenAnimation, 3);
}
