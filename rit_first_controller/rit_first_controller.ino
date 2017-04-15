#include <SPI.h>
#include <RH_NRF24.h>

RH_NRF24 nrf24;

// Address presets
const uint8_t ROBOT_0_ADDR = '0xE7E7E7F0';
const uint8_t ROBOT_1_ADDR = '0xE7E7E7F1';
const uint8_t ROBOT_2_ADDR = '0xE7E7E7F2';
const uint8_t ROBOT_3_ADDR = '0xE7E7E7F3';

void setup() {
  Serial.begin(9600);
  while (!Serial) 
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");   

  pinMode(2, INPUT_PULLUP);   // Left Button
  pinMode(3, INPUT_PULLUP);   // Up Button
  pinMode(4, INPUT_PULLUP);   // Right Button
  pinMode(5, INPUT_PULLUP);   // Down Button
  pinMode(A0, INPUT);         // Thumbstick Y (front/back)
  pinMode(A1, INPUT);         // Thumbstick X (left/right)

  // Loop for the specified milliseconds, waiting for address preset input
  addressPresetLoop(5000);
}

void loop() {

  // read input and build data array
  uint8_t data[] = {((analogRead(A0) - 1) / 4), ((analogRead(A1) - 8) / 4), digitalRead(2), digitalRead(3), digitalRead(4), digitalRead(5)};
  nrf24.send(data, sizeof(data));   // send data to the network address

  // Write data to serial
  Serial.print("Sending: ");
  Serial.print(data[0]);
  Serial.print(" ");
  Serial.print(data[1]);
  Serial.print(" ");
  Serial.print(data[2]);
  Serial.print(" ");
  Serial.print(data[3]);
  Serial.print(" ");
  Serial.print(data[4]);
  Serial.print(" ");
  Serial.print(data[5]);
  Serial.print(" ");
  Serial.print(data[6]);
  Serial.print(" ");
  Serial.print("\n");
  

  nrf24.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(100))
  { 
    // Should be a reply message for us now   
    if (nrf24.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is nrf24_server running?");
  }  
  delay(20);
}

/*****************************************************************
Description:
This method starts a loop that listens for button input that
decides what network address preset to use. 
Uses default network address upon timeout.
 
Parameters:
  interval -> the time in milliseconds to loop and wait for input

Author: Gregory Goh

******************************************************************/
void addressPresetLoop (unsigned long interval) {
  unsigned long previousTime = millis();  // start a timer

  // keep listening for input to decide which address preset to use
  while ((millis() - previousTime) < interval) {
    if (digitalRead(2)) nrf24.setNetworkAddress(&ROBOT_0_ADDR, 4);
    if (digitalRead(3)) nrf24.setNetworkAddress(&ROBOT_1_ADDR ,4);
    if (digitalRead(4)) nrf24.setNetworkAddress(&ROBOT_2_ADDR ,4);
    if (digitalRead(5)) nrf24.setNetworkAddress(&ROBOT_3_ADDR ,4);
  }

  // network address defaults to 0xE7E7E7E7E7 after timeout
}

