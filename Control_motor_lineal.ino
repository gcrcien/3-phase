#include <math.h>
const int OB1 = PA8;
const int OB2 = PA9;
const int OB3 = PA10;
const int OA1 = PB7;
const int OA2 = PB8;
const int OA3 = PB9;
int analogVal;
int analogVal2;
int H1, H2, H3;
const int analogInput = PA0;
const int analogInput2 = PA1;
//HardwareTimer timer(1);
double A = 1;
double B = 0;
double A2 = 1;
double B2 = 0;
int amp;
int amp2;
int ampval;
int ampval2;
int var1 = 0;
int var2 = 0;
int var3 = 0;
int var4 = 0;
int var5 = 0;
int var6 = 0;
int bar1 = 0;
int bar2 = 0;
int bar3 = 0;
int bar4 = 0;
int bar5 = 0;
int bar6 = 0;
int cont = 0;
double Phase3 = 2 * PI;
double Phase1 = 2 * PI / 3;
double Phase2 = 4 * PI / 3;                               // Estas son las fases, o especificamente la relacion que existe entre ellas expresada en numeros
boolean toggle = true;                                    // true = activa la escritura hacia el Serial Plotter
bool serialToggle = true;
bool analogToggle = false;
int Hpos = 250;
int Vpos = 250;
bool serialMode;
int frecuencia = 15000;                                    // frecuencia del pwm en hz
int overflown = 72000000 / frecuencia;                     //calculando el numero de overflow para la fecuencia deseada, la resolucion disminuye a mayor frecuencia
//int overflown = 65535 / 10;                              //aqui se modifica la frecuencia de trabajo para el pwm 65535 == 1khz ahorita esta en 10khz aproximadamente
int medio = overflown / 2;                                 //se divide el overflow number entre 2 para mantener las señales a mitad de rango
int ctV;
int ctH;
double C = 0;
double C2 = 0;
int serialFlag = true;
///// declaracion de anchos de pulso min y max, crear funcion para escalar proporcionalmente con la resolucion del sistema
int maxI = 1000;                                            //ancho de pulso maximo, esta variable dicta la corriente maxima que se administrara al motor
int minI = 50;                                             //ancho de pulso , esta variable dicta la corriente minima que se administra al m



HardwareTimer pwmtimer1(1);                               // asignando nombres a los timers con los que se va a trabajar 1-4
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
  pinMode(OB1, PWM);
  pinMode(OB2, PWM);
  pinMode(OB3, PWM);
  delay(500);
}


void loop()
{

  if (Serial.available() >= 3) {                          // si el serial buffer tiene mas de 3 bytes significa que un mensaje llego por USB
    Serial.read();                                        // si un mensaje llego por USB lee el buffer
  }
  else {
    //Vpos = 250;                                           //el sistema debe estar recibiendo constantemente comandos de posicion para que el motor no se valla a los limites
    //Hpos = 250;                                           //
  }

  if (Serial.peek() == 'V') {                             // aqui se parsea la informacion, se checa el la letra inicial de cada mensaje, por ejemplo si
    Vpos = Serial.parseInt();                             // el primer caracter de el mensaje es V significa que es un comando para el eje vertical
    actualizar_pos();
  }                                                       // se hace exactamente lo mismo para el eje horizontal
  if (Serial.peek() == 'H') {
    Hpos = Serial.parseInt();
    actualizar_pos();

  }
  // getposition();

  Serial.print(var1);
  Serial.print(" ");
  Serial.print(var2);
  Serial.print(" ");
  Serial.println(var3);
  actualizar_pos();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// meter lo siguiente a funcion para que el integrado no trabaje constantemente recalculando lo mismo

void actualizar_pos() {

  C2 = map(Hpos, 0, 500, 10000, -10000 );                   //
  ampval2 = Hpos - 250;                                     //      Algoritmos de control para el eje Horizontal
  if (ampval2 < 0) {                                        //
    ampval2 = ampval2 * (-1);                               //
  }                                                         //
  C2 = C2 / 1000000;                                         //
  amp2 = (map(ampval2, 0, 250, minI, maxI ));// + 2000;         //

  A2 += C2;






  //MOTOR #1
  var4 = amp * sin(A + Phase1);                              //Aqui se calculan los valores de cada una de las fases de cada motor
  var1 = var4 + medio;                                       //se suma 32768/2 para que el valor del pwm se mantenga positivo osea se pone media potencia
  var5 = amp * sin(A + Phase2);                              //y varia hacia arriba y hacia abajo con los parametros dados para el ancho de pulso
  var2 = var5 + medio;                                       //
  var6 = amp * sin(A + Phase3);
  var3 = var6 + medio;
  //MOTOR #2
  bar4 = amp2 * sin(A2 + Phase1);                            //si se quieren observar las fases mejor en el serial ploter agregar un ofset mayor que 65,535
  bar1 = bar4 + medio;                                       //yo use 80,000 y me funciono bien, sin embargo hay que tener cuidado de regresar el ofset a el valor
  bar5 = amp2 * sin(A2 + Phase2);                            //dentro del rango del pwm para evitar un overflow
  bar2 = bar5 + medio;
  bar6 = amp2 * sin(A2 + Phase3);
  bar3 = bar6 + medio;                                       //32768 para maxima resolucion minima frecuencia

  pwmWrite(OA1, var1);                                        //aqui se mandan los valores de cada fase en forma de pwm, es decir son las conexiones de
  pwmWrite(OA2, var2);                                        //cada fase del motor se conectan aqui
  pwmWrite(OA3, var3);
  pwmWrite(OB1, bar1);
  pwmWrite(OB2, bar2);
  pwmWrite(OB3, bar3);
  Serial.print(var1);
  Serial.print(" ");
  Serial.print(var2);
  Serial.print(" ");
  Serial.println(var3);


  /*if (A >= 2 * PI)                                            //aqui se mantenen los valores de la posicion de cada motor en el rango adecuado 2*PI para ambos motores
    {                                                           //revisar bien puede que esta seccion sea innecesaria
    A = A - (2 * PI);
    }

    if (A2 >= 2 * PI)
    {
    A2 = A2 - (2 * PI);
    }
  */
}
void getposition() {
  analogVal = analogRead(analogInput);
  H1 = analogRead(PA1);
  H2 = analogRead(PA1);
  H3 = analogRead(PA1);

}
