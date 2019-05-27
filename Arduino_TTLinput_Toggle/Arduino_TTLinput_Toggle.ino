#include <Time.h>
#include <TimeLib.h>
#define speakerPin 6

unsigned long lastClick;


int frequency = 6000;
int duration = 1000;
int TTLreadold1 = digitalRead(8);
int TTLreadold2 = digitalRead(9);
int TTLreadold3 = digitalRead(10);
int TTLreadold4 = digitalRead(11);
int TTLreadold5 = digitalRead(12);

void setup() {

  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12,INPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.println("pins setup");

  
  pinMode(speakerPin,OUTPUT);
  lastClick = micros();   
  noTone(2);
  noTone(3);
  noTone(4);
  noTone(5);
  noTone(6);
  
}

#define LFSR_INIT  0xfeedfaceUL


#define LFSR_MASK  ((unsigned long)( 1UL<<31 | 1UL <<15 | 1UL <<2 | 1UL <<1  ))

unsigned int generateNoise(){ 
  // See https://en.wikipedia.org/wiki/Linear_feedback_shift_register#Galois_LFSRs
   static unsigned long int lfsr = LFSR_INIT;  /* 32 bit init, nonzero */
   /* If the output bit is 1, apply toggle mask.
                                    * The value has 1 at bits corresponding
                                    * to taps, 0 elsewhere. */

   if(lfsr & 1) { lfsr =  (lfsr >>1) ^ LFSR_MASK ; return(1);}
   else         { lfsr >>= 1;                      return(0);}
}



void loop() {

  noTone(2);
  noTone(3);
  noTone(4);
  noTone(5);

  
  int TTLread1 = digitalRead(8);
  if(TTLread1 != TTLreadold1) {
    tone(2, frequency,duration);
    delay(duration);
    TTLreadold1 = TTLread1;
    Serial.print("Speaker 1 is on at ");
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
  }

  
  int TTLread2 = digitalRead(9);
  if(TTLread2 != TTLreadold2) {
    tone(3, frequency,duration);
    delay(duration);
    TTLreadold2 = TTLread2;
    Serial.print("Speaker 2 is on at ");
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
  }


  int TTLread3 = digitalRead(10);
  if(TTLread3 != TTLreadold3) {
    tone(4, frequency,duration);
    delay(duration);
    TTLreadold3 = TTLread3;
    Serial.print("Speaker 3 is on at ");
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
  }


  int TTLread4 = digitalRead(11);
  if(TTLread4 != TTLreadold4) {
    tone(5, frequency,duration);
    delay(duration);
    TTLreadold4 = TTLread4;
    Serial.print("Speaker 4 is on at ");
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
  }


  int TTLread5 = digitalRead(12);
  if(TTLread5 !=TTLreadold5) {
      if ((micros() - lastClick) > 50 ) { // Changing this value changes the frequency.
        lastClick = micros();
        digitalWrite (speakerPin, generateNoise());
      }
      TTLreadold5 = TTLread5;
      Serial.print("White noise started at ");
      Serial.print(hour());
      printDigits(minute());
      printDigits(second());
      Serial.println();
    }
  
  
}


void printDigits(int digits) {
 // utility function for digital clock display: prints preceding colon and leading 0
 Serial.print(":");
 if (digits < 10)
 Serial.print('0');
 Serial.print(digits);
}


