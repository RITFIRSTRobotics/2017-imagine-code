/******************************************************************
Author: Gregory Goh
Written with the official Arduino IDE, AVAILABLE here:
http://www.arduino.cc/en/Main/software

*******************************************************************/
/*   INCLUDES                                                     */
#include <WString.h> //Official Arduino string library
/******************************************************************/
/*   CONSTANTS                                                    */
#define LED_TURN_ON_TIMEOUT  1000       //Timeout for LED power time (defines how long the LED stays powered on) in milliseconds
#define LED_PIN 13                      //Pin number on which the LED is connected
#define SERIAL_BAUDRATE 9600            //Baud-Rate of the serial Port
#define STX "\x02"                      //ASCII-Code 02, text representation of the STX code
#define ETX "\x03"                      //ASCII-Code 03, text representation of the ETX code
#define RS  "$"                         //Used as RS (record separator) code
#define MAX_ELAPSED_TIME 30000          //Maximum time (ms) since previous input

/*   WARNING, ERROR AND STATUS CODES                              */
// STATUS
// An operation terminated successfully
#define MSG_METHOD_SUCCESS 0
// WARNINGS
// No new data is available at the serial input buffer
#define WRG_NO_SERIAL_DATA_AVAILABLE 250
// ERRORS
// A serial input commands' last char is not a '#'
#define ERR_SERIAL_IN_COMMAND_NOT_TERMINATED -1
/******************************************************************/


/*   METHOD DECLARATIONS                                          */
int readSerialInput(byte *commandArray);
void announcePresence();
void pingAfterTimeout();
/******************************************************************/


/*   METHODS                                                      */
void setup() {
  // setup the LED pin for output
  pinMode(LED_PIN, OUTPUT);
  // setup serial pin
  Serial.begin(SERIAL_BAUDRATE);
  announcePresence();
}

void loop() {
  // Variables to store the latest received command and subsequent return value
  // These pointers here will be the most problematic
  byte commandArray[6];
  int serialResult = 0;
  
  serialResult = readSerialInput(commandArray);
  if (serialResult == MSG_METHOD_SUCCESS) {
    // parse the command string
    // this is where the data gets broken down before forwarding to wherever
    int leftSpeed = commandArray[0];
    int rightSpeed = commandArray[1];
    int buttonA = commandArray[2];
    int buttonB = commandArray[3];
    int buttonX = commandArray[4];
    int buttonY = commandArray[5];
    Serial.print(STX);
    Serial.print(leftSpeed);
    Serial.print(RS);
    Serial.print(rightSpeed);
    Serial.print(RS);
    Serial.print(buttonA);
    Serial.print(RS);
    Serial.print(buttonB);
    Serial.print(RS);
    Serial.print(buttonX);
    Serial.print(RS);
    Serial.print(buttonY);
    Serial.print(ETX);
  }
  
  //If there is no data AVAILABLE at the serial port, let the LED blink
  if (serialResult == WRG_NO_SERIAL_DATA_AVAILABLE) {
     digitalWrite(LED_PIN, HIGH);
     delay(LED_TURN_ON_TIMEOUT);
     digitalWrite(LED_PIN, LOW);
     delay(LED_TURN_ON_TIMEOUT);
  } else {
    //If the command format was invalid, the led is turned off for two seconds
    if (serialResult == ERR_SERIAL_IN_COMMAND_NOT_TERMINATED) {
      digitalWrite(LED_PIN, LOW);
      delay(2000);
    }
  }
}

/*****************************************************************
Description:
This method reads the serial input buffer and writes the content 
of the serial input buffer to the variable which is adressed by the
pointer *command.

Return-Values:
 - MSG_METHOD_SUCCESS -> 
   ->    Serial buffer data stored to the variable which is adressed by the
         pointer *command.
 - ERR_SERIAL_IN_COMMAND_NOT_TERMINATED
   ->    Command was not terminated by a '#' (example: "command") or contains no command text (example: "#" or "").
 - WRG_NO_SERIAL_DATA_AVAILABLE
   ->    No data available

Author: Gregory Goh

******************************************************************/
int readSerialInput(byte *commandArray) {
  
  // Default return is MSG_METHOD_SUCCESS reading data from com buffer.
  int operationStatus = MSG_METHOD_SUCCESS;
  
  // check if serial data is available for reading
  if (Serial.available()) {
    //temporary variables
    char serialInByte;
    int index = 0;

    //Read serial input buffer data byte by byte
    //until '#' comes up, after 6 iterations, 
    //or no serial data is available anymore
    do {
      //Add last read serial input buffer byte to *command pointer
      serialInByte = Serial.read();
      *(commandArray + index) = serialInByte;
      index ++;
    } while (serialInByte != '#' && Serial.available() && index < 6);
    
    if (serialInByte != '#' && index < 6) {
      operationStatus = ERR_SERIAL_IN_COMMAND_NOT_TERMINATED;
    }
  } else {
    // If not serial input buffer data is AVAILABLE
    operationStatus = WRG_NO_SERIAL_DATA_AVAILABLE;
  }
  
  return operationStatus;
}

/*****************************************************************
Description:
This method writes a string to the serial interface output
buffer. The string is formatted as 

[STX]
READY[ETX]

Values written in braces ("[]") Represent an ASCII control
code. For further information about ASCII codes visit
http://www.asciitable.com

Author: Gregory Goh

******************************************************************/
void announcePresence() {
  Serial.print(STX);
  Serial.print("READY");
  Serial.print(ETX);
}

/*****************************************************************
Description:
This method writes a string to the serial interface output
buffer. The string is formatted as 

[STX]
BORED[ETX]

Values written in braces ("[]") Represent an ASCII control
code. For further information about ASCII codes visit
http://www.asciitable.com

Author: Gregory Goh

******************************************************************/
void pingAfterTimeout() {
  Serial.print(STX);
  Serial.print("BORED");
  Serial.print(ETX);
}
