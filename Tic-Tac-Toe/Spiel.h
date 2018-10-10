#ifndef Spiel_h
#define Spiel_h

enum Spieler {
  None = 0b00000000,
  A = 0b00000001,
  B = 0b00000010
};

struct Koordinaten {
  byte X;
  byte Y;

  static bool Equals(Koordinaten *left, Koordinaten *right)
  {
    return left->X == right->X && left->Y == right->Y;
  }
  bool operator ==(Koordinaten right)
  {
      return Equals(this, &right);
  }
  bool operator !=(Koordinaten right)
  {
      return !Equals(this, &right);
  }
};
Koordinaten KeineKoordinaten = {255,255};

#endif
