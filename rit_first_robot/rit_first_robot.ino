#include <SPI.h>
#include <RH_NRF24.h>

#define A_ENABLE 5
#define A_PHASE 3
#define B_ENABLE 6
#define B_PHASE 4

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
  
  pinMode(A_ENABLE, OUTPUT); // A xEnable
  pinMode(A_PHASE, OUTPUT); // A xPhase
  pinMode(B_ENABLE, OUTPUT); // B xEnable
  pinMode(B_PHASE, OUTPUT); // B xPhase

}

void loop() {
  Serial.println(nrf24.available());
  if (nrf24.available()) {
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len)) {
      Serial.print("got request: ");
      //Serial.println((char*)buf);

      Serial.print(buf[0]);
      Serial.print(", ");
      Serial.print(buf[1]);
      Serial.print(", ");
      Serial.print(buf[2]);
      Serial.print("\n");
    }
  }
  
  delay(50);
  
  
}
