/*
 *    PURPOSE: prototype TSL235R monitoring

   Digital Pin layout ARDUINO
   =============================
    2     IRQ 0    - to TSL235R

   PIN 1 - GND-
   PIN 2 - VDD - 5V
   PIN 3 - SIGNAL

*/

//#include <Keyboard.h>

volatile unsigned long cnt = 0;
volatile unsigned long cnt2 = 0;
volatile unsigned long cnt1 = 0;
unsigned long oldcnt = 0;
unsigned long oldcnt2 = 0;
unsigned long t = 0;
unsigned long t2 = 0;
unsigned long last;
unsigned long last2;
int redAvg = 0;

int onLed = 0;
int ledCounter = 1;

char Str1[1];
//
const int numReadings = 7;//3 for blue
//const int interval = 300*32;//31*32;//31
int baseIntervalVal = 31*32;
int interval1 = 31*32;
int interval2 = 31*32;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;    // the average
int prevHz = 0;


int sensVal;           // for raw sensor values
float filterVal = 0.021;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
float smoothedVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing
float outputVal;
bool prevButtVal;

int numLeds = 16;
int numMaxPinsPerLed = 4;

//<datatype> array [DIM_0_SIZE] [DIM_1_SIZE] = {
//  //as many vals as dim1
// {val,val,val},
// {val,val,val}//as many rows as dim0
//};

uint8_t ledPinArray [16][4] = {
  //as many vals as dim1
 {13, 12, 11, 11},
 {10, 9, 8, 8},
 {7, 6, 5, 5},
 {4, 3, 2, 2},
 {54, 55, 14, 14},
 {15, 17, 19, 19},
 {22, 23, 24, 24},
 {25, 26, 27, 27},
 {28, 29, 30, 30},
 {31, 32, 33, 33},
 {34, 35, 36, 36},
 {37, 38, 39, 39},
 {40, 41, 42, 42},
 {43, 44, 45, 46},
 {47, 48, 49, 50},
 {51, 52, 53, 53}
};


int pinIndexNum = 0;
int strengthIndexNum = 0;


//int currentPinNum = 2;
//int ledPinStrength = 0;

int sensorPinVal1 = 20;
int sensorPinVal2 = 18;

void irq1()
{
  cnt++;
}

void irq2()
{
  cnt2++;
}

///////////////////////////////////////////////////////////////////
//
// SETUP
//
void setup()
{


  

//
//
//TCCR0B = TCCR0B & 0b11111000 | 0x01;
//TCCR1B = TCCR1B & 0b11111000 | 0x01;
//TCCR2B = TCCR2B & 0b11111000 | 0x01;
//TCCR3B = TCCR3B & 0b11111000 | 0x01;
//TCCR4B = TCCR4B & 0b11111000 | 0x01;
//TCCR5B = TCCR5B & 0b11111000 | 0x01;

//bit shifting

//low latch//vs high latch

//try analog read on the jitter line

  
  Serial.begin(9600);
  Serial.println("START");
  
  pinMode(sensorPinVal1, INPUT);
  pinMode(sensorPinVal2, INPUT);
digitalWrite(sensorPinVal2, HIGH); 
digitalWrite(sensorPinVal1, HIGH); 

//  digitalWrite(sensorPinVal2, HIGH);
//
//  pinMode(A4, INPUT);
//  pinMode(A5, INPUT);
//  pinMode(A6, INPUT);
//  pinMode(A7, INPUT);

  attachInterrupt(digitalPinToInterrupt(sensorPinVal1), irq1, RISING);
  attachInterrupt(digitalPinToInterrupt(sensorPinVal2), irq2, RISING);


pinMode(A2, OUTPUT);
digitalWrite(A2, HIGH);

pinMode(A3, OUTPUT);
digitalWrite(A3, HIGH);
//setupPins();
//delay(1000);
//startAllLeds();


//pinMode(A0, OUTPUT);
//digitalWrite(A0, HIGH);
//
// pinMode(15, OUTPUT);
// digitalWrite(15, HIGH);
//
// pinMode(22, OUTPUT);
// digitalWrite(22, HIGH);

// pinMode(25, OUTPUT);
// digitalWrite(25, HIGH);
// 
//pinMode(28, OUTPUT);
//digitalWrite(28, HIGH);
//pinMode(37, OUTPUT);
//digitalWrite(37, HIGH);

//  for(int k = 0; k < sizeof(ledPinArray) -1; k++){
//    int pinVal = ledPinArray[k];
//    
//    pinMode(pinVal, OUTPUT);
//    digitalWrite(pinVal, HIGH);
//    analogWrite(pinVal, 100);
//    Serial.print(pinVal);
//    Serial.print("\n");
//  } 



}

void darkenAllPins(){

     for (int i = 0; i < numLeds; i++) {

        for (int j = 0; j < numMaxPinsPerLed; j++) {
          uint8_t thisPinNum = ledPinArray[i][j];
          pinMode(thisPinNum, INPUT_PULLUP);
          digitalWrite(thisPinNum, LOW);
        }
     }
     
}
void setupPins(){
   for (int i = 0; i < numLeds; i++) {

        for (int j = 0; j < numMaxPinsPerLed; j++) {

            uint8_t thisPinNum = ledPinArray[i][j];
              pinMode(thisPinNum, OUTPUT);
              digitalWrite(thisPinNum, LOW);
         }
   
   }
}

void startAllLeds(){

     for (int i = 0; i < numLeds; i++) {
            int j = 0;
            uint8_t thisPinNum = ledPinArray[i][j];
            pinMode(thisPinNum, OUTPUT);
            digitalWrite(thisPinNum, HIGH);            
      }
   
}
void goToLed(int i){
  interval1 = baseIntervalVal;
  interval2 = baseIntervalVal;
  darkenAllPins();
  pinIndexNum = i;
  strengthIndexNum = 0;
  int thisPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
  pinMode(thisPinNum, OUTPUT);
  digitalWrite(thisPinNum, HIGH);
}

void increaseLedStrength(){

  int oldPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
  pinMode(oldPinNum, INPUT_PULLUP);
  digitalWrite(oldPinNum, LOW);
  
  strengthIndexNum += 1;
  if(strengthIndexNum >= numMaxPinsPerLed){
    strengthIndexNum = numMaxPinsPerLed - 1;
  }


  int thisPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
  pinMode(thisPinNum, OUTPUT);
  digitalWrite(thisPinNum, HIGH);
}

void decreaseLedStrength(){

  int oldPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
  pinMode(oldPinNum, INPUT_PULLUP);
  digitalWrite(oldPinNum, LOW);
  
  strengthIndexNum -= 1;
  if(strengthIndexNum < 0){
    strengthIndexNum = 0;
  }

  int thisPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
  pinMode(thisPinNum, OUTPUT);
  digitalWrite(thisPinNum, HIGH);
}

int minIntervalSize = 40*32;
int maxIntervalSize = 256*32;


void increaseInterval(int sensor, int level){

  if(sensor == 1){
    int theNum = minIntervalSize;
    if(level == 1){
      theNum = minIntervalSize;
    }
    if(level == 2){
      theNum = 50*32;
    }
    if(level == 3){
      theNum = 100*32;
    }
    if(level == 4){
      theNum = 164*32;
    }
    if(level == 5){
      theNum = maxIntervalSize;
    }
    interval1 = theNum;
  }

  if(sensor == 2){
    int theNum = minIntervalSize;
    if(level == 1){
      theNum = minIntervalSize;
    }
    if(level == 2){
      theNum = 50*32;
    }
    if(level == 3){
      theNum = 100*32;
    }
    if(level == 4){
      theNum = 164*32;
    }
    if(level == 5){
      theNum = maxIntervalSize;
    }
    interval2 = theNum;
  }
  
}

///////////////////////////////////////////////////////////////////
//
// MAIN LOOP
//
void loop()
{

//change the leds
if(millis() - last >= 24){
  
   String val = "";
   if (Serial.available()) { // If data is available to read,
     val = Serial.readString(); // read it and store it in val
     if(val.startsWith("DID_PRESS_LED") == true){
        String numString = getValue(val, '+', 1);
        int thisNum = numString.toInt();
        goToLed(thisNum);
      }
      if(val == "inc"){
        increaseLedStrength();
      }
      if(val == "dec"){
        decreaseLedStrength();
      }

      if(val.startsWith("sensor") == true){
        String sensorString = getValue(val, '|', 0);
        String sensorScrape = getValue(sensorString, ':', 1);
        int sensorInt = sensorScrape.toInt();

        String levelString = getValue(val, '|', 1);
        String levelScrape = getValue(levelString, ':', 1);
        int levelInt = levelScrape.toInt();

        increaseInterval(sensorInt, levelInt);
      }
      

//      if(val == "incInt1"){
//        increaseInterval1Size();
//      }
//      if(val == "decInt1"){
//        decreaseInterval1Size();
//      }
//      if(val == "incInt2"){
//        increaseInterval2Size();
//      }
//      if(val == "decInt2"){
//        decreaseInterval2Size();
//      }

     // Serial.flush();

      }
  }
  

//check for sensor 1
  if (millis() - last >= interval1/32)//31 for blue
  {
 
    last = millis();
    t = cnt;
    unsigned long hz = t - oldcnt;
    oldcnt = t;
//
//    t2 = cnt2;
//    unsigned long hz2 = t2 - oldcnt2;
//    oldcnt2 = t2;

    int thisPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
    int num = int(hz);
//    int num2 = int(hz2);
    Serial.print("Pin Num:");
    Serial.print(String(thisPinNum));
    Serial.print("\n");
    Serial.print("1:");
    Serial.print(num);
    Serial.print("\n");
    Serial.print("IntervalOne:");
    Serial.print(interval1);
    Serial.print("\n");
//    Serial.print("2:");
//    Serial.print(num2);
//    Serial.print("\n");
}

//check for sensor 2
  if(millis() - last2 >= interval2/32){
    last2 = millis();

    t2 = cnt2;
    unsigned long hz2 = t2 - oldcnt2;
    oldcnt2 = t2;

    int num2 = int(hz2);
//    if(num2 < 0){
//      num2 = 0;
//    }
    Serial.print("\n");
    Serial.print("2:");
    Serial.print(num2);
    Serial.print("\n");
    Serial.print("IntervalTwo:");
    Serial.print(interval2);
    Serial.print("\n");
  }


  //check the GSRs
  
//int grs1 = analogRead(A7);
//int grs2= analogRead(A4);
//int grs3 = analogRead(A5);
//int grs4 = analogRead(A6);
//
//    Serial.print("\n");
//    Serial.print("GSRA:");
//    Serial.print(grs1);
//    Serial.print("\n");
//        Serial.print("GSRB:");
//    Serial.print(grs2);
//        Serial.print("\n");
//    Serial.print("GSRC:");
//    Serial.print(grs3);
//    Serial.print("\n");
//        Serial.print("GSRD:");
//    Serial.print(grs4);
//    Serial.print("\n");
}


String getLedNameForPin(int pinNum){
  //pinIndexNum
  String retString = "";
  if(pinIndexNum == 0){
    
  }
  if(pinIndexNum == 1){
    retString = "PURPLE_TOP";
  }
  if(pinIndexNum == 2){
    retString = "GREEN_TOP";
  }
  if(pinIndexNum == 3){
    retString = "BLUE_TOP";//blue top
  }
  if(pinIndexNum == 4){
    retString = "IR_TOP";//green top
  }
  if(pinIndexNum == 5){
    retString = "RED_TOP";
  }
  if(pinIndexNum == 6){
    retString = "AQUA_TOP";
  }
  if(pinIndexNum == 7){
    retString = "YELLOW_TOP";
  }
  if(pinIndexNum == 8){
    retString = "UV_TOP";
  }
  if(pinIndexNum == 9){
    retString = "UV_BOTTOM";
  }
  if(pinIndexNum == 10){
   retString = "BLUE_BOTTOM";
  }  
  if(pinIndexNum == 11){
    retString = "AQUA_BOTTOM";
  }
  if(pinIndexNum == 12){
   retString = "RED_BOTTOM";
  }
  if(pinIndexNum == 13){
    retString = "GREEN_BOTTOM";
  }
  if(pinIndexNum == 14){
    retString = "YELLOW_BOTTOM";
  }
  if(pinIndexNum == 15){
    retString = "PURPLE_BOTOTM";
  }
  if(pinIndexNum == 16){
    retString = "IR_BOTTOM";
  }

  return "RED_BOTTOM";
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}







//
//
//void moveLedForward(){
//  pinIndexNum += 1;
//  if(pinIndexNum >= numLeds){
//    pinIndexNum = 0;
//  }
//  strengthIndexNum = 0;
//
//  setupPins();
//
//  int thisPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
//  pinMode(thisPinNum, HIGH);
//}


//
//
//void moveLedBackward(){
//  pinIndexNum -= 1;
//  if(pinIndexNum < 0){
//    pinIndexNum = numLeds - 1;
//  }
//  strengthIndexNum = 0;
//
//  setupPins();
//
//  int thisPinNum = ledPinArray[pinIndexNum][strengthIndexNum];
//  pinMode(thisPinNum, HIGH);
//}



