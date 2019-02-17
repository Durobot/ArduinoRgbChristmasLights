ArduinoRgbChristmasLights
-------------------------

Simple firmware for RGB Christmas lights for Arduino and similar boards / MCUs.


This program aims to turn Arduino Nano (probably most other Arduinos, or similar boards 
and microcontrollers) into a controller for WS2811 / WS2812(b) -based RGB decoration lights (of Christmas variety).

The program fits into ATMega168 nicely, but, of course, works on ATMega328 just as well (not better, there's no difference).

This is an [Arduino IDE](https://www.arduino.cc/en/Main/Software) project.

### Video demonstration


### Circuit Diagram and photos
![Circuit Diagram](doc/ChristmasLightsCircuit.jpg?raw=true)


### Frimware Installation

 - Download and install [Arduino IDE](https://www.arduino.cc/en/Main/Software), if you
   don't have it installed already. Depending on your OS, you may need to install additional
   USB drivers to be able to upload complied firmware to your board;

 - Run Arduino IDE, go to ``File -> Open`` and pick ArduinoRgbChristmasLights.ino

 - Go to ``Tools -> Board``, and pick your board type. Some boards come in several versions,
   like Arduino Nano, so make sure that ``Tools -> Processor`` is set correctly as well.

 - Disconnect your board from all external connections. It _might_ be OK to leave LED strip
   connected, but you must definitely disconnect external power supply;

 - Connect your board to the computer. Most Arduino boards can be connected using a USB
   cable, but some (e.g. Arduino Pro Mini) require a USB-to-TTL serial convertor board;

 - Go to ``Tools -> Port`` and make sure USB port corresponding to your board is selected.
   In Linux, the name of the port should be similar to /dev/USB0 or /dev/USB1, etc.

 - Run ``Sketch -> Upload`` command, wait for the operation to finish. Arduino IDE should
   display "Done Uploading" on the cyan bar below the source code.

 - Connect your board to the LED strip and power supply.

 - Done.
