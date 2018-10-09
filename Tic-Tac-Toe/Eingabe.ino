// Diese Funktion liefert zurück, welche Taste aktuell gedrückt wird.
// Alle Tasten bilden einen Spannungsteiler, mit dem das Verhältnis der Widerstände zueinander über einen AD-Wandler ausgelesen wird.
// Falls keine Taste gedrückt wird, zieht der Pulldown-Widerstand die Spannung auf 0V
int TasteGedrueckt( void )
{
  // 0 bedeutet, dass keine Taste gedrückt wird
  int taste = 0;

  int ADC_Wert = analogRead( A7 );

  // Im Folgenen werden die Werte des AD-Wandlers verglichen. daraus wird dann ermittelt, welche Taste gedrückt wird
  
  if ( (ADC_Wert > 845) && (ADC_Wert < 860) ) {
    taste = 1;
  } else if ( (ADC_Wert > 725) && (ADC_Wert < 735) ) {
    taste = 2;
  } else if ( (ADC_Wert > 635) && (ADC_Wert < 645) ) {
    taste = 3;
  } else if ( (ADC_Wert > 560) && (ADC_Wert < 570) ) {
    taste = 4;
  } else if ( (ADC_Wert > 500) && (ADC_Wert < 515) ) {
    taste = 5;
  } else if ( (ADC_Wert > 460) && (ADC_Wert < 470) ) {
    taste = 6;
  } else if ( (ADC_Wert > 415) && (ADC_Wert < 430) ) {
    taste = 7;
  } else if ( (ADC_Wert > 390) && (ADC_Wert < 400) ) {
    taste = 8;
  } else if ( (ADC_Wert > 360) && (ADC_Wert < 370) ) {
    taste = 9;
  }
  
  if (DEBUG && taste)
  {
    Serial.print("Gemessener Wert: ");
    Serial.print(ADC_Wert);
    Serial.print(" - Gedrückte Taste: ");
    Serial.println( taste );
  }

  return taste;
}
