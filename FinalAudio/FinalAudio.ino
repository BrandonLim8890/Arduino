 #include <Time.h>
#include <TimeLib.h>
#define speakerPin 6
#define speakerInput 12

unsigned long lastClick; //Required for white noise

//-----------------------------------------------DEFINE VALUES--------------------------------------------------

int frequency = 6000; //freq at 6KHz. Able to change
int duration = 5000; //Duration of each beep set to 1s

int TTLreadOld[4]; //Array of old values to compare
int input[4] = {8,9,10,11}; //Array of input pins, must be in correct order o f 8 9 10 11
int output[4] = {2,3,4,5}; //Array of outputs, must be in corresponding order of 2 3 4 5
int TTLread[4]; //Array to read TTL values
int speakerVal; //White noise input value
bool onState = false; //Set the speaker to off

//--------------------------------------------SETUP INPUT/OUTPUT PINS-------------------------------------------

void setup() {

  Serial.begin(9600);
  
  for(int i = 0; i < 4; i++){
      pinMode(input[i], INPUT); //Sets up the input pins
      TTLreadOld[i] = digitalRead(input[i]); //Stores the initla value of the TTLinput
      digitalWrite(input[i], LOW);
      
      pinMode(output[i], OUTPUT); //Sets up the output pins
      noTone(output[i]); //Silences all the speakes
    }
    pinMode(speakerPin, OUTPUT); //Setup both input and output for whitenoise
    pinMode(speakerInput, INPUT);
    noTone(speakerPin);
    lastClick = micros();
    Serial.println("pins setup");

}

//--------------------------------------------WHITENOISE-----------------------------------------------------

#define LFSR_INIT  0xfeedfaceUL

#define LFSR_MASK  ((unsigned long)( 1UL<<31 | 1UL <<15 | 1UL <<2 | 1UL <<1  ))

unsigned int generateNoise(){ //Creates white noise
  // See https://en.wikipedia.org/wiki/Linear_feedback_shift_register#Galois_LFSRs
   static unsigned long int lfsr = LFSR_INIT;  /* 32 bit init, nonzero */
   /* If the output bit is 1, apply toggle mask.
                                    * The value has 1 at bits corresponding
                                    * to taps, 0 elsewhere. */

   if(lfsr & 1) { lfsr =  (lfsr >>1) ^ LFSR_MASK ; return(1);}
   else         { lfsr >>= 1;                      return(0);}
}


//-------------------------------------CODE TO ALLOW NOISES TO BE MADE--------------------------------------


void loop() {
  
  for(int i = 0; i < 4; i++) {
      TTLread[i] = digitalRead(input[i]); //Reads the current status of the TTLinput
      if(TTLread[i] == 0) { //Checks if TTL is on
          tone(output[i],frequency);
          Serial.print("Speaker "); //Prints out the time at which each speaker is on
          Serial.print(i + 1);
          Serial.print(" is on at ");
          Serial.print(hour());
          printDigits(minute());
          printDigits(second());
          Serial.println();
        }else{
          noTone(output[i]);
          }
    }

//-----------------------------------------------WHITE NOISE CODE----------------------------------------------------

   speakerVal = digitalRead(speakerInput); //Stores the value of the white noise speaker 

    if(speakerVal == 0) { //Checks if the TTL signal for the whitenoise speaker is on 
        if ((micros() - lastClick) > 50 ) { // Changing this value changes the frequency.
          lastClick = micros();
          digitalWrite (speakerPin, generateNoise()); //Generate a white noise
          if(!onState) { //If the current state of the speaker is off, turn it on and output it to th serial port
              Serial.print("White noise started at "); //prints out the time of when the action started
              Serial.print(hour());
              printDigits(minute());
              printDigits(second()); 
              Serial.println();
              onState = true; //Define that the speaker is "on", so that it doesn't print the same thing many times
            }
        }
        
      }else if(speakerVal== 1 && onState){ //Checks if a signal has come telling to turn the speaker off, and if the speaker is currently on
            noTone(speakerPin); //If the signal is turned off, then turn the speaker off
            Serial.print("White noise stopped at "); //show what time the speaker turned off
            Serial.print(hour());
            printDigits(minute());
            printDigits(second());
            Serial.println();
            onState = false; //set the speaker to off
          }

}


//---------------------------------------------------TIME---------------------------------------------------------


void printDigits(int digits) { //Function to print out digits for the time
    Serial.print(":");
    if (digits < 10) //Makes it a two digit number no matter what to accurately display the timee
    Serial.print('0');
    Serial.print(digits);
  }
