#include <Event.h>
#include <Timer.h>
#define bLights 0
#define rLights 1
#define sLights 2
#define valSize 4
Timer matchTime;
Timer blue;
Timer red;
int blueLights[3] = {9,11,12};
int redLights[3] = {5,6,8};
int sideLights[3] = {2,3,4};
int sideLightsVals[3]={255,0,0};
bool bGb = false;
bool rGr = false;//used to determine if the goal is it's color or white

int ballCounts[valSize];
//Testing with buttons instead of Sensors
int btns[4] = {45, 43, 39, 41};        //
int vals[4] = { 0, 0, 0, 0};          //
int previousVals[4] = { 0, 0, 0, 0}; //
//==================================//
/*
Reads infrared sensors on pins
    When a sensor is triggered
        lights on that side will flash
 s1|            |s2
 s0|____________|s3
Blue   Side     Red

RED   0,255,0
BLUE  0,0,255
GREEN 255,0,0
ORANGE 40,255,0
WHITE 150,255,60
*/

boolean matchEnabled = false;
//int vals[valSize];
int readEn[valSize] = {1,1,1,1};
int thresholds[valSize] = {400, 400, 400, 400};
int ups[valSize] =        { 50,  50,  50,  50};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 2; i<11; i++){
    pinMode(i,OUTPUT);
  }
  setRGR();
  setBGB();
  delay(1000);
  toggleRedGoal();
  delay(1000);
  toggleRedGoal();
  
}

void matchEnable(){
  matchEnabled = true; 
}
void matchDisable(){
  matchEnabled = false;
}
void disableReading(int pin){
    readEn[pin]=0;
}
void enableReading(int pin){
    readEn[pin]=1;
}

void setRGR(){
  analogWrite(redLights[0],0);
  analogWrite(redLights[1], 255);
  analogWrite(redLights[2],0);
  rGr = true;
}
void setRGW(){
  analogWrite(redLights[0],150);
  analogWrite(redLights[1], 255);
  analogWrite(redLights[2],60);
  rGr = false;
}

void setBGB(){
  analogWrite(blueLights[0],0);
  analogWrite(blueLights[1], 0);
  analogWrite(blueLights[2],255);
  bGb = true;
}
void setBGW(){
  analogWrite(blueLights[0],150);
  analogWrite(blueLights[1], 255);
  analogWrite(blueLights[2],60);
  bGb = false;
}

void toggleRedGoal(){
  if(rGr){
    setRGW();
  }
  else{
    setRGR();
  }
}

void toggleBlueGoal(){
  if(bGb){
    setBGW();
  }
  else{
    setBGB();
  }
}
void flashRedGoal(){
  red.every(250,toggleRedGoal,6);
}
void flashBlueGoal(){
  blue.every(250,toggleBlueGoal,6);
}

void gTrFade(){
  sideLightsVals[0]-=1;
  sideLightsVals[1]+=1;
  for(int i=0;i<3;i++){
    analogWrite(sideLights[i],sideLightsVals[i]);
  }
}

void setFieldYellow(){
  sideLightsVals[0]=255;
  sideLightsVals[1]=130;
  sideLightsVals[2]=0;
  analogWrite(sideLights[0], 130);
  analogWrite(sideLights[1], 255);
  analogWrite(sideLights[2],   0);
}

void yTrFade(){
  sideLightsVals[0]-=1;
  analogWrite(sideLights[0], sideLightsVals[0]);
}

void updateButtons(){
  previousVals[0]=vals[0];
  previousVals[1]=vals[1];
  previousVals[2]=vals[2];
  previousVals[3]=vals[3];
  vals[0]=digitalRead(btns[0]);
  vals[1]=digitalRead(btns[1]);
  vals[2]=digitalRead(btns[2]);
  vals[3]=digitalRead(btns[3]);
}

void readButtons(){
  Serial.print("S");
  matchTime.after(120000,matchDisable);
  matchTime.every(120000/255,gTrFade,255);
  while(matchEnabled){
    red.update();
    blue.update();
    matchTime.update();
    updateButtons();
    for (int b = 0; b<4; b++){
      if (vals[b] != previousVals[b] && vals[b]==1){
        Serial.print((String)b);
        if(b>1){
          flashRedGoal();
        }
        else{
          flashBlueGoal();
        }
      }
    }
  }
}



void readSensors(){
  Serial.print("S");
  matchTime.after(100000, setFieldYellow);
  matchTime.after(120000,matchDisable);
  matchTime.every(20000/130,gTrFade,130);
  while(matchEnabled){
    red.update();
    blue.update();
    matchTime.update();
    for(int i = 0; i<valSize; i++){ //checking each sensor
      int val = analogRead(i); //get the analog value
      vals[i] = val; //put the analog value in the list of values
      if (val<thresholds[i] && readEn[i] == 1){ //if sensor triggered, do score stuff       
        Serial.print((String)i);
        disableReading(i);
        //Flash the goal that scored
        if(i<2){
          flashRedGoal();
        }
        else{
          flashBlueGoal();
        }
      }
      else {
        if (val>thresholds[i]+ups[i] && readEn[i]==0){
          enableReading(i);
        }
      }
    } 
  }
}

void debugPrint(){
      for(int i = 0; i<valSize; i++){
        if (vals[i]<thresholds[i] && readEn[i]==1){
            ballCounts[i] += 1;
        }
    }
    for(int i = 0; i<valSize; i++){
        Serial.print(" | Sensor "+(String)i+": "+"Ball "+(String)ballCounts[i]);
    }
    Serial.println();
    Serial.print("[ ");
    for(int i = 0; i<valSize-1; i++){
        Serial.print((String)readEn[i]+", ");
    }
    Serial.print((String)readEn[valSize-1]+"]\n");
    //Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  matchEnable();
  analogWrite(sideLights[0],255);
  analogWrite(sideLights[0],0);
  analogWrite(sideLights[0],0);
  
  
  //readSensors();
  readSensors();
  while(true){//breathe effect after match has run
    for(int i=0;i<3;i++){
      analogWrite(sideLights[i],0);
    }
    for(int cv=0;cv<61;cv++){
      analogWrite(sideLights[0],((140/60)*cv));
      analogWrite(sideLights[1],((255/60)*cv));
      analogWrite(sideLights[2],((60/60)*cv));
      delay(100);
    }
    for(int cv=60;cv>=0;cv--){
      analogWrite(sideLights[0],((140/60)*cv));
      analogWrite(sideLights[1],((255/60)*cv));
      analogWrite(sideLights[2],((60/60)*cv));
      delay(100);
    }
  }
  
}
