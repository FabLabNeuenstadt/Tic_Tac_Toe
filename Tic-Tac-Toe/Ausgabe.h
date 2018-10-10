#ifndef Ausgabe_h
#define Ausgabe_h

struct RGB {
  byte R;
  byte G;
  byte B;
};

const byte anodePin[3] = {10, 9, 6};
const byte kathodeRPin[3] = {12, 8, 2};
const byte kathodeGPin[3] = {A0, 7, 3};
const byte kathodeBPin[3] = {13, 5, 4};

RGB Bild[3][3];

#endif
