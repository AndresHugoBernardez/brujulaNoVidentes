/*
===============================================================================================================
QMC5883LCompass.h Library XYZ Example Sketch
Learn more at [https://github.com/mprograms/QMC5883LCompass]

*/
#include <QMC5883LCompass.h>
#include <Servo.h>

Servo servo;

QMC5883LCompass compass;

void setup() {
  servo.attach(9);
  Serial.begin(9600);
  compass.init();
  compass.setMode(0x01,0x08,0x00,0x00);
  compass.setMagneticDeclination(3,6);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
  
}
  int grados;
  int searchSouth;
  int botonCalibracion=0;
  int Apretado=0;
  int calibrando=0;

void loop() {
 


//Calibración (dura 10 segundos)
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
          compass.calibrate();
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

// Localizar servo
   searchSouth=digitalRead(3);  
  if(searchSouth==0){
    if(0<grados&&grados<=180)servo.write(grados);
    if(180<grados&&grados<=270)servo.write(180);
    if(270<grados&&grados<=360)servo.write(0);
  }
  if(searchSouth==1){
    if(180<grados&&grados<=360)servo.write(grados-180);
    if(90<grados&&grados<=180)servo.write(0);
    if(0<grados&&grados<=90)servo.write(180);
  }

  //Escribir datos:
  Serial.print("Grados: ");
  Serial.println(grados);

  delay(444);
}