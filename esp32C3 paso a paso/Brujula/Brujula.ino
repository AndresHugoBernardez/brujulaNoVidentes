

/*
===============================================================================================================
QMC5883LCompass.h Library Learn more at [https://github.com/mprograms/QMC5883LCompass]



*/
#include <QMC5883LCompass.h>
#include <Preferences.h>
#include "stepperAS.h"

Preferences espacio;
int xmin,xmax,ymin,ymax,zmin,zmax,decl;



int declinacion = 2;
int inCalibrar = 6;
int outCalibrar = 8;
int inSurPos = 10;



int gradosDecl;
uint8_t minutosDecl;



int grados;
int grados1;
int referencia;
int botonCalibracion = 0;
int Apretado = 0;
int calibrando = 0;
long contador = 0;
long pasos = 0;
long gradosLong;
int diferencia = 0;
int i;
int posicion = 0;
int EastApretado = 0;
char myArray[3];




void andyCalibrate(QMC5883LCompass *cmpss) {
  int actualizar=0;
  int declin=0;
  long lectura;
  int k;
  long xch,ych,zch;
  int parpadeo=0;
 



 if(Serial)Serial.println("Ajuste la declinacion.");

  for(k=0;k<50;k++){

    lectura=(long)analogRead(declinacion);
    declin=(int)map(lectura,3100,4095,-300,300) ;
    if(Serial){
      Serial.print("Declinacion: ");
      Serial.print(declin);
      Serial.print("tiempo restante: ");
      Serial.println(25.0-(((float)k)*0.5));
      
      }

    if(parpadeo==0){
      parpadeo=1;  
      digitalWrite(outCalibrar,HIGH);
    }else
    {
      parpadeo=0;
      digitalWrite(outCalibrar,LOW);
    }
    delay(500);
    }

   gradosDecl =(declin/10);
   minutosDecl =((uint8_t ) (  ((declin%10)*60)/10   ) );

   Serial.print("DeclinacionFINAL:");

   Serial.print(gradosDecl);
   Serial.print("º");
   Serial.print(minutosDecl);
   Serial.println("'");
  
  cmpss->setMagneticDeclination(gradosDecl,minutosDecl);

  digitalWrite(outCalibrar,HIGH);
 if(Serial)Serial.println("Mueva la brujula");

	cmpss->clearCalibration();
	long calibrationData[3][2] = {{65000, -65000}, {65000, -65000}, {65000, -65000}};

    cmpss->read();

  	long	x = calibrationData[0][0] = calibrationData[0][1] = cmpss->getX();
  	long	y = calibrationData[1][0] = calibrationData[1][1] = cmpss->getY();
  	long	z = calibrationData[2][0] = calibrationData[2][1] = cmpss->getZ();
    xch=x;
    ych=y;
    zch=z;



	unsigned long startTime = millis();

	while((millis() - startTime) < 30000) {
		cmpss->read();

  		x = cmpss->getX();
  		y = cmpss->getY();
  		z = cmpss->getZ();
      

		if(x < calibrationData[0][0]) {
			calibrationData[0][0] = x;
      actualizar=1;
      
		}
		if(x > calibrationData[0][1]) {
			calibrationData[0][1] = x;
      actualizar=1;
		}

		if(y < calibrationData[1][0]) {
			calibrationData[1][0] = y;
      actualizar=1;
		}
		if(y > calibrationData[1][1]) {
			calibrationData[1][1] = y;
      actualizar=1;
		}

		if(z < calibrationData[2][0]) {
			calibrationData[2][0] = z;
      actualizar=1;
		}
		if(z > calibrationData[2][1]) {
			calibrationData[2][1] = z;
      actualizar=1;
		}

     if(actualizar){
    actualizar=0;
    if(Serial){
     Serial.print(x);
     Serial.print(":");
     Serial.print(y);
     Serial.print(":");
     Serial.print(z);
     Serial.println(":");
     }
  }
	}

 
  
  if(Serial){
    Serial.println("Resultados: ");
    Serial.print(" xmin:");
    Serial.print(calibrationData[0][0]);
    Serial.print(" xmax:");
    Serial.print(calibrationData[0][1]);
    Serial.print(" ymin:");
    Serial.print(calibrationData[1][0]);
    Serial.print(" ymax:");
    Serial.print(calibrationData[1][1]);
    Serial.print(" zmin:");
    Serial.print(calibrationData[2][0]);
    Serial.print(" zmax:");
    Serial.println(calibrationData[2][1]);
  }


  xmin=calibrationData[0][0];
  xmax=calibrationData[0][1];
  ymin=calibrationData[1][0];
  ymax=calibrationData[1][1];
  zmin=calibrationData[2][0];
  zmax=calibrationData[2][1];


  if(xch!=calibrationData[0][1]||ych!=calibrationData[1][1]||zch!=calibrationData[2][1]){
    espacio.begin("variables",false);

    espacio.putInt("xmin",xmin);
    espacio.putInt("xmax",xmax);
    espacio.putInt("ymin",ymin);
    espacio.putInt("ymax",ymax);
    espacio.putInt("zmin",zmin);
    espacio.putInt("zmax",zmax);
    decl=declin;
    espacio.putInt("decl",decl);
    espacio.end();
    }

	cmpss->setCalibration(
		calibrationData[0][0],
		calibrationData[0][1],
		calibrationData[1][0],
		calibrationData[1][1],
		calibrationData[2][0],
		calibrationData[2][1]
	);
  digitalWrite(outCalibrar,LOW);
}







stepperMotor motor;

QMC5883LCompass compass;

void setup() {

  unsigned long tiempo,tiempo2;


  

  motor.Init(4096, 4, 3, 1, 0);

 int k;
  k=0;
  Serial.begin(9600);
  tiempo=millis();
  tiempo=tiempo+100;
  while(!Serial && k<100){
    
    
    tiempo2=millis();
    
    if(tiempo2>tiempo){
      k++;
      tiempo=tiempo2+100;
      }
    };
  Serial.println("Andy Softwares...");

  
  Wire.begin(5,7);
  compass.setReset();
  //compass.setMode(0x01,0x0C,0x10,0X00);
   
  compass.setMode(0x01, 0x04, 0x10, 0x00);

  espacio.begin("variables", true);

  xmin=espacio.getInt("xmin",0);
  xmax=espacio.getInt("xmax",0);
  ymin=espacio.getInt("ymin",0);
  ymax=espacio.getInt("ymax",0);
  zmin=espacio.getInt("zmin",0);
  zmax=espacio.getInt("zmax",0);
  decl=espacio.getInt("decl",0);

  espacio.end();
  
  gradosDecl =(decl/10);
   minutosDecl =((uint8_t ) ((decl%10)*60)/10 );
  Serial.print(gradosDecl);
   Serial.print("º");
   Serial.print(minutosDecl);
   Serial.println("'");
   //delay(500);
  compass.setMagneticDeclination(gradosDecl,minutosDecl);

 if(Serial){
    Serial.println("Leido: ");
    Serial.print(" xmin:");
    Serial.print(xmin);
    Serial.print(" xmax:");
    Serial.print(xmax);
    Serial.print(" ymin:");
    Serial.print(ymin);
    Serial.print(" ymax:");
    Serial.print(ymax);
    Serial.print(" zmin:");
    Serial.print(zmin);
    Serial.print(" zmax:");
    Serial.print(zmax);
    Serial.print(" decl:");
    Serial.println(decl);
  }

  compass.setCalibration(xmin,xmax,ymin,ymax,zmin,zmax);
  //compass.setMagneticDeclination(3, 6);


  pinMode(inCalibrar, INPUT);
  pinMode(inSurPos, INPUT);
  pinMode(outCalibrar, OUTPUT);
  
  pinMode(declinacion,INPUT);
 
  digitalWrite(outCalibrar, LOW);

  motor.Speed(4);
  

   referencia=1;
        while (referencia == 1) {
          motor.Steps(1);
          referencia = digitalRead(inSurPos);
        }

          motor.Off();
}







void loop() {

 Serial.println("loop:");
 


       

  //Calibración
  botonCalibracion = digitalRead(inCalibrar);
  if ((botonCalibracion == 0) && (Apretado == 0)) {
    delay(333);
    botonCalibracion = digitalRead(inCalibrar);
    if (botonCalibracion == 0) {
      Apretado = 1;

      if (calibrando == 0) {
        calibrando = 1;
        digitalWrite(outCalibrar, HIGH);
        Serial.print("Calibrando...");
        //dura 10 segundos
        andyCalibrate(&compass);
        referencia = 1;
        while (referencia == 1) {
          motor.Steps(1);
          referencia = digitalRead(inSurPos);
        }
        contador = 0;
        motor.Off();


        digitalWrite(outCalibrar, LOW);
        Serial.println("...READY!");
      }
    }
  }
  if ((botonCalibracion == 1) && (Apretado == 1)) {
    delay(333);
    botonCalibracion = digitalRead(inCalibrar);
    if (botonCalibracion == 1) {
      Apretado = 0;
      calibrando = 0;
    }
  }



  // Read compass values
  compass.read();
  grados1 = compass.getAzimuth();

  //Corrección
  if (grados1 < 0) { grados = grados1 + 360; }
  else grados=grados1;
  gradosLong = (long)grados;

  //Escribir datos:
  Serial.print("Grados: ");
  Serial.println(grados);
  
  compass.getDirection(myArray, grados1);

  Serial.print("Direccion: ");
  Serial.print(myArray[0]); // 
  Serial.print(myArray[1]); // 
  Serial.print(myArray[2]); // 
  Serial.print("::(");
  Serial.print(compass.getX());
  Serial.print(",");
  Serial.print(compass.getY());
  Serial.print(",");
  Serial.print(compass.getZ());
  Serial.println(")");
  



  // Localizar motor

  pasos = map(gradosLong, 0, 359, 0, 4095);
  Serial.print(" pasos: ");
  Serial.println(pasos);

  if (contador - pasos > 2048) pasos += 4095;
  if (contador - pasos < -2048) pasos -= 4095;

  Serial.print("contador: ");
  Serial.println(contador);
  Serial.print(" pasos: ");
  Serial.println(pasos);

  referencia = 0;
  if (pasos > contador) {
    diferencia = (int)(pasos - contador);
    Serial.print("diferencia:");
    Serial.println(diferencia);
    if(diferencia<48)diferencia=0;

    for (i = 0; i < diferencia && i < 222; i++) {



      referencia = digitalRead(inSurPos);
      if (EastApretado == 0 && referencia == 0) {

        delay(88);
        referencia = digitalRead(inSurPos);
        if (referencia == 0) {
          Serial.println("posición Sur encontrada...");
          contador = 0;
          i = diferencia;
          EastApretado = 1;
        }
      } else if (EastApretado == 1 && referencia == 1) {

        delay(88);
        referencia = digitalRead(inSurPos);
        if (referencia == 1) {
          EastApretado = 0;
        }
      }

      motor.Steps(1);
      contador++;
      if (contador == 4096) contador = 0;
    }
  } else if (pasos < contador) {
    diferencia = (int)(contador - pasos);
    Serial.print("diferencia:-");
    Serial.println(diferencia);
    if(diferencia<48)diferencia=0;

    for (i = 0; i < diferencia && i < 222; i++) {



      referencia = digitalRead(inSurPos);
      if (EastApretado == 0 && referencia == 0) {


        delay(88);
        referencia = digitalRead(inSurPos);
        if (referencia == 0) {
          Serial.println("posición Sur encontrada...");
          contador = 0;
          i = diferencia;
          EastApretado = 1;
        }
      } else if (EastApretado == 1 && referencia == 1) {

        delay(88);
        referencia = digitalRead(inSurPos);
        if (referencia == 1) {
          EastApretado = 0;
        }
      }



      motor.Steps(-1);
      contador--;

      if (contador == -1) contador = 4095;
    }
  }

  motor.Off();



  delay(222);
}
