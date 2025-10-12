


#include "Arduino.h"
#include "stepperAS.h"


  stepperMotor::stepperMotor(){};
  stepperMotor::~stepperMotor(){};


  void stepperMotor::Init(unsigned long pasos, int pin1, int pin2, int pin3, int pin4) {

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    _pin4 = pin4;
    _posicion = 0;
    _pasos = pasos;
    _tiempoPaso = 20;
    
  }

  void stepperMotor::Speed(unsigned long rpm) {


    _tiempoPaso = (int)(60000 / (rpm * _pasos));
    if (_tiempoPaso < 1) _tiempoPaso = 1;

   
      Serial.print("tiempo:");
      Serial.println(_tiempoPaso);
    
  }


  void stepperMotor::Steps(unsigned long CantidadDePasos) {

    int pasos;

    pasos = CantidadDePasos;
    if (pasos > 0)
      while (pasos > 0) {
        switch (_posicion) {
          case 0:
            digitalWrite(_pin1, HIGH);
            digitalWrite(_pin2, HIGH);
            _posicion++;
            break;
          case 1:
            digitalWrite(_pin1, LOW);
            digitalWrite(_pin2, HIGH);
            _posicion++;
            break;
          case 2:
            digitalWrite(_pin2, HIGH);
            digitalWrite(_pin3, HIGH);
            _posicion++;
            break;
          case 3:
            digitalWrite(_pin2, LOW);
            digitalWrite(_pin3, HIGH);
            _posicion++;
            break;
          case 4:
            digitalWrite(_pin3, HIGH);
            digitalWrite(_pin4, HIGH);
            _posicion++;
            break;
          case 5:
            digitalWrite(_pin3, LOW);
            digitalWrite(_pin4, HIGH);
            _posicion++;
            break;
          case 6:
            digitalWrite(_pin4, HIGH);
            digitalWrite(_pin1, HIGH);
            _posicion++;
            break;
          case 7:
            digitalWrite(_pin4, LOW);
            digitalWrite(_pin1, HIGH);
            _posicion = 0;
            break;
          default: _posicion = 0;
        }

        pasos--;
        delay(_tiempoPaso);
      }

    else
      while (pasos < 0) {
        switch (_posicion) {
          case 7:
            digitalWrite(_pin1, LOW);
            digitalWrite(_pin4, HIGH);
            _posicion--;
            break;
          case 6:
            digitalWrite(_pin3, HIGH);
            digitalWrite(_pin4, HIGH);
            _posicion--;
            break;
          case 5:
            digitalWrite(_pin3, HIGH);
            digitalWrite(_pin4, LOW);
            _posicion--;
            break;
          case 4:
            digitalWrite(_pin2, HIGH);
            digitalWrite(_pin3, HIGH);
            _posicion--;
            break;
          case 3:
            digitalWrite(_pin2, HIGH);
            digitalWrite(_pin3, LOW);
            _posicion--;
            break;
          case 2:
            digitalWrite(_pin1, HIGH);
            digitalWrite(_pin2, HIGH);
            _posicion--;
            break;
          case 1:
            digitalWrite(_pin1, HIGH);
            digitalWrite(_pin2, LOW);
            _posicion--;
            break;
          case 0:
            digitalWrite(_pin1, HIGH);
            digitalWrite(_pin4, HIGH);
            _posicion = 7;
            break;
          default: _posicion = 7;
        }
        pasos++;
        delay(_tiempoPaso);
      }
  }
  
  void stepperMotor::Off(){
    digitalWrite(_pin1,LOW);
    digitalWrite(_pin2,LOW);
    digitalWrite(_pin3,LOW);
    digitalWrite(_pin4,LOW);
  }


