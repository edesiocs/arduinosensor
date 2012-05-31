/* CE 9005 Project By Team BEA
  Environment System
  
  author: Boon Tat
          Elvin Poh
          Ashraf       
  created 26th May 2012
  
*/

// SL018 demo application
// Marc Boon <http://www.marcboon.com>
// April 2010



#include <Wire.h>
#include <SL018.h>
#include <LiquidCrystal.h>
#include <Bounce.h>
#include "userProfile.h"
#include "pitches.h"
#include <FlexiTimer2.h>
#include <interrupt.h>
// TAG pin (low level when tag present)
#define TAG 21 // A3

// Actions
#define NONE 0
#define SEEK 1
#define READ 2
#define WRITE 3

/********************************** Buzzer ****************************************/
/*   Melody
  
  Plays a melody 
 
  circuit:
  * 8-ohm speaker on digital pin 8
  
  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe 

This example code is in the public domain.
  
  http://arduino.cc/en/Tutorial/Tone
  
*/
// notes in the melody:
 int melody[] = {
   NOTE_C2, NOTE_B2,NOTE_A2, NOTE_C2, NOTE_B2,NOTE_A2, NOTE_C2, NOTE_B2};
 
// note durations: 4 = quarter note, 8 = eighth note, etc.:
 int noteDurations[] = {
   4, 4, 4, 4,4,4,4,4 };

/****************************************** RFID ************************************************/
SL018 rfid;

// Global vars
byte action = NONE;
boolean autoRead = true;
boolean tagPresent = false;
boolean authenticated;
byte block;
byte numBlocks;
byte tagType;
char msg[16];

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(0, 1, 2, 3, 4, 7);
int valMeth = 0;
int valLpg = 0;
int valCo = 0;
int valLight = 0;

int valTemp = 0;
float tempCel = 0;
int tempOffset = 96;
float oneDegreeChange = 2.05;


const int buzzerPin = 12;
const int lightSensor = 17;
const int tempSensor = 16;
const int lpgSensor = 20;
const int methSensor = 19;
const int coSensor = 18;

/********************************* Push Button **************************************/
const int buttonPin1 = 8;
const int buttonPin2 = 9;
const int buttonPin3 = 14;
const int buttonPin4 = 13;

Bounce pushbutton1 = Bounce(buttonPin1, 10); // 10 ms debounce   //Stop Pause
Bounce pushbutton2 = Bounce(buttonPin2, 10); // 10 ms debounce   //Previous
Bounce pushbutton3 = Bounce(buttonPin3, 10); // 10 ms debounce   //Next
Bounce pushbutton4 = Bounce(buttonPin4, 10); // 10 ms debounce   //Serial

/***************************** RFID Card ID's ****************************************/
int profileID = 0;
String currentID = "Default";
/////////
/***************************** Global Mode Menu ***********************************/
int menuNo=0;  //0=splash
int menuMax=6; //Max no of menu + 1
boolean menu=true;
boolean paused=false;
boolean serialEnabled=true;
/////////
void setup()
{
  FlexiTimer2::set(1000, timerInterrupt); // MsTimer2 style is also supported
  FlexiTimer2::start();
  pinMode(TAG, INPUT);
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Serial comm started!");
  lcd.begin(16, 2);
  lcd.print("CE 9005 Project");
  lcd.setCursor(0, 1); 
  lcd.print("Team BEA");
  initButton();
  delay(1000);
}
////////////////////////////////////////////////
void initButton(){
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
}
void timerInterrupt(){
  readSensors();
  if (!paused){
    menuNo=menuNo+1;
    menuNo=menuNo%menuMax; 
  }
  if (serialEnabled){
    printSerial();
  }
  dispMenu();
}
////////////////////////////////////////////////
void loop()
{
  cardCheck();
  buttonPress();
  checkThreshold();
}
void readSensors(){
  valLpg = analogRead(lpgSensor);
  valMeth = analogRead(methSensor);
  valCo = analogRead(coSensor);
  valLight = analogRead(lightSensor);
  valTemp = analogRead(tempSensor);
  adcToTemp();
}
void printSerial(){
  Serial.println("");
  Serial.print("Value of LPG: ");Serial.println(valLpg);
  Serial.print("Value of Methane: ");Serial.println(valMeth);
  Serial.print("Value of CO: ");Serial.println(valCo);
  Serial.print("Value of Light: ");Serial.println(valLight);
  Serial.print("Value of Temp in Celcius: ");Serial.println(tempCel); 
  Serial.print("Profile: ");Serial.println(currentProfile);
  Serial.print("Light Profile: ");Serial.println(profLight);

}
/////////////////////////////////
void buzzer(){
    // no need to repeat the melody.   
   for (int thisNote = 0; thisNote < 8; thisNote++) {
 
    // to calculate the note duration, take one second 
    // divided by the note type.
     //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
     int noteDuration = 1000/noteDurations[thisNote];
     tone(12, melody[thisNote],noteDuration);
 
    // to distinguish the notes, set a minimum time between them.
     // the note's duration + 30% seems to work well:
     int pauseBetweenNotes = noteDuration * 1.30;
     delay(pauseBetweenNotes);
     // stop the tone playing:
     noTone(12);
   }
}
void dispMenu(){
  switch (menuNo)
  {
    case 0:
      dispMenu0();
    break;
    case 1:
      dispMenu1();
    break;
    case 2:
      dispMenu2();
    break;
    case 3:
      dispMenu3();
    break;
    case 4:
      dispMenu4();
    break;
    case 5:
      dispMenu5();
    break;
  }
}
/////////////////////////////////
////////////////////////////////
void dispMenu0(){
  clearLCD();  
  lcd.setCursor(0, 0); // top left
  lcd.print("Profile:");
  lcd.setCursor(0, 1); 
  lcd.print(currentProfile);
}
void dispMenu1(){
  clearLCD();
  lcd.setCursor(0, 0); // top left
  lcd.print("1)Temperature");
  lcd.setCursor(0, 1); 
  lcd.print("Value (C): ");lcd.print(tempCel);
}
void dispMenu2(){
  clearLCD();
  lcd.setCursor(0, 0); // top left
  lcd.print("2)Methane");
  lcd.setCursor(0, 1); 
  lcd.print("Value: ");lcd.print(valMeth);
}
void dispMenu3(){
  clearLCD();
  lcd.setCursor(0, 0); // top left
  lcd.print("3)LPG");
  lcd.setCursor(0, 1); 
  lcd.print("Value: ");lcd.print(valLpg);  
}
void dispMenu4(){
  clearLCD();
  lcd.setCursor(0, 0); // top left
  lcd.print("4)C0");
  lcd.setCursor(0, 1); 
  lcd.print("Value: ");lcd.print(valCo);
}
void dispMenu5(){
  clearLCD();  
  lcd.setCursor(0, 0); // top left
  lcd.print("5)Light");
  lcd.setCursor(0, 1); 
  lcd.print("Value: ");lcd.print(valLight);
}

void clearLCD(){
  lcd.clear();
}
///////////////////////////////
void cardCheck()
{
  
  // check for tag presence when auto read is enabled
  if(autoRead && !tagPresent && !digitalRead(TAG) && action == NONE)
  {
    tagPresent = true;
    // read tag
    action = READ;
    // specify what to read
    block = 0;
    numBlocks = 16;
    // tag has to be selected first
    rfid.selectTag();
  }

  // check if tag has gone
  if(tagPresent && digitalRead(TAG))
  {
    tagPresent = false;
  }
  
  // check for command from serial port

   rfid.seekTag();
   action = SEEK;

  // check for response from rfid
  if(rfid.available())
  {
    // check for errors
    if(rfid.getErrorCode() != SL018::OK && rfid.getErrorCode() != SL018::LOGIN_OK)
    {
      if(action != SEEK) // ignore errors while in SEEK mode
      {
        Serial.println(rfid.getErrorMessage());
        rfid.haltTag();
        action = NONE;
      }
    }
    else // deal with response if no error
    {
      switch(rfid.getCommand())
      {
      case SL018::CMD_SEEK:
      case SL018::CMD_SELECT:
        // store tag type
        tagType = rfid.getTagType();
        // show tag name and serial number
/*        Serial.print(rfid.getTagName());    //Commented to remove mifare output
        Serial.print(' ');
        Serial.println(rfid.getTagString());*/
        currentID = rfid.getTagString();
        loadProfile();
        break;  
      }
    } 
  }    
}

void checkBtn1(){
  if (pushbutton1.update()) {
    if (pushbutton1.fallingEdge()) {
      if (paused){
        Serial.println("Btn1 Resume");
        paused=false;
      }
      else{
        Serial.println("Btn1 Paus");
        paused=true;
      }
    }
  }
}
void checkBtn2(){
  if (pushbutton2.update()) {
    if (pushbutton2.fallingEdge()) {
      menuNo=menuNo+menuMax-1;
      menuNo=menuNo%menuMax;
      Serial.println(menuNo);
    }
  }
}
void checkBtn3(){
  if (pushbutton3.update()) {
    if (pushbutton3.fallingEdge()) {
      menuNo=menuNo+1;
      menuNo=menuNo%menuMax;    //Btn ToDo
      Serial.println(menuNo);
    }
  }
}
void checkBtn4(){
  if (pushbutton4.update()) {
    if (pushbutton4.fallingEdge()) {
      if (serialEnabled){
        Serial.println("Serial Falsed");
        serialEnabled=false;
      }
      else{
        Serial.println("Serial True");
        serialEnabled=true;
      }
    }
  }
}
void buttonPress(){
  checkBtn1();
  checkBtn2();
  checkBtn3();
  checkBtn4();
}

void checkThreshold(){
  if (valLight<profLight)
  { 
    Serial.print("Alert: Light is below threshold: ");Serial.println(valLight);
    buzzer();
  }
  
  if (tempCel>profTemp)
  { 
    Serial.print("Alert: Temp is above threshold: ");Serial.println(tempCel);
    buzzer();
  }  
  if (valLpg>profLpg)
  { 
    Serial.print("Alert: LPG is above threshold: ");Serial.println(valLpg);
    buzzer();
  }
  if (valMeth>profMeth)
  { 
    Serial.print("Alert: Methane is above threshold: ");Serial.println(valMeth);
    buzzer();
  }
  if (valCo>profCo)
  { 
    Serial.print("Alert: Carbon Monoxide is above threshold: ");Serial.println(valCo);
    buzzer();
  }
        
}
void loadProfile(){
  if (currentID == ashID){
    currentProfile="Ashraf";
    profLight=ashLight;
    profTemp=ashTemp;
    profCo=ashCo;
    profMeth=ashMeth;
    profLpg=ashLpg;
  }
  else if (currentID == elvID){
    currentProfile="Elvin";
    profLight=elvLight;
    profTemp=elvTemp;
    profCo=elvCo;
    profMeth=elvMeth;
    profLpg=elvLpg;
  }
  else if (currentID == kopID){
    currentProfile="Koptiam";
    profLight=kopTempight;
    profTemp=kopTemp;
    profCo=kopCo;
    profMeth=kopMeth;
    profLpg=kopLpg;
  }
  else if (currentID == tagID1){
    currentProfile="Tag1";
    profLight=id1Light;
    profTemp=id1Temp;
    profCo=id1Co;
    profMeth=id1Meth;
    profLpg=id1Lpg;
  }
  else if (currentID == tagID2){
    currentProfile="Tag2";
    profLight=id2Light;
    profTemp=id2Temp;
    profCo=id2Co;
    profMeth=id2Meth;
    profLpg=id2Lpg;
  }
  else if (currentID == tagID3){
    currentProfile="Tag3";
    profLight=id3Light;
    profTemp=id3Temp;
    profCo=id3Co;
    profMeth=id3Meth;
    profLpg=id3Lpg;
  }
  else{
    currentProfile="Not Registered";
    profLight=defaultLight;
    profTemp=defaultTemp;
    profCo=defaultCo;
    profMeth=defaultMeth;
    profLpg=defaultLpg;
  }
}  

void adcToTemp(){
  
  tempCel = (valTemp/oneDegreeChange)-tempOffset;
}


