

/*
===============================================================================================================
QMC5883LCompass.h Library 
Learn more at [https://github.com/mprograms/QMC5883LCompass]

*/
#include <QMC5883LCompass.h>


#define STEPS 2048



  class stepperMotor{

    private:
    int _posicion;
    unsigned long _pasos;
    unsigned long _tiempoPaso;
    int _pin1,_pin2,_pin3,_pin4;


    public:
     stepperMotor(unsigned long pasos,int pin1,int pin2,int pin3, int pin4){

        pinMode(pin1,OUTPUT);
        pinMode(pin2,OUTPUT);
        pinMode(pin3,OUTPUT);
        pinMode(pin4,OUTPUT);
        _pin1=pin1;
        _pin2=pin2;
        _pin3=pin3;
        _pin4=pin4;
        _posicion=0; 
        _pasos=pasos;
        _tiempoPaso=20;

    }

    void Speed(unsigned long rpm){
 

      _tiempoPaso=60000/(rpm*_pasos);
      if(_tiempoPaso<1)_tiempoPaso=1;

      Serial.print("tiempo:");
      Serial.println(_tiempoPaso);
    }


      int Steps(int CantidadDePasos){

        int pasos;

        pasos=CantidadDePasos;
        if(pasos>0)
        while(pasos>0){
          switch(_posicion){
            case 0: digitalWrite(_pin1,HIGH);
                    digitalWrite(_pin2,HIGH);
                    _posicion++;
                    break;
            case 1: digitalWrite(_pin1,LOW);
                    digitalWrite(_pin2,HIGH);
                    _posicion++;
                    break;
            case 2: digitalWrite(_pin2,HIGH);
                    digitalWrite(_pin3,HIGH);
                    _posicion++;
                    break;
            case 3: digitalWrite(_pin2,LOW);
                    digitalWrite(_pin3,HIGH);
                    _posicion++;
                    break;
            case 4: digitalWrite(_pin3,HIGH);
                    digitalWrite(_pin4,HIGH);
                    _posicion++;
                    break;
            case 5: digitalWrite(_pin3,LOW);
                    digitalWrite(_pin4,HIGH);
                    _posicion++;
                    break;
            case 6: digitalWrite(_pin4,HIGH);
                    digitalWrite(_pin1,HIGH);
                    _posicion++;
                    break;
            case 7: digitalWrite(_pin4,LOW);
                    digitalWrite(_pin1,HIGH);
                    _posicion=0;
                    break;
            default: _posicion=0;
                    
          }

          pasos--;
          delay(_tiempoPaso);
          
        }

        else while(pasos<0){
          switch(_posicion){
            case 7: digitalWrite(_pin1,LOW);
                    digitalWrite(_pin4,HIGH);
                    _posicion--;
                    break;
            case 6: digitalWrite(_pin3,HIGH);
                    digitalWrite(_pin4,HIGH);
                    _posicion--;
                    break;
            case 5: digitalWrite(_pin3,HIGH);
                    digitalWrite(_pin4,LOW);
                    _posicion--;
                    break;
            case 4: digitalWrite(_pin2,HIGH);
                    digitalWrite(_pin3,HIGH);
                    _posicion--;
                    break;
            case 3: digitalWrite(_pin2,HIGH);
                    digitalWrite(_pin3,LOW);
                    _posicion--;
                    break;
            case 2: digitalWrite(_pin1,HIGH);
                    digitalWrite(_pin2,HIGH);
                    _posicion--;
                    break;
            case 1: digitalWrite(_pin1,HIGH);
                    digitalWrite(_pin2,LOW);
                    _posicion--;
                    break;
            case 0: digitalWrite(_pin1,HIGH);
                    digitalWrite(_pin4,HIGH);
                    _posicion=7;
                    break;
            default: _posicion=7;
                    
          }
          pasos++;
          delay(_tiempoPaso);

          
        }
    

      }
  
  };
  
  
stepperMotor motor(4096,8,9,10,11);

QMC5883LCompass compass;

void setup() {
  
  Serial.begin(9600);
  compass.init();
  compass.setMode(0x01,0x08,0x00,0x00);
  compass.setMagneticDeclination(3,6);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);

  motor.Speed(10);
  
}
  int grados;
  int referencia;
  int botonCalibracion=0;
  int Apretado=0;
  int calibrando=0;
  long contador=0;
  long pasos=0;
  long gradosLong;
  int diferencia=0;
  int i;
  int posicion=0;
  int EastApretado=0;






void loop() {
 


//Calibración 
    botonCalibracion=digitalRead(2);
   if((botonCalibracion==1)&&(Apretado==0)){
    delay(333);
    botonCalibracion=digitalRead(2);
    if(botonCalibracion==1){
      Apretado=1;
      
        if(calibrando==0){
          calibrando=1;
          digitalWrite(4,HIGH);
          Serial.print("Calibrando...");
          //dura 10 segundos
          compass.calibrate();
          referencia=0;
          while(referencia==0){
            motor.Steps(1);
            referencia=digitalRead(3);

          }
          contador=0;
          

          digitalWrite(4,LOW);
          Serial.println("...READY!");
      
        }
    }

   }
   if((botonCalibracion==0)&&(Apretado==1)){
    delay(333);
    botonCalibracion=digitalRead(2);
    if(botonCalibracion==0){
      Apretado=0;
      calibrando=0;
    }

   }
   


// Read compass values
  compass.read();
  grados=compass.getAzimuth();

//Corrección
if(grados<0){grados=grados+360;}
gradosLong=(long)grados;

  //Escribir datos:
  Serial.print("Grados: ");
  Serial.println(grados);



// Localizar motor
    
  pasos=map(gradosLong,0,359,0,4095);
  Serial.print(" pasos: ");
  Serial.println(pasos);

  if(contador-pasos>2048)pasos+=4095;
  if(contador-pasos<-2048)pasos-=4095;
 
 Serial.print("contador: ");
  Serial.print(contador);
   Serial.print(" pasos: ");
  Serial.println(pasos);

referencia=0;
if(pasos>contador){
    diferencia=(int)(pasos-contador);
      Serial.print("diferencia:");
      Serial.println(diferencia);
    for(i=0;i<diferencia&&i<500;i++){
      


        referencia=digitalRead(3);
        if(EastApretado==0&&referencia==1) {
         
          delay(88);
          referencia=digitalRead(3);
          if(referencia==1){
            Serial.println("posición Este encontrada...");
            contador=0;
            i=diferencia;
            EastApretado=1;
          }  
        }
        else if(EastApretado==1&&referencia==0)
        {
          
          delay(88);
          referencia=digitalRead(3);
          if(referencia==0){
            EastApretado=0;
          }
        }

      motor.Steps(1);
      contador++;
      if(contador==4096)contador=0;

   
    }
  }
  else if(pasos<contador){
       diferencia=(int)(contador-pasos);
       Serial.print("diferencia:-");
      Serial.println(diferencia);
      
    for(i=0;i<diferencia&&i<500;i++){
      

      
        referencia=digitalRead(3);
        if(EastApretado==0&&referencia==1) {

         
          delay(88);
          referencia=digitalRead(3);
          if(referencia==1){
            Serial.println("posición Este encontrada...");
            contador=0;
            i=diferencia;
            EastApretado=1;
          }  
        }
        else if(EastApretado==1&&referencia==0)
        {
          
          delay(88);
          referencia=digitalRead(3);
          if(referencia==0){
            EastApretado=0;
          }
        }
        
      
      
      motor.Steps(-1);
      contador--;
      
      if(contador==-1)contador=4095;
    
    }
  }



  delay(333);
}