Brújula para no videntes. 

Permite al no vidente saber hacia dirección está yendo y mantenerse. 

Este ejemplo se realiza con un servo posicional de 180° ya que no es fácil conseguir uno que sea posicional de 360°(la mayoría son de rotación continua). 

Este ejemplo es solo un prototipo, idealmente debe miniaturizarse. 

Materiales utilizados: 
-Servo 180°
-Arduino UNO
-brújula QMC5883L
-Level shifter de 4 canales (conversor de 5v a 3,3V)
-2 pulsadores
-1 led
-resistencias: 2 de 10K y 1 de 330.
-protoboard

Modo de uso:
1 pulsador es para calibrar, al apretarlo debe girar la brújula en todos los sentidos para detectar valores máximos y mínimos y calibrar. Tiene 10 segundos en los cuales el LED quedará prendido. (tal vez habría que cambiar el LED por un buzzer)
La brújula siempre apunta al Norte, a menos que se presione el segundo botón que es para apuntar al sur. Esto es porque el servo de 180 grados no puede girar en todas las posiciones. Habría que cambiar el pulsador por un miniswitch de 2 pines y 2 posiciones. 



