#include <Time.h>
#include <TimeLib.h>
#include <sstimer.h>
#define speakerPin 6
#define speakerInput 12

unsigned long lastClick; //Required for white noise
unsigned long currentMicros[4];

//-----------------------------------------------DEFINE VALUES--------------------------------------------------

int input[4] = {8, 9, 10, 11}; //Array of input pins, must be in correct order o f 8 9 10 11
int output[4] = {2, 3, 4, 5}; //Array of outputs, must be in corresponding order of 2 3 4 5
int TTLread[4]; //Array to read TTL values
int speakerVal; //White noise input value
int speaker[4] = {LOW,LOW,LOW,LOW};
bool speakerState[4];
bool onState = false;
unsigned long previousMicros[4];
const long interval = 77;
const long whiteNoiseInterval = 77;


  //--------------------------------------------SETUP INPUT/OUTPUT PINS-------------------------------------------

void setup() {

  for (int i = 0; i < 4; i++) {
    pinMode(input[i], INPUT); //Sets up the input pins

    pinMode(output[i], OUTPUT); //Sets up the output pins
    digitalWrite(output[i], LOW); // turns the speakers off
  }
  pinMode(speakerPin, OUTPUT); //Setup both input and output for whitenoise
  pinMode(speakerInput, INPUT);
  digitalWrite(speakerPin, LOW);
  lastClick = micros();

}

//--------------------------------------------WHITENOISE-----------------------------------------------------

#define LFSR_INIT  0xfeedfaceUL

#define LFSR_MASK  ((unsigned long)( 1UL<<31 | 1UL <<15 | 1UL <<2 | 1UL <<1  ))

unsigned int generateNoise() { //Creates white noise
  // See https://en.wikipedia.org/wiki/Linear_feedback_shift_register#Galois_LFSRs
  static unsigned long int lfsr = LFSR_INIT;  /* 32 bit init, nonzero */
  /* If the output bit is 1, apply toggle mask.
                                     The value has 1 at bits corresponding
                                     to taps, 0 elsewhere. */

  if (lfsr & 1) {
    lfsr =  (lfsr >> 1) ^ LFSR_MASK ;
    return (1);
  }
  else         {
    lfsr >>= 1;
    return (0);
  }
}


//-------------------------------------CODE TO ALLOW NOISES TO BE MADE--------------------------------------


void loop() {
  
  for(int i = 0; i < 4; i++) {
      TTLread[i] = digitalRead(input[i]); //Reads the current status of the TTLinput
      currentMicros[i] = micros();
    
          if(TTLread[i] == 0 && onState){
              if(currentMicros[i] - previousMicros[i] >= whiteNoiseInterval){
                previousMicros[i] = currentMicros[i];
                  if (speaker[i] == LOW) {
                    speaker[i] = HIGH;
                  } else {
                    speaker[i] = LOW;
                  }
                  digitalWrite(output[i], speaker[i]);
                }
            }else if(TTLread[i] == 0) {
              if(currentMicros[i] - previousMicros[i] >= interval){
              previousMicros[i] = currentMicros[i];
                if (speaker[i] == LOW) {
                  speaker[i] = HIGH;
                } else {
                  speaker[i] = LOW;
                }
                digitalWrite(output[i], speaker[i]);
              } 
          }else{
            digitalWrite(output[i], LOW);
          }
  }

  //-----------------------------------------------WHITE NOISE CODE----------------------------------------------------

  speakerVal = digitalRead(speakerInput); //Stores the value of the white noise speaker

  if (speakerVal == 0) { //Checks if the TTL signal for the whitenoise speaker is on
    if ((micros() - lastClick) > 50 ) { // Changing this value changes the frequency.
      lastClick = micros();
      digitalWrite (speakerPin, generateNoise()); //Generate a white noise
      }
    }
   else if (speakerVal == 1) { //Checks if a signal has come telling to turn the speaker off, and if the speaker is currently on
    digitalWrite(speakerPin,LOW); //If the signal is turned off, then turn the speaker off
  }

}
