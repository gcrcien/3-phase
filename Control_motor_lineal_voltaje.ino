#include <math.h>
const int OA1 = PA8;
const int OA2 = PA9;
const int OA3 = PA10;
int analogVal;
int analogVal2;
int H1, H2, H3, vel, I;
const int analogInput = PA0;
const int analogInput2 = PA1;
double A = 1;
double B = 0;
int amp;
int ampval;
int var1 = 0;
int var2 = 0;
int var3 = 0;
int var4 = 0;
int var5 = 0;
int var6 = 0;
int cont = 0;
double Phase3 = 2 * PI;
double Phase1 = 2 * PI / 3;
double Phase2 = 4 * PI / 3;                               //Estas son las fases, o especificamente la relacion que existe entre ellas expresada en numeros
bool dir;
int Hpos = 250;
int Vpos = 250;
bool serialMode;
int frecuencia = 15000;                                    //frecuencia del pwm en hz
int overflown = 72000000 / frecuencia;                     //calculando el numero de overflow para la fecuencia deseada, la resolucion disminuye a mayor frecuencia
//int overflown = 65535 / 10;                              //aqui se modifica la frecuencia de trabajo para el pwm 65535 == 1khz ahorita esta en 10khz aproximadamente
int medio = overflown / 2;                                 //se divide el overflow number entre 2 para mantener las señales a mitad de rango

double C = 0;
int serialFlag = true;
///// declaracion de anchos de pulso min y max, crear funcion para escalar proporcionalmente con la resolucion del sistema
int maxI = 300;                                            //ancho de pulso maximo, esta variable dicta la corriente maxima que se administrara al motor
int minI = 10;                                             //ancho de pulso , esta variable dicta la corriente minima que se administra al m

HardwareTimer pwmtimer1(1);                                //asignando nombres a los timers con los que se va a trabajar 1-4
HardwareTimer pwmtimer2(2);
HardwareTimer pwmtimer3(3);
HardwareTimer pwmtimer4(4);



void setup()
{

  pwmtimer1.pause();
  pwmtimer1.setPrescaleFactor(1);
  pwmtimer1.setOverflow(overflown);
  pwmtimer1.refresh();
  pwmtimer1.resume();
  ///
  pwmtimer2.pause();
  pwmtimer2.setPrescaleFactor(1);
  pwmtimer2.setOverflow(overflown);
  pwmtimer2.refresh();
  pwmtimer2.resume();
  ///                                                         Aqui se estan modificando los timers para elevar la frecuencia de trabajo del pwm
  pwmtimer3.pause();
  pwmtimer3.setPrescaleFactor(1);
  pwmtimer3.setOverflow(overflown);
  pwmtimer3.refresh();
  pwmtimer3.resume();
  ///
  pwmtimer4.pause();
  pwmtimer4.setPrescaleFactor(1);
  pwmtimer4.setOverflow(overflown);
  pwmtimer4.refresh();
  pwmtimer4.resume();
  //


  //timer.setPrescaleFactor(1);
  Serial.begin(9600);                                       //
  Serial.setTimeout(1);                                     // timeout para la espera de datos por serial, en mS
  pinMode(OA1, PWM);                                        //poniendo los pines en modo PWM
  pinMode(OA2, PWM);
  pinMode(OA3, PWM);
  delay(500);
}
void actualizar_pos() {
  if (vel > 4096 / 2) {
    dir = 1;
  }
  if (vel < 4096 / 2) {
    dir = 0;
  }
  
  C = map(vel, 0, 4096, 0, 1000 );                         //Son los mmismos algoritmos de control que para las palancas, simplemente tomando los comandos por serial
  if (dir) {                                        //Algoritmo de control para el motor
    ampval = ampval * (-1);                                 //checando la direccion del movimiento
  }                                                         //
  C = C / 1000000;                                          //
  amp = map(vel, 0, 2048, minI, maxI );                   //
  A += C;





  //MOTOR #1
  var4 = amp * sin(A + Phase1);                              //Aqui se calculan los valores de cada una de las fases de cada motor
  var1 = var4 + medio;                                       //se suma 32768/2 para que el valor del pwm se mantenga positivo osea se pone media potencia
  var5 = amp * sin(A + Phase2);                              //y varia hacia arriba y hacia abajo con los parametros dados para el ancho de pulso
  var2 = var5 + medio;                                       //
  var6 = amp * sin(A + Phase3);
  var3 = var6 + medio;
  //32768 para maxima resolucion minima frecuencia

  pwmWrite(OA1, var1);                                        //aqui se mandan los valores de cada fase en forma de pwm, es decir son las conexiones de
  pwmWrite(OA2, var2);                                        //cada fase del motor se conectan aqui
  pwmWrite(OA3, var3);
  Serial.print(var1);
  Serial.print(" ");
  Serial.print(var2);
  Serial.print(" ");
  Serial.println(var3);

}
void getcommands() {
  vel = analogRead(PA4);                                      //lecturas con pines provisionales/placeholders
  //dir = digitalRead(PA5);
  H1 = analogRead(PA1);
  H2 = analogRead(PA2);
  H3 = analogRead(PA3);
  I = analogRead(PA6);
}
void loop()
{
  getcommands();                                          //Se piden leer los comandos analogicos
  actualizar_pos();                                     //se agrega este nivel de disparo para evitar que ruido genere movimientos no deseados


}
