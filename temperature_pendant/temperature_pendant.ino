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

*/

/*
The Software PWM routine was original written by Ernst Christensen and was found in the following Arduino forum thread: 
http://arduino.cc/forum/index.php/topic,75334.0.html

All credit goes to him for the handy routine
*/

// BlinkM / BlinkM MinM pins
const int redPin = 3;  // 
const int grnPin = 4;  //
const int bluPin = 1;  // PWM, will blink when programming
const int sdaPin = 0;  // PWM, 'd' pin, can be digital I/O
const int sclPin = 2;  // A/D, 'c' pin, can be digital I/O, or analog input

#define VOLTAGE_SCALE 4.96       // not quite 5
#define fadeSpeed 30             //Defines how fast we fade in the SoftPWM routine

int SW_Pin = sdaPin;             //Digital in pin for reading switch
const int tmp36_Pin  = A1;       //Analog in pin for TMP36 temp sensor and A1 because Arduino is stupid
const int numReadings = 50;

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
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);

  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   

// Configures the Switch and TMP36 sensor pins
  pinMode(SW_Pin, INPUT);
  //digitalWrite(SW_Pin, LOW);
  pinMode(tmp36_Pin, INPUT);
  
//  This is a short "Hello" to show we have initialized and setup everything

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
  digitalWrite(redPin, LOW);
  digitalWrite(bluPin, LOW);
  digitalWrite(grnPin, LOW);
}

//  Main Loop

void loop() 
{
  
  int switchVal = digitalRead(0);
  if (switchVal == HIGH)
  {
    mode++;
    delay(150); 
  }
  
    switch (mode) {
      case 1:  //Set LED color based on temp, default mode
        set_color();
        break;
      case 2: //Pulse purple LED
        pulse_purple();
        break;
      case 3: //Off mode
        digitalWrite(redPin, LOW);
        digitalWrite(bluPin, LOW);
        digitalWrite(grnPin, LOW);
        break;
      case 4:
        mode = 1;
        break;
      default:
        mode = 1;     
  
  }
  
  //delay(50);

}

float temp_read (void) {
  float tmp36;
  int raw_temp;
  raw_temp = analogRead(tmp36_Pin);
  float voltage = raw_temp * 3.7;
  voltage /= 1024.0;
  tmp36 = (voltage - 0.5) * 100;
  return tmp36;
}

void set_color (void) {
  
  int sp = 20;
  int duty = 250;
  
  // subtract the last reading:
  total= total - readings[index]; 
  
  // read from the sensor:  
  readings[index] = analogRead(tmp36_Pin); 
  
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
  
  //temperature = 0;
  //temperature = analogRead(A1);
  //delay(10);
  if (temperature < 103)
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
  else if(temperature < 158 && temperature >= 103)
  {
    g = map(temperature, 103, 157, 1, 254);
    b = map(temperature, 103, 157, 254, 1);
    /*
    for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    softPWM(bluPin, (fadeValue-b), sp);
    softPWM(grnPin, (fadeValue-g), sp);
    digitalWrite(redPin, LOW);
    }
    
    for(int fadeValue = 253; fadeValue > 1; fadeValue--) { 
    softPWM(bluPin, (fadeValue-b), sp);
    softPWM(grnPin, (fadeValue-g), sp);
    softPWM(redPin, 1, sp);
    }
    */
    softPWM(grnPin, g, sp);
    softPWM(bluPin, b, sp);
    digitalWrite(redPin, LOW);
    
  }
  
  else if(temperature < 180 && temperature >= 158)
  {
    r = map(temperature, 158, 179, 1, 254);
    g = map(temperature, 158, 179, 254, 1);
    /*
    for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    softPWM(bluPin, (fadeValue-b), sp);
    softPWM(grnPin, (fadeValue-g), sp);
    digitalWrite(redPin, LOW);
    }
    
    for(int fadeValue = 253; fadeValue > 1; fadeValue--) { 
    softPWM(bluPin, (fadeValue-b), sp);
    softPWM(grnPin, (fadeValue-g), sp);
    softPWM(redPin, 1, sp);
    }
    */
    softPWM(grnPin, g, sp);
    softPWM(redPin, r, sp);
    digitalWrite(bluPin, LOW);
    
  }
  
  else if (temperature >= 180)
  {
    for(int fadeValue = 1; fadeValue < 254; fadeValue++) { 
    softPWM(redPin, fadeValue, sp);
    digitalWrite(grnPin, LOW);
    digitalWrite(bluPin, LOW);
    }
    
    for(int fadeValue = 253; fadeValue > 1; fadeValue--) { 
    softPWM(redPin, fadeValue, sp);
    digitalWrite(grnPin, LOW);
    digitalWrite(bluPin, LOW);
    }
  }
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

// software PWM function that fakes analog output
// Function reads in the pin,  duty cycle (brightness), and fade speed)
// Duty cycle is 1-254
void softPWM(int pin, int freq, int sp) {
  digitalWrite(pin,HIGH); //on
  delayMicroseconds(sp*freq);
  digitalWrite(pin,LOW); //off
  delayMicroseconds(sp*(255-freq));
}
