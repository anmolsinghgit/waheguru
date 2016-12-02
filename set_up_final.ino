// CS207 Lab Test Template 3
// Name:
// Student Number:
//Code in this file is taken and modified from lab notes and following sources:
//https://github.com/sparkfun/DangerShield/blob/master/Firmware/DangerShield/DangerShield.ino
//http://www.cs.uregina.ca/Links/class-info/207/Lab9Test/LabTestTemplate3.ino
//https://www.arduino.cc/en/Reference/EEPROMPut
//https://www.arduino.cc/en/Reference/EEPROMGet

#include <EEPROM.h>
#include <CapSense.h>


const int data = 4;
const int latch = 7;
const int clock = 8;
const int button1 = 10;
const int button2 = 11;
const int button3 = 12;
const int slider1 = A2;
const int slider2 = A1;
const int slider3 = A0;
const int buzzer = 3;
const int LDR = A3;
const int LED1 = 5;
const int LED2 = 6;

//Initialize CapSense object
CapSense   cs_9_2 = CapSense(9,2); 

struct createThree{ int entryOne; int entryTwo; int entryThree; };

void storeThreeNumbers( int entry_1, int entry_2, int entry_3 )
{
  
createThree numbersToStore = { entry_1, entry_2, entry_3 };

int locationToStore = 0;

locationToStore = locationToStore + sizeof(int);
EEPROM.put(locationToStore, numbersToStore);  
}

void fetchThreeNumbers() 
{

  int locationToStart = sizeof(int);

  createThree getThreeHere; 
  EEPROM.get( locationToStart, getThreeHere );
  
  int one = getThreeHere.entryOne;
  int two = getThreeHere.entryTwo;
  int three = getThreeHere.entryThree;
  
  Serial.println( "Three entries stored are:" );
  Serial.println( one );
  Serial.println( two ); 
  Serial.println( three );
}

void setup()
{
  //Set up output pins
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);

  //Set up button inputs
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  //Enable Serial Communication (for debugging)
  Serial.begin(9600);

  // Turn off capsense object autocalibration
  cs_9_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 
  
  int one   = 5;
  int two   = 1;
  int three = 8;
  storeThreeNumbers(one,two,three);
  fetchThreeNumbers();
}

const byte numberGlyphs[] = 
{
  B11000000, //0
  B11111001, //1
  B10100100, //2
  B10110000, //3
  B10011001, //4
  B10010010, //5
  B10000010, //6
  B11111000, //7
  B10000000, //8
  B10011000, //9
  B11111111, //10 - CLEAR
};

void loop()
{
  
}

