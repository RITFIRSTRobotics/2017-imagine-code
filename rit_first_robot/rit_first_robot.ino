#include <SoftwareServo.h>

#include <SPI.h>
#include <RH_NRF24.h>


SoftwareServo servo;
SoftwareServo servo2;

void setup() {
  Serial.begin(9600);
  /*
  //pinMode(14, OUTPUT);
  if (!servo.attach(14)) {
    Serial.println("servo failure");
  }
  //pinMode(15, OUTPUT);
  if (!servo2.attach(15)) {
    Serial.println("servo2 failure");
  }
  
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
*/
}

void loop() {/*
  Serial.print(nrf24.available());
  if (nrf24.available()) {
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len)) {
      Serial.print("got request: ");
      //Serial.println((char*)buf);


      analogWrite(A_ENABLE, buf[0]);
      Serial.print(buf[0]);
      Serial.print(", ");
      Serial.print(buf[1]);
      Serial.print(", ");
      Serial.print(buf[2]);
      Serial.print("\n");

      // Send a reply
      uint8_t data[] = "And hello back to you";
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      Serial.println("Sent a reply");
    }
  }

  analogWrite(A_ENABLE, 127);
  */
  delay(20);
  
  
}
