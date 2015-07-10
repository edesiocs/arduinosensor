# Introduction #

This is a guide on how to calibrate the Temperature Sensor.

# Details #

In our project, we have 5 sensors in all and they are all connected to the ADC of the Teensy. What's an ADC? Read more [here](http://arduino.cc/playground/CourseWare/AnalogInput).

The cliff notes is that the arduino reads in voltage value from 0v to 5v and it relates this values to a number between 0 to 1023. 0v being 0, 5v being 1023 and everything inbetween is scaled appropriately.

Why the magic number of 1023? The answer: It has only a 10 bit resolution for the ADC. 2^10 = 1024.

For this entry, we will try to calibrate the Temperature Sensor. The data sheet is located [here](http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Temp/TMP35_36_37.pdf). Our Temperature Sensor is the TMP36 so we shall look at the appropriate section. To calibrate, we take a look at the "Output voltage vs Temperature" Graph.

![http://3.bp.blogspot.com/-QAuK7-KHWlY/T8b7i51P4-I/AAAAAAAAASE/4whidkqjT1M/s1600/Graph.png](http://3.bp.blogspot.com/-QAuK7-KHWlY/T8b7i51P4-I/AAAAAAAAASE/4whidkqjT1M/s1600/Graph.png)

![http://1.bp.blogspot.com/-9Rn1x6jJvzQ/T8b8Bi2kcqI/AAAAAAAAASM/clzH3QEQ2wg/s1600/Graph.png](http://1.bp.blogspot.com/-9Rn1x6jJvzQ/T8b8Bi2kcqI/AAAAAAAAASM/clzH3QEQ2wg/s1600/Graph.png)

We can see that line "b" is the sensor we are using and that it is linear. At 0 degrees celcius, it's roughly at 0.5v and at 100 degrees celcius it's at about 1.5v.

By calculating, 0 degrees should give an ADC value of about 102 and a 100 degrees should give 307.

Assuming there's some error of + or - 0.1 volt, the readings can go from 82 to 123 for 0 degrees and 287 to 327 for 100 degrees.

We then did a physical test by putting the sensor near a kettle of boiling water (aka 100 degrees celcius), the reading should an ADC value of 320. When we put the sensor near an ice cube (aka 0 degrees celcius) through a plastic bag, it read 226. The Ice value was expected to be between 82 to 123 but it was so much higher. This could be due to the insulation of the plastic bag preventing an accurate reading.

With this, we know it's linear and that 100 degrees is 320, we can then calculate 0 degrees and hence calibrate it.

Boiling water = 320/1023 = 1.56 volts

Therefore Ice = 1023/5 `*` (1.56 - 1) = 115 ADC value

This means that 320-115 = 205 ADC values to go from 0 to 100.

1 degree change = 205/100 = 2.05 ADC

Next:-

100 degrees celcius to readings = 320 ADC /2.05 ADC = 156 in temperature
0 degrees celcius to readings = 115 ADC /2.05 ADC = 56 in temperature

We can see it is off by 56, therefore the offset is set to 56. During physical testing, we found that the offset is actually 96 through trial and error.

We can now calibrate it in this method:-
```
valTemp = X //taken from ADC
tempOffset = 96
oneDegreeChange = 2.05 // Per ADC

tempCel = (valTemp/oneDegreeChange)-tempOffset;
```