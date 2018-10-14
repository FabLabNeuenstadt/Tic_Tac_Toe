#ifndef Spiel_h
#define Spiel_h

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
