// ermittelt die aktuell gedrückte Taste
Koordinaten TasteGedrueckt()
{
  int ADC_Wert = analogRead(A7);

  if (IstTasteGedrueckt(ADC_Wert, 850)) {
    return {0,2};
  } else if (IstTasteGedrueckt(ADC_Wert, 730)) {
    return {0,1};
  } else if (IstTasteGedrueckt(ADC_Wert, 640)) {
    return {0,0};
  } else if (IstTasteGedrueckt(ADC_Wert, 565)) {
    return {1,0};
  } else if (IstTasteGedrueckt(ADC_Wert, 510)) {
    return {1,1};
  } else if (IstTasteGedrueckt(ADC_Wert, 465)) {
    return {1,2};
  } else if (IstTasteGedrueckt(ADC_Wert, 420)) {
    return {2,2};
  } else if (IstTasteGedrueckt(ADC_Wert, 395)) {
    return {2,1};
  } else if (IstTasteGedrueckt(ADC_Wert, 365)) {
    return {2,0};
  }
  
  return {255,255};
}

bool IstTasteGedrueckt(int wert, int ziel)
{
  return wert > (ziel-10) && wert < (ziel+10);
}

