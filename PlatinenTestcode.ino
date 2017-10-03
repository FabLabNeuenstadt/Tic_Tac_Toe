char spalte[] = {11,10,9,6};
char RGB[] = {2,3,4,8,7,5,12,14,13};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(int s = 0; s < 4; s++){
    pinMode(spalte[s], OUTPUT);
    digitalWrite(spalte[s], HIGH);
  }
  for(int r = 0; r < 9; r++) {
    digitalWrite(RGB[r], HIGH);
    pinMode(RGB[r], OUTPUT);
  }
}

void loop() {

  for(int s = 0; s < 4; s++){
   digitalWrite(spalte[s], (s==0) ? HIGH : LOW);
   for(int r = 0; r < 9; r++) {
      digitalWrite(RGB[r], LOW);
      delay(500);
      digitalWrite(RGB[r], HIGH);
    
        Serial.println(analogRead(A7));

    }
    digitalWrite(spalte[s], (s==0) ? LOW : HIGH);
  }
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A7));
}
