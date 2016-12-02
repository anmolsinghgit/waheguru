// CS207 Lab Test Template 3
// Name: 
// Student Number: 

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
  
  /*//Configures buttons for internal pull-up
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
byte numberGlyphs[] = 
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

void makeTone(){
 for (int i = 0; i < 36; i++){
    if (endSongNotes[i] != 0){
      tone(3, endSongNotes[i]);
    }else{
      noTone(3);
    }
      delay(endSongTimes[i]);
  }
}

void codeEntryMode(int entry_1, int entry_2, int entry_3){
  struct MyObject{
  int field1;
  int field2;
  int field3;
  };
 
  MyObject passVar = {
    entry_1,
    entry_2,
    entry_3
  };
  
  int eAddress = 0;
  eAddress += sizeof(int);
  EEPROM.put( eAddress, passVar );

  int eeAddress = sizeof(int); 
  MyObject storeVar; 
  EEPROM.get( eeAddress, storeVar );

  Serial.println( "Read custom object from EEPROM: " );
  Serial.println( storeVar.field1 );
  Serial.println( storeVar.field2 ); 
  Serial.println( storeVar.field3 );
}

void sevenSegment(int arrayNumber){
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, (numberGlyphs[arrayNumber]));
  digitalWrite(latch, HIGH);
}

const byte aniCircle[6] = {
  B00001000,
  B00010000,
  B00100000,
  B00000001,
  B00000010,
  B00000100, 
};

const byte aniStar[2] = {
  B10000000,
  B01110110,
};

int animation(){
  for (int j = 0; j < 5; j++){
       for (int i = 0; i < 2; i++){
       digitalWrite(latch, LOW);
       shiftOut(data, clock, MSBFIRST, ~(aniStar[i]));
       digitalWrite(latch, HIGH);
       delay(151);
       }
  }
  
  for (int j = 0; j < 5; j++){
       for (int i = 0; i < 6; i++){
       digitalWrite(latch, LOW);
       shiftOut(data, clock, MSBFIRST, ~(aniCircle[i]));
       digitalWrite(latch, HIGH);
       delay(151);
       }
  }
}

int sense(){
  //touch
  long total =  cs_9_2.capSense(30);
  int touch = map(total, 0, 1023, 0, 9); 
  return touch;

  //light
  //temperature
  
}

boolean startMode(){
  int val1 = analogRead(slider1);
  int val2 = analogRead(slider2);
  int val3 = analogRead(slider3);

  int sliderNumber1 = map(val1, 0, 1024, 0, 10); 
  int sliderNumber2 = map(val2, 0, 1023, 0, 9);
  int sliderNumber3 = map(val3, 0, 1023, 0, 9);

  static boolean success = false;
  
if(digitalRead(button1) == LOW){     
  sevenSegment(numToDisplay1);
  codeEntryMode(sliderNumber1, sliderNumber2, sliderNumber3);
  
}
if(digitalRead(button2) == LOW){
  sevenSegment(numToDisplay2);
  codeEntryMode(sliderNumber1, sliderNumber2, sliderNumber3);
}

if(digitalRead(button3) == LOW){
  sevenSegment(numToDisplay3);
  codeEntryMode(sliderNumber1, sliderNumber2, sliderNumber3);
}

if(digitalRead(button1) == LOW && digitalRead(button2) == LOW && digitalRead(button3) == LOW){
  sevenSegment(10);

}else if(digitalRead(button1) == LOW && digitalRead(button2) == LOW){
  sevenSegment(10);

}else if(digitalRead(button2) == LOW && digitalRead(button3) == LOW){
  sevenSegment(10);

}else if(digitalRead(button1) != HIGH && digitalRead(button3) != HIGH){
  sevenSegment(10);
  
}else{
  sevenSegment(10);
}

if(numToDisplay1 == 5 && numToDisplay2 == 5 && numToDisplay3 == 5){     
 //makeTone();
 animation();
 success = true;
} 
return success;
}

void guessReset(){
displayLEDs(10);
}

void loop(){
guess();
if(sense() == 1){
   animation();

   while(startMode() != true){
    startMode();
   } 
}
guessReset();
}
