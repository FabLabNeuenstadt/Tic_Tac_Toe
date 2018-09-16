#define BUTTON_PIN A7
#define DEBUG 0

struct RGB {
  byte r;
  byte g;
  byte b;
};

int cycle = 0;
int letzteTaste = 0;

int Taste2LED( int button ) {
    switch (button) {
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


void SetLED( int LED, byte r, byte g, byte b) {
  int anode; // gemeinsame Anode der LED
  RGB kathode;
  int num = LED - 1;
  if (num >= 0 && num <= 8)
  {
    // Spielfeld-LED
    int zeile = num % 3;
    int spalte = num / 3;
    switch(zeile)
    {
      case 0: anode = 10; break;
      case 1: anode = 9; break;
      case 2: anode = 6; break;
    }
    switch(spalte)
    {
      case 0: kathode.r = 12; kathode.g = A0; kathode.b = 13; break;
      case 1: kathode.r =  8; kathode.g =  7; kathode.b =  5; break;
      case 2: kathode.r =  2; kathode.g =  3; kathode.b =  4; break;
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
    // Der PIN, der diese Anode ansteuert, hat keinen Transistor
    digitalWrite(anode, HIGH);
    delay(1);
    digitalWrite(anode, LOW);
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
  // Anoden
  digitalWrite(6, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);

  // Kathoden
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(A0, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
}

// Diese Funktion liefert zurück, welche Taste atuell gedrückt wird.
// Alle Tasten bilden einen Spannungsteiler, mit dem das Verhältnis der Widerstände zueinander über einen AD-Wandler ausgelesen wird.
// Falls keine Taste gedrückt wird, zieht der Pulldown-Widerstand die Spannung auf 0V
int TasteGedrueckt( void )
{
  // 0 bedeutet, dass keine Taste gedrückt wird
  int Taste = 0;

  int ADC_Wert = analogRead( A7 );

  // Im Folgenen werden die Werte des AD-Wandlers verglichen. daraus wird dann ermittelt, welche taste gedrückt wird
  
  if ( (ADC_Wert > 845) && (ADC_Wert < 860) ) {
    Taste = 1;
  } else if ( (ADC_Wert > 725) && (ADC_Wert < 735) ) {
    Taste = 2;
  } else if ( (ADC_Wert > 635) && (ADC_Wert < 645) ) {
    Taste = 3;
  } else if ( (ADC_Wert > 560) && (ADC_Wert < 570) ) {
    Taste = 4;
  } else if ( (ADC_Wert > 500) && (ADC_Wert < 515) ) {
    Taste = 5;
  } else if ( (ADC_Wert > 460) && (ADC_Wert < 470) ) {
    Taste = 6;
  } else if ( (ADC_Wert > 415) && (ADC_Wert < 430) ) {
    Taste = 7;
  } else if ( (ADC_Wert > 390) && (ADC_Wert < 400) ) {
    Taste = 8;
  } else if ( (ADC_Wert > 360) && (ADC_Wert < 370) ) {
    Taste = 9;
  }
  
  if (DEBUG && Taste)
  {
    Serial.print("Gemessener Wert: ");
    Serial.print(ADC_Wert);
    Serial.print(" - Gedrückte Taste: ");
    Serial.println( Taste );
  }

  return Taste;
}

// Die Setup-Funktion wird beim ersten Start aufgerufen
void setup() {
  InitIO();
  AllesAus();
  if (DEBUG)
  {
    Serial.begin(9600);
  }
  for (int i = 1; i < 11; i++) {
    SetLED(i, 255, 0, 0);
    delay(100);
  }
  for (int i = 1; i < 11; i++) {
    SetLED(i, 0, 255, 0);
    delay(100);
  }
  for (int i = 1; i < 11; i++) {
    SetLED(i, 0, 0, 255);
    delay(100);
  }
}

void loop() {
  int Taste = TasteGedrueckt();
  if (Taste) {
    if (cycle == 0) {
      SetLED(Taste2LED(Taste), 255, 0, 0);
      SetLED(10, 255, 0, 0);
    } else if (cycle == 1) {
      SetLED(Taste2LED(Taste), 0, 255, 0);
      SetLED(10, 0, 255, 0);
    } else if (cycle == 2) {
      SetLED(Taste2LED(Taste), 0, 0, 255);
      SetLED(10, 0, 0, 255);
    }
    if (Taste != letzteTaste) {
      cycle = ++cycle % 3;
      letzteTaste = Taste;
    }
    
  } else {
    AllesAus();
  }

}
