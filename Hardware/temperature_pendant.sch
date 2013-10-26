EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Aerodyne_v2
LIBS:temperature_pendant-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Temperature Pendant"
Date "26 oct 2013"
Rev "1"
Comp "Matthew E. Nelson"
Comment1 "mnelson@aerodynelabs.com"
Comment2 "www.rfgeeks.com"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BATTERY BT1
U 1 1 5264A202
P 4000 3200
F 0 "BT1" H 4000 3400 50  0000 C CNN
F 1 "BATTERY" H 4000 3010 50  0000 C CNN
F 2 "~" H 4000 3200 60  0000 C CNN
F 3 "~" H 4000 3200 60  0000 C CNN
	1    4000 3200
	0    1    1    0   
$EndComp
$Comp
L TMP3X U2
U 1 1 5264B6FD
P 7900 3550
F 0 "U2" H 8100 3200 60  0000 C CNN
F 1 "TMP3X" H 7700 3850 60  0000 C CNN
F 2 "" H 7900 3550 60  0000 C CNN
F 3 "" H 7900 3550 60  0000 C CNN
	1    7900 3550
	1    0    0    -1  
$EndComp
$Comp
L SW-MOM SW1
U 1 1 5264B70C
P 7950 4900
F 0 "SW1" H 7950 5050 60  0000 C CNN
F 1 "SW-MOM" H 7950 4800 60  0000 C CNN
F 2 "~" H 7950 4900 60  0000 C CNN
F 3 "~" H 7950 4900 60  0000 C CNN
	1    7950 4900
	1    0    0    -1  
$EndComp
$Comp
L BLINKM/MINM U1
U 1 1 5265EB43
P 5500 3700
F 0 "U1" H 5500 3450 60  0000 C CNN
F 1 "BLINKM/MINM" H 5500 4150 60  0000 C CNN
F 2 "" H 5500 3700 60  0000 C CNN
F 3 "" H 5500 3700 60  0000 C CNN
	1    5500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3500 4000 3600
$Comp
L GND #PWR2
U 1 1 5265EE24
P 4000 3600
F 0 "#PWR2" H 4000 3600 30  0001 C CNN
F 1 "GND" H 4000 3530 30  0001 C CNN
F 2 "" H 4000 3600 60  0000 C CNN
F 3 "" H 4000 3600 60  0000 C CNN
	1    4000 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2900 4000 2800
$Comp
L +BATT #PWR1
U 1 1 5265EE65
P 4000 2800
F 0 "#PWR1" H 4000 2750 20  0001 C CNN
F 1 "+BATT" H 4000 2900 30  0000 C CNN
F 2 "" H 4000 2800 60  0000 C CNN
F 3 "" H 4000 2800 60  0000 C CNN
	1    4000 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3400 4950 3400
Wire Wire Line
	4700 3600 4950 3600
Wire Wire Line
	4700 3800 4950 3800
$Comp
L +BATT #PWR3
U 1 1 5265EF91
P 4700 3800
F 0 "#PWR3" H 4700 3750 20  0001 C CNN
F 1 "+BATT" H 4700 3900 30  0000 C CNN
F 2 "" H 4700 3800 60  0000 C CNN
F 3 "" H 4700 3800 60  0000 C CNN
	1    4700 3800
	1    0    0    1   
$EndComp
$Comp
L GND #PWR4
U 1 1 5265EFA0
P 4700 3650
F 0 "#PWR4" H 4700 3650 30  0001 C CNN
F 1 "GND" H 4700 3580 30  0001 C CNN
F 2 "" H 4700 3650 60  0000 C CNN
F 3 "" H 4700 3650 60  0000 C CNN
	1    4700 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3550 8550 3550
Text GLabel 8550 3550 2    39   Input ~ 0
TEMP
Wire Wire Line
	7900 4000 7900 4200
Wire Wire Line
	7900 2900 7900 3100
$Comp
L +BATT #PWR5
U 1 1 5265F004
P 7900 2900
F 0 "#PWR5" H 7900 2850 20  0001 C CNN
F 1 "+BATT" H 7900 3000 30  0000 C CNN
F 2 "" H 7900 2900 60  0000 C CNN
F 3 "" H 7900 2900 60  0000 C CNN
	1    7900 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR6
U 1 1 5265F013
P 7900 4200
F 0 "#PWR6" H 7900 4200 30  0001 C CNN
F 1 "GND" H 7900 4130 30  0001 C CNN
F 2 "" H 7900 4200 60  0000 C CNN
F 3 "" H 7900 4200 60  0000 C CNN
	1    7900 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3600 4700 3650
Wire Wire Line
	6050 3400 6250 3400
Wire Wire Line
	6050 3600 6250 3600
Wire Wire Line
	6050 3800 6250 3800
Text GLabel 6250 3400 2    39   Input ~ 0
MISO
Text GLabel 6250 3600 2    39   Input ~ 0
TEMP
Text GLabel 6250 3800 2    39   Input ~ 0
SW
$Comp
L CONN-6 J1
U 1 1 5265F63E
P 4000 4850
F 0 "J1" H 4000 5200 60  0000 C CNN
F 1 "CONN-6" H 4000 4500 60  0000 C CNN
F 2 "~" H 4000 4850 60  0000 C CNN
F 3 "~" H 4000 4850 60  0000 C CNN
	1    4000 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 4900 7450 4900
Text GLabel 7450 4900 0    39   Input ~ 0
SW
Wire Wire Line
	8200 4900 8450 4900
$Comp
L +BATT #PWR?
U 1 1 526BECF7
P 8450 4900
F 0 "#PWR?" H 8450 4850 20  0001 C CNN
F 1 "+BATT" H 8450 5000 30  0000 C CNN
F 2 "" H 8450 4900 60  0000 C CNN
F 3 "" H 8450 4900 60  0000 C CNN
	1    8450 4900
	1    0    0    -1  
$EndComp
Text GLabel 4700 3400 0    39   Input ~ 0
RST
Wire Wire Line
	4400 4600 4600 4600
Wire Wire Line
	4400 4700 5000 4700
Wire Wire Line
	4400 4800 5000 4800
Wire Wire Line
	4400 4900 4600 4900
Wire Wire Line
	4400 5000 4600 5000
Wire Wire Line
	4400 5100 4600 5100
Text GLabel 4600 4600 2    39   Input ~ 0
RST
Text GLabel 4600 4900 2    39   Input ~ 0
SW
Text GLabel 4600 5000 2    39   Input ~ 0
TEMP
Text GLabel 4600 5100 2    39   Input ~ 0
MISO
$Comp
L +BATT #PWR?
U 1 1 526BEF1C
P 5000 4800
F 0 "#PWR?" H 5000 4750 20  0001 C CNN
F 1 "+BATT" H 5000 4900 30  0000 C CNN
F 2 "" H 5000 4800 60  0000 C CNN
F 3 "" H 5000 4800 60  0000 C CNN
	1    5000 4800
	1    0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 526BEF30
P 5000 4700
F 0 "#PWR?" H 5000 4700 30  0001 C CNN
F 1 "GND" H 5000 4630 30  0001 C CNN
F 2 "" H 5000 4700 60  0000 C CNN
F 3 "" H 5000 4700 60  0000 C CNN
	1    5000 4700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
