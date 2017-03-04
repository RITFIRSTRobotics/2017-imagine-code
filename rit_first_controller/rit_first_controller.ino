#include <SPI.h>
#include <RH_NRF24.h>

RH_NRF24 nrf24;

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

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  
  uint8_t data[] = {(analogRead(A0) / 4), (analogRead(A1) / 4), digitalRead(2), digitalRead(3), 0, 0};
  nrf24.send(data, sizeof(data));

  Serial.print("Sending: ");
  Serial.print(data[0]);
  Serial.print(" ");
  Serial.print(data[1]);
  Serial.print(" ");
  Serial.print(data[2]);
  Serial.print("\n");

  delay(20);
}
