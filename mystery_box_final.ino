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

void setup()
{
  //Set up output pins
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);

  //Set up LED pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  /*Configures buttons for internal pull-up
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  digitalWrite(button1, HIGH); 
  digitalWrite(button2, HIGH);
  digitalWrite(button3, HIGH);*/

  //Set up button inputs
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  
  //Enable Serial Communication (for debugging)
  Serial.begin(9600);

  // Turn off capsense object autocalibration
  cs_9_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 
}

/*int endSongNotes[36] = {  262,   0,  392,    0,  349,    0,  330,    0,  294,    0,  523,    0,  392,    0,  349,    0,  330,    0,  294,    0,  523,    0,  392,    0,  349,    0,  330,    0,  349,    0,  294,    0,  262,    0,  262,    0 };
int endSongTimes[36] = { 1100, 100,  500,  100,  100,  100,  100,  100,  100,  100, 1100,  100,  500,  100,  100,  100,  100,  100,  100,  100, 1100,  100,  500,  100,  100,  100,  100,  100,  100,  100, 1500,  100,  100,  100,  100,  100 };
*/

const byte animationForStar[] = 
{
  B01111111,
  B10001001,
};

const byte animationForCircle[] = 
{
  B11111011,
  B11101111,
  B11011111,
  B11111110,
  B11111101,
  B11111011,
};

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

struct createThree{ int entryOne; int entryTwo; int entryThree; };

void guessReset()
{
sevenSegment(10);
keyEntryMode(0,0,0);
/*digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);*/
}

int sense()
{
  //touch
  long total =  cs_9_2.capSense(30);
  int touch = map(total, 0, 1023, 0, 9); 
  return touch;

  /*light
  lightSensorReading = analogRead(LDR);
  Serial.println("A shadow just passed over the light sensor");
  int light = map(lightSensorReading, 0, 1023, 0, 9);
  
  if (lightSensorReading > 0) 
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  } 
  else 
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  return light;*/
}

void sevenSegment(int arrayNumber)
{
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, (numberGlyphs[arrayNumber]) );
  digitalWrite(latch, HIGH);
}

int animation()
{
 int delayTime = 123;
  
 for ( int x = 0 ; x < 4 ; x++ )
 {
  for ( int y = 0 ; y < 2 ; y++ )
  {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, (animationForStar[y]));
    digitalWrite(latch, HIGH);
    delay(delayTime);
  }
 }
  
 for (int x = 0; x < 4; x++)
 {
  for (int y = 0; y < 6; y++)
  {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, (animationForCircle[y]));
    digitalWrite(latch, HIGH);
    delay(delayTime);
  }
 }
}

/*void makeTone()
{
 for (int i = 0; i < 36; i++)
 {
    if (endSongNotes[i] != 0)
    {
      tone(3, endSongNotes[i]);
    }
    else
    {
      noTone(3);
    }
      delay(endSongTimes[i]);
  }
}
*/

void keyEntryMode( int entry_1, int entry_2, int entry_3 )
{
  int locationToStore = 0;
  int locationToStart = sizeof(int);
  
  createThree numbersToStore = { entry_1, entry_2, entry_3 };
  
  locationToStore = locationToStore + sizeof(int);
  EEPROM.put(locationToStore, numbersToStore);

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

int codeEntryMode()
{
  static int prevState = HIGH;
  static int currState = HIGH;
  static int keyFound = 0;
  int readingOne = analogRead(slider1);
  int sliderNumberOne = map(readingOne, 0, 1023, 0, 9); 
  
  int readingTwo = analogRead(slider2);
  int sliderNumberTwo = map(readingTwo, 0, 1023, 0, 9);
  
  int readingThree = analogRead(slider3);
  int sliderNumberThree = map(readingThree, 0, 1023, 0, 9);
    
  if ( digitalRead(button1) == LOW )
  {     
  sevenSegment(sliderNumberOne);
  codeEntryMode(sliderNumberOne, sliderNumberTwo, sliderNumberThree);
  }
  
  else if ( digitalRead(button2) == LOW )
  {
  sevenSegment(sliderNumberTwo);
  codeEntryMode(sliderNumberOne, sliderNumberTwo, sliderNumberThree);
  }
  
  else if ( digitalRead(button3) == LOW )
  {
  sevenSegment(sliderNumberThree);
  codeEntryMode(sliderNumberOne, sliderNumberTwo, sliderNumberThree);
  }

  if (digitalRead(button1) == LOW && digitalRead(button2) == LOW && digitalRead(button3) == LOW)
  {
  sevenSegment(10);
  }
  
  if (digitalRead(button1) == LOW && digitalRead(button2) == LOW)
  {
  sevenSegment(10);
  }
  
  if(digitalRead(button2) == LOW && digitalRead(button3) == LOW)
  {
  sevenSegment(10);
  }
  
  if(digitalRead(button1) == LOW && digitalRead(button3) == LOW)
  {
  sevenSegment(10);
  }
  else
  {
  sevenSegment(10);
  }

  /*if ( currState == LOW && prevState == HIGH)
    {
    
    }   
  */

  if(sliderNumberOne == 5 && sliderNumberTwo == 1 && sliderNumberThree == 8)
  {     
  //makeTone();
  animation();
  keyFound = 1;
  } 
  return keyFound;
}

/*void buttonState()
{
}
*/

void loop()
{
guessReset();

  int touchDetected = sense();
  int keyCheck = codeEntryMode();

  if( touchDetected == 1 )
  {
   animation();
   /*digitalWrite(LED1, HIGH);
     digitalWrite(LED2, HIGH);*/
   while( keyCheck == 0 )
   {
    codeEntryMode();
   } 
  }
  
guessReset();
}
