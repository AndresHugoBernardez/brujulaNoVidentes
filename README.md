Brújula para no videntes. 
===========================
Permite al no vidente saber hacia dirección está yendo y mantenerse. 

Este ejemplo se realiza con un servo posicional de 180° ya que no es fácil conseguir uno que sea posicional de 360°(la mayoría son de rotación continua). 

* Este ejemplo es solo un prototipo, idealmente debe miniaturizarse. 

Materiales utilizados: 
----------------------
-Servo 180°
-Arduino UNO
-brújula QMC5883L
-Level shifter de 4 canales (conversor de 5v a 3,3V)
-2 pulsadores
-1 led
-resistencias: 2 de 10K y 1 de 330.
-protoboard
-Cables
-Batería 9V

Modo de uso:
------------
Un pulsador es para calibrar, al apretarlo debe girar la brújula en todos los sentidos para detectar valores máximos y mínimos y calibrar. Tiene 10 segundos en los cuales el LED quedará prendido. (tal vez habría que cambiar el LED por un buzzer)
La brújula siempre apunta al Norte, a menos que se presione el segundo botón que es para apuntar al sur. Esto es porque el servo de 180 grados no puede girar en todas las posiciones. Habría que cambiar el pulsador por un miniswitch de 2 pines y 2 posiciones. 

VIDEO: https://www.youtube.com/watch?v=Hrc1E1AIdSQ


DOCUMENTACIÓN: 
--------------
https://github.com/e-Gizmo/QMC5883L-GY-271-Compass-module/blob/master/QMC5883L%20Datasheet%201.0%20.pdf

* Declinación Magnética: Cada lugar del mundo tiene su propia declinación magnética
https://www.isndf.com.ar/curso/declinacion-magnetica/
https://collections.lib.uwm.edu/digital/collection/agdm/id/11610/

* Librería utilizada: Para que funcione tiene que utilizarse la siguiente librería. Se puede descargar desde Arduino IDE en "Manage Libraries..."
https://github.com/mprograms/QMC5883LCompass (QMC5883L Compass Arduino Library by MPrograms)


