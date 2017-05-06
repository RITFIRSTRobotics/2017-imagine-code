/**
 * rit_first_controller.ino
 * 
 * A controller for the robots in the RIT FIRST ImagineRIT project
 * 
 * @author Connor Henley, @thatging3rkid
 * @author Gregory Goh, @ShinyTeeth
 */
#include <SPI.h>
#include <RH_NRF24.h>

#define BUTTON_TOP    2
#define BUTTON_LEFT   3
#define BUTTON_BOTTOM 4
#define BUTTON_RIGHT  5

RH_NRF24 nrf24;

void setup() {
  Serial.begin(9600);
  while (!Serial) 
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");   

  // Button initalization
  pinMode(BUTTON_TOP, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_BOTTOM, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  // Thumbstick initalization
  pinMode(A0, INPUT);         // Thumbstick Y (front/back)
  pinMode(A1, INPUT);         // Thumbstick X (left/right)

  // Loop for the specified milliseconds, waiting for address preset input
  addressPresetLoop(5000);
}

void loop() {

  // read input and build data array
  uint8_t data[] = {((analogRead(A0)) / 4), ((analogRead(A1)) / 4), digitalRead(2), digitalRead(3), digitalRead(4), digitalRead(5)};
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

/**
 * This method starts a loop that listens for button input that
 * decides what network address preset to use. 
 * Uses default network address upon timeout.
 * 
 * @param interval the time in milliseconds to loop and wait for input
 */
void addressPresetLoop (unsigned long interval) {
  unsigned long previousTime = millis();  // start a timer
  uint8_t addr_end = (uint8_t) 0xf0;
  
  // keep listening for input to decide which address preset to use
  while ((millis() - previousTime) < interval) {
    if (!digitalRead(BUTTON_TOP))    addr_end = (uint8_t) 0xf0;
    if (!digitalRead(BUTTON_LEFT))   addr_end = (uint8_t) 0xf1;
    if (!digitalRead(BUTTON_BOTTOM)) addr_end = (uint8_t) 0xf2;
    if (!digitalRead(BUTTON_RIGHT))  addr_end = (uint8_t) 0xf3;
  }

  uint8_t * address = malloc(sizeof(uint8_t) * 4);
  address[0] = (uint8_t) 0xe7;
  address[1] = (uint8_t) 0xe7;
  address[2] = (uint8_t) 0xe7;
  address[3] = addr_end;  

  // Set address
  nrf24.setNetworkAddress(address ,4);  
}

