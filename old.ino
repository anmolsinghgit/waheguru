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
CapSense cs_9_2 = CapSense(9,2);   

void setup()
{
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  
  //Configures buttons for internal pull-up
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  digitalWrite(button1, HIGH); 
  digitalWrite(button2, HIGH);
  digitalWrite(button3, HIGH);

  //Turn off capsense object autocalibration
  cs_9_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 

  Serial.begin(9600);
}

int endSongNotes[36] = {  262,   0,  392,    0,  349,    0,  330,    0,  294,    0,  523,    0,  392,    0,  349,    0,  330,    0,  294,    0,  523,    0,  392,    0,  349,    0,  330,    0,  349,    0,  294,    0,  262,    0,  262,    0 };
int endSongTimes[36] = { 1100, 100,  500,  100,  100,  100,  100,  100,  100,  100, 1100,  100,  500,  100,  100,  100,  100,  100,  100,  100, 1100,  100,  500,  100,  100,  100,  100,  100,  100,  100, 1500,  100,  100,  100,  100,  100 };

const byte ledCharSet[11] = {
  B00111111, //0 
  B00000110, //1
  B01011011, //2
  B01001111, //3
  B01100110, //4
  B01101101, //5
  B01111101, //6
  B00000111, //7
  B01111111, //8
  B01101111, //9
  B00000000  //clear
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

void passEEPROM(int pass1, int pass2, int pass3){
  struct MyObject{
  int field1;
  int field2;
  int field3;
  };
 
  MyObject passVar = {
    pass1,
    pass2,
    pass3
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

void displayLEDs(int num){
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, ~(ledCharSet[num]));
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
  long total =  cs_9_2.capSense(30);
  int touch = map(total, 0, 1023, 0, 9); 
  return touch;
}

boolean startMode(){
  int val1 = analogRead(slider1);
  int val2 = analogRead(slider2);
  int val3 = analogRead(slider3);

  int numToDisplay1 = map(val1, 0, 1024, 0, 10); 
  int numToDisplay2 = map(val2, 0, 1023, 0, 9);
  int numToDisplay3 = map(val3, 0, 1023, 0, 9);

  static boolean success = false;
  
if(digitalRead(button1) == LOW){     
  displayLEDs(numToDisplay1);
  passEEPROM(numToDisplay1, numToDisplay2, numToDisplay3);
  
}
if(digitalRead(button2) == LOW){
  displayLEDs(numToDisplay2);
  passEEPROM(numToDisplay1, numToDisplay2, numToDisplay3);
  
}
if(digitalRead(button3) == LOW){
  displayLEDs(numToDisplay3);
  passEEPROM(numToDisplay1, numToDisplay2, numToDisplay3);
  
}

if(digitalRead(button1) == LOW && digitalRead(button2) == LOW && digitalRead(button3) == LOW){
  displayLEDs(11);

}else if(digitalRead(button1) == LOW && digitalRead(button2) == LOW){
  displayLEDs(11);

}else if(digitalRead(button2) == LOW && digitalRead(button3) == LOW){
  displayLEDs(11);

}else if(digitalRead(button1) != HIGH && digitalRead(button3) != HIGH){
  displayLEDs(11);
  
}else{
  displayLEDs(10);
}

if(numToDisplay1 == 5 && numToDisplay2 == 5 && numToDisplay3 == 5){     
 //makeTone();
 animation();
 success = true;
} 
return success;
}

void reset(){
displayLEDs(10);
}

void loop(){
reset();
if(sense() == 1){
   animation();

   while(startMode() != true){
    startMode();
   } 
}
reset();
}
