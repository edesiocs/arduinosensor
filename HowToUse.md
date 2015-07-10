# Introduction #

Once assembled according to the schematic given and flashed the code into the teensy, it's ready to be used! Here's a guide on how it works.


# Details #

In order to use it correctly, first plug the device to the computer. Next run the Python script to allow checking on the device for any alerts that need to be sent to its twitter account.

![http://3.bp.blogspot.com/-cFtIudohGxQ/T8eDRxhS6ZI/AAAAAAAAAS4/QNQUUom1spg/s1600/2012-05-31+14.39.31.jpg](http://3.bp.blogspot.com/-cFtIudohGxQ/T8eDRxhS6ZI/AAAAAAAAAS4/QNQUUom1spg/s1600/2012-05-31+14.39.31.jpg)

Once the device is turned on, it will show a welcome logo. After 1 second, it will cycle through 6 screens at 1 second interval. There are 4 buttons. The first button will pause the screen. Pressing button 2 and 3 would then allow you to physically cycle through the windows. Pressing button 4 will disable or enable serial output.

![http://4.bp.blogspot.com/-YuAllEvnAPg/T8eDP_pJvhI/AAAAAAAAASw/z3tDoFckqYs/s1600/2012-05-31+22.36.49.jpg](http://4.bp.blogspot.com/-YuAllEvnAPg/T8eDP_pJvhI/AAAAAAAAASw/z3tDoFckqYs/s1600/2012-05-31+22.36.49.jpg)

If a RFID card is presented to the device, the profile set will change to the RFID card's profile. If an unknown card is used, a default profile is loaded instead.

![http://2.bp.blogspot.com/-UA-WKPjqst4/T8eDMwHxB9I/AAAAAAAAASg/UNv-sfLHOBE/s1600/2012-05-31+22.36.15.jpg](http://2.bp.blogspot.com/-UA-WKPjqst4/T8eDMwHxB9I/AAAAAAAAASg/UNv-sfLHOBE/s1600/2012-05-31+22.36.15.jpg)

![http://3.bp.blogspot.com/-owVEY_huuMk/T8eDONh4M2I/AAAAAAAAASo/XvCpv6c9B6Q/s1600/2012-05-31+22.36.29.jpg](http://3.bp.blogspot.com/-owVEY_huuMk/T8eDONh4M2I/AAAAAAAAASo/XvCpv6c9B6Q/s1600/2012-05-31+22.36.29.jpg)


While reading the values of the various sensors, if the threshold value of a sensor is breached, the device will beep and send out an alert message out on the serial communication port. The reading and updating of the serial is interrupt driven and the system will continue to run. The python script is constantly listening on the serial communication port and if it detects "Alert" in the text, it will post it to the twitter account assigned to it.