void SetzeZeile( int zeile, RGB s1, RGB s2, RGB s3) {
  int anode; // gemeinsame Anode der LED

  anode = anodePin[zeile];

  digitalWrite(anode, HIGH);
  digitalWrite(kathodeRPin[0], (s1.r ? LOW : HIGH));
  digitalWrite(kathodeGPin[0], (s1.g ? LOW : HIGH));
  digitalWrite(kathodeBPin[0], (s1.b ? LOW : HIGH));
  digitalWrite(kathodeRPin[1], (s2.r ? LOW : HIGH));
  digitalWrite(kathodeGPin[1], (s2.g ? LOW : HIGH));
  digitalWrite(kathodeBPin[1], (s2.b ? LOW : HIGH));
  digitalWrite(kathodeRPin[2], (s3.r ? LOW : HIGH));
  digitalWrite(kathodeGPin[2], (s3.g ? LOW : HIGH));
  digitalWrite(kathodeBPin[2], (s3.b ? LOW : HIGH));
  digitalWrite(anode, LOW);
  delay(1);
  digitalWrite(anode, HIGH);
}

void SetzeLED( int led, byte r, byte g, byte b) {
  int anode; // gemeinsame Anode der LED

  //RGB kathode;
  int num = led - 1;
  if (num >= 0 && num <= 8)
  {
    // Spielfeld-LED
    int zeile = num % 3;
    int spalte = num / 3;
    anode = anodePin[zeile];
    /*switch(zeile)
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
    }*/

    // Der PIN, der die Anode ansteuert, muss wegen Transistoren invertiert werden
    digitalWrite(anode, HIGH);
    digitalWrite(kathodeRPin[spalte], (r ? LOW : HIGH));
    analogWrite(anode, 255-r);
    delay(1);
    digitalWrite(anode, HIGH);
    digitalWrite(kathodeGPin[spalte], (g ? LOW : HIGH));
    analogWrite(anode, 255-g);
    delay(1);
    digitalWrite(anode, HIGH);
    digitalWrite(kathodeBPin[spalte], (b ? LOW : HIGH));
    analogWrite(anode, 255-b);
    delay(1);
    digitalWrite(anode, HIGH);

/*    digitalWrite(anode, LOW);
    delay(1);
    digitalWrite(anode, HIGH);*/
    digitalWrite(kathodeRPin[spalte], HIGH);
    digitalWrite(kathodeGPin[spalte], HIGH);
    digitalWrite(kathodeBPin[spalte], HIGH);
  }
  else if (num == 9)
  {
    // Status-LED
    anode = 11;
    int spalte = 1;

    // Der PIN, der diese Anode ansteuert, hat keinen Transistor

    digitalWrite(anode, LOW);
    digitalWrite(kathodeRPin[spalte], (r ? LOW : HIGH));
    analogWrite(anode, r);
    delay(1);
    digitalWrite(anode, LOW);
    digitalWrite(kathodeGPin[spalte], (g ? LOW : HIGH));
    analogWrite(anode, g);
    delay(1);
    digitalWrite(anode, LOW);
    digitalWrite(kathodeBPin[spalte], (b ? LOW : HIGH));
    analogWrite(anode, b);
    delay(1);
    digitalWrite(anode, LOW);
    digitalWrite(kathodeRPin[spalte], HIGH);
    digitalWrite(kathodeGPin[spalte], HIGH);
    digitalWrite(kathodeBPin[spalte], HIGH);
  }
}
