


#ifndef STTEPER_AS
#define STTEPER_AS


#include "Arduino.h"

#define STEPS 2048



class stepperMotor {



public:
  stepperMotor();
  ~stepperMotor();


  void Init(unsigned long pasos, int pin1, int pin2, int pin3, int pin4);

  void Speed(unsigned long rpm);


  void Steps(unsigned long CantidadDePasos);

  void Off();
  
  
  private:
  int _posicion;
  unsigned long _pasos;
  int _tiempoPaso;
  int _pin1, _pin2, _pin3, _pin4;

};


#endif

