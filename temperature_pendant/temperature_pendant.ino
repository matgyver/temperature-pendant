/*
 * BlinkM Necklace with temperature sensor and switch
 * This will read in a temperature sensor and set the color
 * of the RGB LED accordingly.  A switch is used to rotate between
 * Temp mode, color mode and simulated off (LED is off, proc still running)
 * 
 * 2013 Matthew E. Nelson - Aerodyne Labs
 * www.rfgeeks.com          www.aerodynelabs.com
 */

/* Notes on this project - Please Read This if you are having problems!

Known Issues

*  There is some delay with the switch and switching modes, this is due to the software PWM, see note below

---------------------------------------------------------------------------------------------------------------

This project uses a BlinkM or MiniM module to read a temp sensor (analog) and use that to control the RGB LED on the module
The BlinkM/MiniM modules have an ATTiny85 (MiniM) or ATTiny44 (BlinkM).  While these processors do have PWM, the pins on the RGB
are not tied to the hardware PWM outputs except for Blue.  Therefore, a software based PWM was needed.  The software PWM that I am
using (see note below on original Author) does a pretty good job, but does have some limitiations.  This is mainly due to the fact 
that the ATTiny processors only have 1 timer.  The values that I choose was picked to produce as smooth of a result I could get while 
keeping the functionaility consistent.

Programming this requires a programmer as the BlinkM or MiniM do not have an arduino comptabible bootloader (or a HW serial port for that matter).  
For this reason you will need an ISP.  Fortunatly you can use an Arduino as an ISP.  Instructions for programming the BlinkM and MiniM can be 
found on ThingM's blog:  http://blog.thingm.com/2011/03/blinkm-smart-led-as-the-smallest-arduino/.  It's a little dated but still applies.

In addition, you will need to add the ATTiny44 and 85 processors to the Arduino IDE.  You can information about this on MIT's website located
here:  http://hlt.media.mit.edu/?p=1695.  There is a known issue with the Arduino IDE being able to address the full 8KB on the processor.  To fix
this issue, see this:  https://github.com/TCWORLD/ATTinyCore/tree/master/PCREL%20Patch%20for%20GCC.  Basically there is a problem with the version
of the compilier that Arduino uses, so this patch will fix that bug that limits it from compiling for programs larger than 4KB.  

UPDATE!
Through this process I ended up switching cores.  This was mainly due to the addition of using an ISR.  You can find the core I used here https://code.google.com/p/arduino-tiny/

Another note on the ISR.  The ISR does indeed work, however since the softwarePWM also uses that timer, it has to finish that before it goes to the ISR.  The result is that if
you push the button and it is pulsing a color, it will not move to the next mode until the the fade is complete.  Remember, it's a "feature".

*/

/*
The Software PWM routine was original written by Ernst Christensen and was found in the following Arduino forum thread: 
http://arduino.cc/forum/index.php/topic,75334.0.html

All credit goes to him for the handy routine
*/

#include "PinChangeInterrupt.h"
// BlinkM / BlinkM MinM pins
const int redPin = 3;  // 
const int grnPin = 4;  //
const int bluPin = 1;  // PWM, will blink when programming
const int sdaPin = 0;  // PWM, 'd' pin, can be digital I/O
const int sclPin = 2;  // A/D, 'c' pin, can be digital I/O, or analog input

volatile long lastDebounceTime = 0;   // the last time the interrupt was triggered

//Defines
#define fadeSpeed 15             //Defines how fast we fade in the SoftPWM routine
#define debounceDelay 50    // the debounce time in ms; decrease if quick button presses are ignored, increase
                             //if you get noise (multipule button clicks detected by the code when you only pressed it once)

#define INTERNAL2V56_NO_CAP (6)
//Variable setup and initialize
int SW_Pin = sdaPin;             //Digital in pin for reading switch
const int tmp36_Pin  = A1;       //Analog in pin for TMP36 temp sensor and A1 because Arduino is stupid
const int numReadings = 10;      //Number of readings to take to average out the values

int temperature = 0;
int SW_Status;
int mode = 1;

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

int r,g,b = 0;

//  Setup routine, configures our pins and initializes a few things
void setup() 
{
  //configures the pins to the BlinkM module
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);

  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   

// Configures the Switch and TMP36 sensor pins
  pinMode(SW_Pin, INPUT);
  digitalWrite(SW_Pin, LOW);
  pinMode(tmp36_Pin, INPUT);
  
//Turn on the internal voltage reference for ADC since our source varies, uses the ATTiny 2.56 internal ref.
  analogReference(INTERNAL2V56_NO_CAP);  
  
//Setup interrupt for switching modes
  attachPcInterrupt(0,SWITCH_ISR,RISING);
  
//  This is a short "Hello" to show we have initialized and setup everything
// This will blink each color once to make sure the RGB LED is functioning

  digitalWrite(grnPin, HIGH);     
  delay(250);
  digitalWrite(grnPin, LOW);
  delay(250);
  digitalWrite(bluPin, HIGH);     
  delay(250);
  digitalWrite(bluPin, LOW);
  delay(250);
  digitalWrite(redPin, HIGH);     
  delay(250);
  digitalWrite(redPin, LOW);
  delay(250);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluPin, HIGH);
  digitalWrite(grnPin, HIGH);
  delay(250);
  digitalWrite(redPin, LOW);
  digitalWrite(bluPin, LOW);
  digitalWrite(grnPin, LOW);
  delay(250);
}

//  Main Loop

void loop() 
{
  if (mode > 6) 
    mode = 1;
    
    switch (mode) {
      case 1:  //Set LED color based on temp, default mode
        set_color_by_temp();
        break;
      case 2: //Rotate flashing each pin on the RGB
        flash_rgb();
        break;
      case 3: //Off mode
        pulse_purple();
        break;
      case 4: //Off mode
        pulse_red();
        break;
      case 5: //Off mode
        pulse_white();
        break;
      case 6:
        digitalWrite(redPin, LOW);
        digitalWrite(bluPin, LOW);
        digitalWrite(grnPin, LOW);
        delay(250);
        break;
      default:
        mode = 1;     
  
  }

}

//Switch ISR Routine
void SWITCH_ISR(){
  
  if ((millis() - lastDebounceTime) > debounceDelay) //if current time minus the last trigger time is greater than
    {                                                  //the delay (debounce) time, button is completley closed.
      lastDebounceTime = millis();
   
      mode++;
    }

}

void set_color_by_temp (void) {
  
  int sp = 15;

  //int temperature = temp_read();
  int sensorValue = analogRead(tmp36_Pin);
  int temperature = sensorValue * 2.56; 
  
/* Average routine
   In order to help smooth out the readings, we will take several measurements
   and average them out.  This does mean that upon initial startup, it will start
   at zero and pulse blue.  Once enough readings have been taken it will go to the 
   right color
   */
   
  // subtract the last reading:
  total = total - readings[index]; 
  
  // read from the sensor:  
  readings[index] = temperature; 
  
  // add the reading to the total:
  total = total + readings[index];       
  
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  temperature = total / numReadings;
  
  //Here we determine the color base on the readings in millivolts.  This was done as doing
  //a full conversion to temp was causing some issues.  We are not sending out the temp data
  //So there is no need for this, this however keeps it fairly simple
  
  //If less than 0 C, pulse blue
  if (temperature < 500)
  {
    for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    softPWM(bluPin, fadeValue, sp);
    digitalWrite(grnPin, LOW);
    digitalWrite(redPin, LOW);
    }
    
    for(int fadeValue = 253; fadeValue > 1; fadeValue--) { 
    softPWM(bluPin, fadeValue, sp);
    digitalWrite(grnPin, LOW);
    digitalWrite(redPin, LOW);
    }
  }
  //If between 0 and 25 C, fade from blue to green
  else if(temperature < 750 && temperature >= 500)
  {
    g = map(temperature, 500, 750, 1, 254);
    b = map(temperature, 500, 750, 254, 1);
    softPWM(grnPin, g, sp);
    softPWM(bluPin, b, sp);
    digitalWrite(redPin, LOW);
    
  }
  //If between 25 and 50 C, fade from green to red
  else if(temperature < 950 && temperature >= 750)
  {
    r = map(temperature, 750, 950, 1, 254);
    g = map(temperature, 750, 950, 254, 1);
    softPWM(grnPin, g, sp);
    softPWM(redPin, r, sp);
    digitalWrite(bluPin, LOW);
    
  }
  
  //If the temperature is hot, over 50 C, pulse red quickly
  else if (temperature >= 950)
  {
    for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    softPWM(redPin, fadeValue, 5);
    digitalWrite(grnPin, LOW);
    digitalWrite(bluPin, LOW);
    }
    
    for(int fadeValue = 253; fadeValue > 1; fadeValue--) { 
    softPWM(redPin, fadeValue, 5);
    digitalWrite(grnPin, LOW);
    digitalWrite(bluPin, LOW);
    }
  }
}

//A simple routine that rotates from flashing each color
void flash_rgb(void) {
  digitalWrite(grnPin, HIGH);     
  delay(150);
  digitalWrite(grnPin, LOW);
  delay(150);
  digitalWrite(bluPin, HIGH);     
  delay(150);
  digitalWrite(bluPin, LOW);
  delay(150);
  digitalWrite(redPin, HIGH);     
  delay(150);
  digitalWrite(redPin, LOW);
  delay(150);
  digitalWrite(redPin, LOW);
  digitalWrite(bluPin, LOW);
  digitalWrite(grnPin, LOW);
  delay(150);
}

void pulse_purple(void) {
  int fadeValue = 0;
  
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    // sets the value (range from 0 to 255):
    digitalWrite(grnPin, LOW);
    softPWM(redPin, fadeValue, fadeSpeed);
    softPWM(bluPin, fadeValue, fadeSpeed);
                         
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 254; fadeValue > 1; fadeValue--) { 
    // sets the value (range from 0 to 255):
    digitalWrite(grnPin, LOW);
    softPWM(redPin, fadeValue, fadeSpeed);
    softPWM(bluPin, fadeValue, fadeSpeed);                        
  }
}

void pulse_white(void) {
  int fadeValue = 0;
  
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    // sets the value (range from 0 to 255):
    softPWM(grnPin, fadeValue, fadeSpeed);
    softPWM(redPin, fadeValue, fadeSpeed);
    softPWM(bluPin, fadeValue, fadeSpeed);
                         
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 254; fadeValue > 1; fadeValue--) { 
    // sets the value (range from 0 to 255):
    softPWM(grnPin, fadeValue, fadeSpeed);
    softPWM(redPin, fadeValue, fadeSpeed);
    softPWM(bluPin, fadeValue, fadeSpeed);                        
  }
}

void pulse_red(void) {
  int fadeValue = 0;
  
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    // sets the value (range from 0 to 255):
    digitalWrite(grnPin, LOW);
    softPWM(redPin, fadeValue, fadeSpeed);
    digitalWrite(bluPin, LOW);
                         
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 254; fadeValue > 1; fadeValue--) { 
    // sets the value (range from 0 to 255):
    digitalWrite(grnPin, LOW);
    softPWM(redPin, fadeValue, fadeSpeed);
    digitalWrite(bluPin, LOW);                        
  }
}

//software PWM function that fakes analog output
// Function reads in the pin,  duty cycle (brightness), and fade speed)
// Duty cycle is 1-254
void softPWM(int pin, int freq, int sp) {
  digitalWrite(pin,HIGH); //on
  delayMicroseconds(sp*freq);
  digitalWrite(pin,LOW); //off
  delayMicroseconds(sp*(255-freq));
}
