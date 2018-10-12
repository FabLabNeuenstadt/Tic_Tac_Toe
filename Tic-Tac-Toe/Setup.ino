// Alle Pins, die mit den LEDs verbunden sind, auf Ausgang setzen
void SetupEA( void )
{
  for (int pin = 2; pin < 14; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  pinMode(A0, OUTPUT);
}
