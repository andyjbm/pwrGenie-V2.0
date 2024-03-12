#ifndef GEEKUTILS_H
  #define GEEKUTILS_H

  #include <Arduino.h>
  extern float my_vfact;
  float getBattery()
  {
    analogRead(A0); // drop first read
    return analogRead(A0) / my_vfact;
  }
#endif