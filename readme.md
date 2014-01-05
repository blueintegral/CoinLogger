CoinLogger
==========

CoinLogger is a US Quarter sized data logging platform. The current incantation has an accelerometer on it and 8 MB of Flash storage. It is based on the ATTiny85 microcontroller and also includes a USB port for retrieving the captured data. It was designed with a small size and extremely low power consumption in mind.

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
Version 1 of the boards has been fabbed and is being tested. Firmware is still being written. A program to pull data off the device over USB is also under development. 

Fabbed v1 boards:
![Fabbed v1](http://i.imgur.com/1uSZR5D.jpg)

Breadboarded circuit:

![Breadboarded circuit](http://i.imgur.com/qOhUhsW.jpg?1)