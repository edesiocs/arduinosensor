# Introduction #

When a MiFare card is presented, our program will read in the card ID. A default profile and setting file is pre-programmed into the arduino. The system will adjust the threshold and sound the buzzer if the threshold is breached.


# Details #

Our Device uses the SL018 RFID module. In order to use it, we referenced [Marc Boon's RFID Sniffer code](http://rfid.marcboon.com/). We used his code from https://github.com/marcboon/RFIDuino and modified it so that we can read a MiFare card for it's ID number.