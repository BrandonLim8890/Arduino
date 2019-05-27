/*Brandon Lim. July 10 2018.
 * 
 * For this project, ensure that you have installed the following Libraries: TVout, pollSerial, and TVoutfonts. These are required in order for the Overlay to work.
 * To get the black bars, you need to edit the TVout library by adding some lines in Arduino/libraries/TVout/video_gen.cpp.
 * In the video_gen.cpp file, add the following lines at the beginning of the void render_line5c() function:
 * if ((display.scanLine > 31) && (display.scanLine < 46)) {

    PORTB &= ~(_BV(2));  // video off for lower lines

  }

 * At the very end of the same function, add the following line:
 *   PORTB |= _BV(2);
 *   This should be enough to add a black line to the top
 */

#include <TVout.h>
#include <fontALL.h>

#define W 136
#define H 107

TVout tv;
unsigned char ploty = 40;
int TTL[5] = {0,0,0,0,0};
int pins[5] = {3,4,5,11,12};    //Numbers that don't work: 0,1,2,6,7,8,9 ---- 10,11,12 are kind of weird
bool on[5];

void setup()  {
  for(int i = 0; i < 5; i++) {
      pinMode(pins[i],INPUT);
    }
  pinMode(10, OUTPUT); //Pin 10 is used for the MOSFET pin to create the black bars
  tv.begin(NTSC, W, H); //Starts the TV output
  initOverlay(); //Starts the overlay process
  tv.select_font(font6x8); //Selecting font size
  tv.fill(0);
  randomSeed(analogRead(0));

}

// Initialize ATMega registers for video overlay capability.
// Must be called after tv.begin().
void initOverlay() {
  TCCR1A = 0;
  // Enable timer1.  ICES0 is set to 0 for falling edge detection on input capture pin.
  TCCR1B = _BV(CS10);

  // Enable input capture interrupt
  TIMSK1 |= _BV(ICIE1);

  // Enable external interrupt INT0 on pin 2 with falling edge.
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01);
}

// Required to reset the scan line when the vertical sync occurs
ISR(INT0_vect) {
  display.scanLine = 0;
}


void loop() {
    for(int i = 0; i < 5; i++) {
        TTL[i] = digitalRead(pins[i]); //Reads all the pins
        if(TTL[i] == 0 && !on[i]){ //Checks the input of the TTL signal
            tv.print(i * (25), 0, "[ ]"); //If there is nothing, just print open brackets to indicate that it isn't on
          }else{
            on[i] = true; //Does this so that once the video changes, it stays turned on even if the TTL signal is lost.
            tv.draw_rect(i * 25, 0, 15, 5, 1, 1); //Fills in the square box if it detects an input
          }
      }
      
}


