#define BUTTON_PIN A7
#define DEBUG 0

int farbwechsel = 0;

int letzteTaste = 0;

int TasteZuLED( int button ) {
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


void SetzeLED( int led, byte r, byte g, byte b) {
  int anode; // gemeinsame Anode der LED
  byte kathodeR;
  byte kathodeG;
  byte kathodeB;
  
  //RGB kathode;
  int num = led - 1;
  if (num >= 0 && num <= 8)
  {
    // Spielfeld-LED
    int zeile = num % 3;
    int spalte = num / 3;
    switch(zeile)
    {
      case 0: anode = 10; if (DEBUG) Serial.print("z1"); break;
      case 1: anode = 9;  if (DEBUG) Serial.print("z2"); break;
      case 2: anode = 6;  if (DEBUG) Serial.print("z3"); break;
    }
    switch(spalte)
    {
      case 0: kathodeR = 12; kathodeG = A0; kathodeB = 13; if (DEBUG) Serial.println("s1"); break;
      case 1: kathodeR =  8; kathodeG =  7; kathodeB =  5; if (DEBUG) Serial.println("s2"); break;
      case 2: kathodeR =  2; kathodeG =  3; kathodeB =  4; if (DEBUG) Serial.println("s3"); break;
    }

    // Der PIN, der die Anode ansteuert, muss wegen Transistoren invertiert werden
    digitalWrite(anode, HIGH);
    digitalWrite(kathodeR, (r ? LOW : HIGH));
    analogWrite(anode, 255-r);
    delay(1);
    digitalWrite(anode, HIGH);
    digitalWrite(kathodeG, (g ? LOW : HIGH));
    analogWrite(anode, 255-g);
    delay(1);
    digitalWrite(anode, HIGH);
    digitalWrite(kathodeB, (b ? LOW : HIGH));
    analogWrite(anode, 255-b);
    delay(1);
    digitalWrite(anode, HIGH);

/*    digitalWrite(anode, LOW);
    delay(1);
    digitalWrite(anode, HIGH);*/
    digitalWrite(kathodeR, HIGH);
    digitalWrite(kathodeG, HIGH);
    digitalWrite(kathodeB, HIGH);
  }
  else if (num == 9)
  {
    // Status-LED
    anode = 11;
    kathodeR = 8;
    kathodeG = 7;
    kathodeB = 5;

    // Der PIN, der diese Anode ansteuert, hat keinen Transistor

    digitalWrite(anode, LOW);
    digitalWrite(kathodeR, (r ? LOW : HIGH));
    analogWrite(anode, r);
    delay(1);
    digitalWrite(anode, LOW);
    digitalWrite(kathodeG, (g ? LOW : HIGH));
    analogWrite(anode, g);
    delay(1);
    digitalWrite(anode, LOW);
    digitalWrite(kathodeB, (b ? LOW : HIGH));
    analogWrite(anode, b);
    delay(1);
    digitalWrite(anode, LOW);
    digitalWrite(kathodeR, HIGH);
    digitalWrite(kathodeG, HIGH);
    digitalWrite(kathodeB, HIGH);
  }
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

  // Im Folgenen werden die Werte des AD-Wandlers verglichen. daraus wird dann ermittelt, welche Taste gedrückt wird
  
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

// Die Setup-Funktion wird nur einmal beim ersten Start aufgerufen
void setup() {
  InitEA();
  AllesAus();
  if (DEBUG)
  {
    Serial.begin(9600);
  }
  for (int i = 1; i < 11; i++) {
    SetzeLED(i, 255, 0, 0);
    delay(100);
  }
  for (int i = 1; i < 11; i++) {
    SetzeLED(i, 0, 255, 0);
    delay(100);
  }
  for (int i = 1; i < 11; i++) {
    SetzeLED(i, 0, 0, 255);
    delay(100);
  }
}

// Die Loop-Funktion wird nach der Setup-Funktion in einer Dauerschleife ausgeführt

void loop() {
  int taste = TasteGedrueckt();
  if (taste) {
    if (farbwechsel == 0) {
      SetzeLED(TasteZuLED(taste), 255, 0, 0);
      SetzeLED(10, 255, 0, 0);
    } else if (farbwechsel == 1) {
      SetzeLED(TasteZuLED(taste), 0, 255, 0);
      SetzeLED(10, 0, 255, 0);
    } else if (farbwechsel == 2) {
      SetzeLED(TasteZuLED(taste), 0, 0, 255);
      SetzeLED(10, 0, 0, 255);
    }
    if (taste != letzteTaste) {
      farbwechsel++;
      if (farbwechsel > 2) {
        farbwechsel = 0;
      }
      letzteTaste = taste;
    }
    
  } else {
    AllesAus();
  }

}
