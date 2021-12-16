
# GREENHOUSE

### Team members

* Téo Dos Santos
* Gabriel Taffin
* Alexandre Truong
* Renaud Visioli

Link to this file in your GitHub repository:

[https://github.com/AkaiRyussei/Digital-electronics-2/tree/main/Green%20House%20Project](https://github.com/AkaiRyussei/Digital-electronics-2/tree/main/Green%20House%20Project)

### Table of contents

- [GREENHOUSE](#greenhouse)
    - [Team members](#team-members)
    - [Table of contents](#table-of-contents)
  - [Project objectives](#project-objectives)
  - [Hardware description](#hardware-description)
  - [Libraries description](#libraries-description)
  - [Main application](#main-application)
  - [Video](#video)
  - [References](#references)

<a name="objectives"></a>

## Project objectives

Make a monitoring and control system of a GreenHouse, including : light, moisture and temperature sensor and also : irrigation pump control, DC fans and windows opening or closing.

<a name="hardware"></a>

## Hardware description

![Circuit](https://github.com/AkaiRyussei/Digital-electronics-2/blob/main/Green%20House%20Project/Circuit.png?raw=true)

* Arduino Uno board : ATmega328P 
* DHT12 Digital temperature and humidity sensor
* Photoresistor : Light sensor
* Resistor 330 Ohm  x 2
* LED
* Relay Modules
* SG90 Micro-servo motor
  
![IMG_3910](https://user-images.githubusercontent.com/91612064/145234998-c7b4ab60-2436-494e-a160-be616c6de720.jpg)



<a name="libs"></a>

## Libraries description
The libraries used, are libraries used in previous labs : 

* avr/io.h        : AVR device-specific IO definitions 
* avr/interrupt.h : Interrupts standard C library for AVR-GCC 
* stdbool.h       : Boolean type and values 
* timer.h         : Timer library for AVR-GCC
* tdlib.h         : C library. Needed for conversion function
* uart.h          : Peter Fleury's UART library
* twi.h           : TWI library for AVR-GCC
* gpio.h          : Contains functions for controlling AVRs' gpio pin(s).
* util/delay.h    : Contains delay functions

<a name="main"></a>

## Main application


![Flowchart](https://github.com/AkaiRyussei/Digital-electronics-2/blob/main/Green%20House%20Project/flowchart.png?raw=true)

Our project consists of the operation of an autonomous greenhouse. Our greenhouse has sensors: a light sensor, a temperature sensor and a humidity sensor. The objective of our project is to be able to control these variables in real time so that they remain in optimal intervals for the culture of plants. Therefore, depending on the values of the variables, actions can be taken in order to try to keep the variables in the optimal ranges. Irrigation, opening the window or turning on the fans can be activated. 

For this we have considered the optimal value intervals for humidity, temperature and light, and we have defined the set of actions to be performed for each combination of divergence from these intervals.

We realized a dynamic system thanks to the use of two separate interrupts: one realizing the recovery of the sensors' data, while the other one realizes the management of the outputs according to the retrieved data. Both running in parallel, this allows us to act on the outputs of the system without ceasing to update the sensor data.

__PS__ : *We did not have enough time to complete the project during the lab session. Therefore, our complete system could only be partially tested: we were able to test the operation of each output separately as well as the recovery of sensor data. Nevertheless, we are not sure that the outputs of our system behave according to our expectations in relation to the variations of the data recovered by the sensors.* 

<a name="video"></a>

## Video
https://drive.google.com/file/d/1YLNULn7bUl6V9HJiC57NfYxINMQx4-yd/view?usp=sharing

<a name="references"></a>

## References

https://moodle.vut.cz/course/view.php?id=241310

https://github.com/tomas-fryza/Digital-electronics-2

http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf

https://www.handsontec.com/dataspecs/module/2Ch-relay.pdf
