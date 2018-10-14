RGB GetSpielerfarbe(int spieler)
{
  switch(spieler)
  {
  case 0: return RGB {0,0,0};
  case 1: return RGB {255,0,0};
  case 2: return RGB {0,255,0};
  }
}

void LeereBild()
{
  for(byte x = 0; x < 3; x++)
  {
    for(byte y = 0; y < 3; y++)
    {
      Bild[x][y] = {0,0,0};
    }
  }
}

void ZeigeSpielfeld()
{ 
  for(byte x = 0; x < 3; x++)
  {
    for(byte y = 0; y < 3; y++)
    {
      Bild[x][y] = GetSpielerfarbe(Spielfeld[x][y]);
    }
  }

  LedAusgabe();
}

void LedAusgabe()
{
  for(byte y = 0; y < 3; y++)
  {
    byte anode = anodePin[y];

    digitalWrite(anode, HIGH);
    for(byte x = 0; x < 3; x++)
    {
      RGB farbe = Bild[x][y];
      digitalWrite(kathodeRPin[x], (farbe.R ? LOW : HIGH));
      digitalWrite(kathodeGPin[x], (farbe.G ? LOW : HIGH));
      digitalWrite(kathodeBPin[x], (farbe.B ? LOW : HIGH));
    }
    digitalWrite(anode, LOW);
    delay(1); // gewählte PINs für 1ms leuchten lassen
    digitalWrite(anode, HIGH);
  }
}

// Diese Funktion kümmert sich um die Ausgabe der Status-LED
void AusgabeStatusLED( void )
{
  // Je nachdem, welcher Spieler gerade dran ist, leuchtet die LED in einer bestimmten Farbe
  RGB farbe = GetSpielerfarbe(AktiverSpieler);

  // Status-LED
  byte anode = 11;
  int x = 1;

  // Der PIN, der diese Anode ansteuert, hat keinen Transistor
  digitalWrite(anode, LOW);
  digitalWrite(kathodeRPin[x], (farbe.R ? LOW : HIGH));
  digitalWrite(kathodeGPin[x], (farbe.G ? LOW : HIGH));
  digitalWrite(kathodeBPin[x], (farbe.B ? LOW : HIGH));
  digitalWrite(anode, HIGH);
  delay(1);
  digitalWrite(anode, LOW);
}
