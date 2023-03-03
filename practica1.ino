#define ledPin 8
#include <LiquidCrystal.h>
#include <Keypad.h>

const int rs = 53, en = 49, d4 = 47, d5 = 45, d6 = 43, d7 = 41;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//4x4 Matrix key pad
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Four columns
  
  // Define the Keymap
  char keys[ROWS][COLS] = 
  {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','#'}

  };

byte rowPins[ROWS] = { 21, 20, 19, 1 };
byte colPins[COLS] = { 17, 16, 15}; 

// Create the Keypad
//Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 
//           ROWS, COLS );
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 
           ROWS, COLS );

int timeroven=200;
int minutes;
int seconds;
int button;
const int pwm = 12 ;  //initializing pin 12 as pwm
const int in_1 = 6;
const int in_2 = 5;
int flag;

void setup()

{
  //lcd.noDisplay();
  lcd.display();
  lcd.begin(16, 2); 
  pinMode(pwm,OUTPUT) ;    
  pinMode(in_1,OUTPUT) ;  
  pinMode(in_2,OUTPUT) ;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,HIGH);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 62499;            // compare match register 16MHz/256 - 1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

// Función está toggleando el pin cada segundo
//Handler de la interrupción
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);   // toggle LED pin
  flag = 1;
}

void loop()
{
  char key = keypad.getKey();
  if (key){
    if (key == '1'){
     lcd.print("Key is pressed"); 
    }
  }
  if (flag == 1){
    flag = 0;
    //countdown();
    minutes= timeroven/100;
    seconds= timeroven%100;
    //updateDisplay(minutes,seconds);
  /*char key = keypad.getKey();
  if(key = '1')  
    {
      //lcd.setCursor(1, 1);
      lcd.print("idk");
    }else {
      lcd.clear();
    }*/
}}

void countdown(){
// timeroven % 100 == 0;
  if (timeroven == 0){
       digitalWrite(ledPin,LOW);
       return;
    }  
   int minch = timeroven%100;
   if (minch == 0){
       timeroven= ((timeroven/100-1)*100)+59;
   }
   
   else {
    timeroven -= 1;
    } 
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

// ------------------------------------------

void updateDisplay(int minutes, int seconds) {
   lcd.setCursor(0,0);
   lcd.print(minutes);
   lcd.print(":");
   lcd.print(seconds);
}
