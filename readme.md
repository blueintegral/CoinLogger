CoinLogger
==========

CoinLogger is a US Quarter sized data logging platform. The current incantation has an accelerometer on it and 8 MB of Flash storage. It is based on the ATTiny85 microcontroller and also includes a USB port for retrieving the captured data. It was designed with a small size and extremely low power consumption in mind.

NanoLogger is a smaller version of CoinLogger, coming in at about the size of a dime, just over 15mm across. It doesn't have a USB port because there's no room. Instead, you need something like this: https://www.sparkfun.com/products/9235. The battery is smaller, so the battery life is substantially less, and programming and reading data off the device is done via small pads below the battery. This is best done with pogo pins, or you can solder to them. NanoLogger was designed to sacrifice some usability in exchange for an extremely small size.

Uses:
-----
* Amateur rocketry
* Tracking and alerting upon concussions in impact sports
* Characterizing your favorite rollercoaster
* Whatever else you can think of

Future Plans:
-------------
* Add versions with more/different sensors
* Make it even smaller
* Make a version that uses energy harvesting and remove the need for a battery
* Add a low power transciever?
* Move to FRAM instead of Flash, since FRAM can be lower power

Current State:
--------------
Version 1.1 of the boards has been fabbed and is being tested. Firmware is still being written. A program to pull data off the device over USB is also under development. 

Fabbed v1.1 CoinLogger:
![Assembled v1.1 front](http://i.imgur.com/WFPpeck.png)
![Assembled v1.1 back](http://i.imgur.com/N0dvxUg.png)

Fabbed v1 NanoLogger:

![Assembled NanoLogger front](http://i.imgur.com/UW9NngH.png)
![Assembled NanoLogger back](http://i.imgur.com/5LeluEu.png)

Old v1:

![Fabbed v1](http://i.imgur.com/1uSZR5D.jpg)

Breadboarded circuit:

![Breadboarded circuit](http://i.imgur.com/qOhUhsW.jpg?1)