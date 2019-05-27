/*Brandon Lim. July 10 2018.
 * 
 * This code requires the time nad sstimer libraries in order to run properly. Currently, the piezo speakers will constantly output a sound with a freqency of 6.25KHz.
 */

#include <Time.h>
#include <TimeLib.h>
#include <sstimer.h>
#define speakerPin 12
#define speakerInput 6

unsigned long lastClick; //Required for white noise

//-----------------------------------------------DEFINE VALUES--------------------------------------------------

int output[4] = {PH5, PH6, PB4, PB5}; //Array of ouput pins, must be in correct order. Using Lower level naming scheme as it is faster than using its corresponding numbers
int input[4] = {2,3,4,5}; //Array of inputs, must be in corresponding order
int speakerVal; //White noise input value
bool speakerState[4];

const uint16_t t1_load = 0; //Initially set the intial counter
const uint16_t t1_comp = 4; //Using timer1, check for when that counter reaches 4, and then reset it back down - This determines the period for our function

  //--------------------------------------------SETUP INPUT/OUTPUT PINS-------------------------------------------

void setup() {


  for(int i = 0; i < sizeof(input); i++) {
      pinMode(input[i], INPUT);
    }

  //Setup input/output for whitenoise
  pinMode(speakerPin, OUTPUT); 
  pinMode(speakerInput, INPUT);
  digitalWrite(speakerPin, LOW);

  //Setup counter for white noise
  lastClick = micros();

  //DDR is same as pinMode(OUTPUT); - The last letter corresponds to the port's letter. e.g: port PH5 would be DDRH while port PB4 would be DDRB
  
  DDRH |= (1 << output[0]);
  DDRH |= (1 << output[1]);
  DDRB |= (1 << output[2]);
  DDRB |= (1 << output[3]);

  TCCR1A = 0; //Reset Timer1 Contol

  //Set CTC Mode
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);

  /*Sets the prescalar to be 256. This is the amount of information the processor is executing at a given time. Increasing this would make our code run faster
  thus requiring a change in timing for the frequency to stay the same. Uses binary to determine.
                                                      0, 0, 1 - Prescalar of 1
                                                      0, 1, 0 - Prescalar of 8
                                                      0, 1, 1 - Prescalar of 64
                                                      1, 0, 0 - Prescalar of 256
                                                      1, 0, 1 - Prescalar of 1024
  */
  TCCR1B |= (1 << CS12); //Equivalent to a 1
  TCCR1B &= ~(1 << CS11); //Equivalent to a 0
  TCCR1B &= ~(1 << CS10);


  
  TCNT1 = t1_load; //Sets initial starting point for timer
  OCR1A = t1_comp; //Sets when Timer1 should reset e.g. our period

  TIMSK1 = (1 << OCIE1A); //Enables Timer1 to reset when it reaches OCR1A

  //Enables global interrupts
  sei(); 

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


void loop() {


  //-----------------------------------------------WHITE NOISE CODE----------------------------------------------------

  speakerVal = digitalRead(speakerInput); //Stores the value of the white noise speaker

  if (speakerVal == 0) { //Checks if the TTL signal for the whitenoise speaker is on
    if ((micros() - lastClick) > 50 ) { //Changing this value changes the frequency.
      lastClick = micros(); //Timer that checks when the timer of the board reaches the intended period
      digitalWrite (speakerPin, generateNoise()); //Generate a white noise
      }
    }
   else if (speakerVal == 1) { //Checks if a signal has come telling to turn the speaker off
    digitalWrite(speakerPin,LOW); //If the signal is turned off, then turn the speaker off
  }

}


//------------------------------------------------CODE FOR INDIVIDUAL SPEAKERS-----------------------------------------


ISR (TIMER1_COMPA_vect) {
    
    if(digitalRead(input[0]) == 0) { //Checks TTL signal of each port
      PORTH ^= (1 << output[0]); //Sends voltage to the output port when Timer1 = 0 at the specified frequency.
    }
    if(digitalRead(input[1]) == 0) {
      PORTH ^= (1 << output[1]);
    }
    if(digitalRead(input[2]) == 0) {
      PORTB ^= (1 << output[2]);
    }
    if(digitalRead(input[3]) == 0) {
      PORTB ^= (1 << output[3]);
    }
    
  }
