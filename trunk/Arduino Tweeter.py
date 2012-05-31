import twitter
import serial
import time
import re

pattern = 'Alert:'
api = twitter.Api(consumer_key='key', consumer_secret='key', access_token_key='key', access_token_secret='key');
arduino = serial.Serial('COM3', 9600)

while 1:
   status = arduino.readline()
   print  status
   tweet = re.match(pattern, status)
   if (tweet):
       print "Sending to twitter..."
       api.PostUpdate(status)
       
    
