/*
 * PROYECTO INSTRUMENTAL 2 - MÓDULO DE ELECTRÓNICA DIGITAL
 * PRACTICA 1
 * Nombre: LABED
 * Proposito: realizar el temporizador de un horno microondas usando un AVR como sistema embebido.
 * @autor Maria Paula Rey B./Estefania Zuluaga P.
 * @version 1.8.12
 */

#define ledPin 8
#include <LiquidCrystal.h>
#include <Keypad.h>


//Declarar de pines de pantalla LCD
const int rs = 53, en = 49, d4 = 47, d5 = 45, d6 = 43, d7 = 41, pin1 = 21, pin2 = 20, pin3 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Matriz de 4x3 del keypad
const byte ROWS = 4; // Cuatro filas
const byte COLS = 3; // Cuatro columnas
  
// Definir el Keymap
char keys[ROWS][COLS] = 
  {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
  };

//Declarar de pines del keypad 
byte rowPins[ROWS] = { 21, 20, 19, 18 };
byte colPins[COLS] = { 17, 16, 15}; 

//Crear el Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins,
  colPins, ROWS, COLS );

int timerflag=0;                            //se enciende cada que pasa un segundo
int bflag=0;                                //Determina el estado del proceso (Pausa/Inicio)
int timeroven=00000;                        //Variable que guarda el tiempo
int minutes;                                //Miles de unidades y centecimas de unidades de timeroven
int seconds;                                //Unidades y decenas de unidades de timeroven

//Pines de los motores
const int pwm = 12 ;                        //inicializando el pin 12 como pwm
const int in_1 = 6;
const int in_2 = 5;

char timeove[4] = {'0','0','0','0'};        //Vector que recibe del keypad
char key;                                   //Botón presionado

volatile byte state = LOW;                  //Variable global para cambiar el estado del pin
void blink() {                              //Funcion para cambiar de estado el LED
  state = !state;
}

//Recibe la información del Keypad y la convierte a número entero
int some(char key)
{
  if (key == '*'){             // * para aceptar
    return 1;                  // Empezar el proceso 
  }
  if (key == '#'){             // # para cancelar
    return 2;                  // Detener el proceso
  } 

  //Siempre que llegan nuevos números, se ubica en la posición de segundos y se corren los otros dígitos una posición hacia la izquierda
  timeove[0] = timeove[1];
  timeove[1] = timeove[2];
  timeove[2] = timeove[3];
  timeove[3] = key;
  
  timeroven = atoi(timeove);          // Convertir de char a int
  return 0;                           // Si no es ni * ni #, no se ha iniciado el proceso*/
}


void setup()
{
  lcd.display();
  lcd.begin(16, 2);
  
  pinMode(pwm,OUTPUT) ;    
  pinMode(in_1,OUTPUT) ;  
  pinMode(in_2,OUTPUT) ;
  pinMode(ledPin, OUTPUT);
  pinMode(pin1, INPUT);
  digitalWrite(pin1, LOW);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  
  // inicializar timer3 
  noInterrupts();           // deshabilitar todas las interrupciones
  TCCR1A = 0;               // establecer todo el registro TCCR3A a 0
  TCCR1B = 0;               // establecer todo el registro TCCR3B a 0
  TCNT1  = 0;               // inicializar el valor del contador interno a 0. Cuenta 1 cada que pasa un ciclo.
  
  //establecer registros para incrementos de 1hz
  // Definir según el preescalador a cuantos ciclos equivale un segundo. 
  OCR1A = 15624/2;         // = (16*10^6 Hz) / (1Hz*1024) - 1 (must be <65536). Es la cantidad de ciclos que pasan en un segundo.
    
  /*Enceder modo CTC = Clear Time Counter
   * TCCR3B = Registro de control del timer
   */
  TCCR1B = 15;
  //TCCR3B |= (1 << CS32) | (1 << CS30);  
  
  // registro de máscara de interrupción
  TIMSK1 |= (1 << OCIE1A);
  interrupts();             // habilitar todas las interrupciones
}

// Handler de la interrupción
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  blink();
  timerflag = 1;
}


//----------- FUNCIONES MOTOR ----------//
void stopB()
{
  digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,HIGH) ;
}

void clockwiseB()
{
  digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,LOW) ;
}

void anclockwiseB(){
  digitalWrite(in_1,LOW) ;
  digitalWrite(in_2,HIGH) ;
}

//Función para imprimir el tiempo y final
void updateDisplay(int minutes, int seconds, int bflag) {
  if (bflag == 1){                    //Inicia el proceso
   lcd.setCursor(0,0);
   lcd.print(minutes/10);             //Decenas de minutos
   lcd.print(minutes%10);             //Unidades de minutos
   lcd.print(":");
   lcd.print(seconds/10);             //Decenas de segundos
   lcd.print(seconds%10);             //Unidades de segundos
  }
   if (bflag == 2){                   //Termina el proceso
    lcd.setCursor(0,0);
    lcd.print("--END--");  
   }
}

//Cuenta regresiva teniendo en cuenta el cambio de minuto
int countdown(){
  if (timeroven == 0){               //Termina proceso
    return 2; 
    }  
   if (timeroven%100 == 0){         // Si termina en 00, pasar a 59 s en vez de 99
       timeroven= (((timeroven/100) - 1)*100)+59;
   }
   else {
    timeroven -= 1;                 //Restar segundos
    }
   return 1;
}


void loop()
{
   switch(bflag){                                          
        case 0:                                            //Inicio o pausa
        digitalWrite(ledPin, state);
        stopB();                  
         key = keypad.getKey();
          if (key){
              bflag = some(key);                          //Si *, iniciar. Si #, terminar.
          }
          minutes = timeroven/100;                        //en timeroven, los miles y las centenas equivalen a los minutos
          seconds= timeroven%100;                         //en timeroven, las decimas y los segundos equivalen a los segundos
          updateDisplay(minutes,seconds, 1);
        break;
        
        case 1:                                           //Correr
        analogWrite(pwm,70) ;
          clockwiseB();                                   //Encender motor
          key = keypad.getKey();
          if (key == '#'){
              bflag = 0;
              break;
          }
          if (timerflag == 1){                         //Pasa un segundo
            timerflag = 0;
            bflag = countdown();                        //Si countdown llega a 0 (bflag=2), cambio a paso 2
            minutes = timeroven/100;
            seconds= timeroven%100;
            updateDisplay(minutes,seconds, bflag);
          }
         break;
       
        case 2:                                              //Terminar
        digitalWrite(ledPin, state);
        stopB();                                             //Apagar motor
        key = keypad.getKey();
        if (key == '*'){                                     //Aceptar
            lcd.clear();
            lcd.setCursor(0,0);
            bflag = 0;                                       //Cambiar a caso 0
        }
        for (int i = 0; i <= 3; i++){                        //Poner timeove en 0
            timeove[i] = '0';
          }
        timeroven = 0;                                       //Reiniciar 
        minutes = timeroven/100;
        seconds= timeroven%100;
        updateDisplay(minutes,seconds, bflag);    
        break;
       }
    }
