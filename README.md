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

The Software PWM routine was original written by Ernst Christensen and was found in the following Arduino forum thread: 
http://arduino.cc/forum/index.php/topic,75334.0.html

All credit goes to him for the handy routine
