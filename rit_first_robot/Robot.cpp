#include "Robot.h"

/**
 * 
 */
Robot::Robot(uint8_t _address, SoftwareServo _arm_servo, SoftwareServo _grip_servo) {
  address = _address;
  arm_servo = _arm_servo;
  grip_servo = _grip_servo;
  RH_NRF24 nrf24;
}

void Robot::init() {
  // Radio initalization
  if (!nrf24.init()) {
    Serial.println("init failed");
  }
  if (!nrf24.setChannel(1)) {
    Serial.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
    Serial.println("setRF failed");
  }
  /* Being left default for now
  if (!this.nrf24.setNetworkAddress(this.address, 5)) {
    
  }
  */
  
  // Pin initalization
  pinMode(A_ENABLE, OUTPUT);
  pinMode(A_PHASE, OUTPUT);
  pinMode(B_ENABLE, OUTPUT);
  pinMode(B_PHASE, OUTPUT);
}

/**
 * 
 */
void Robot::drive(uint8_t left_side, uint8_t right_side) {
  // Direction on the left side
  digitalWrite(A_PHASE, left_side > 127);

  // Speed on the left side
  analogWrite(A_ENABLE, (abs(left_side - 127)) * 2);

  // Direction on the right side
  digitalWrite(B_PHASE, right_side > 127);

  // Speed on the right side
  analogWrite(B_ENABLE, (abs(right_side - 127)) * 2);
}

/**
 * 
 */
void Robot::set_arm_pos(uint8_t arm_value, uint8_t grip_value) {
  arm_servo.write(arm_value);
  grip_servo.write(grip_value);
}

void Robot::update_loop() {
  Serial.print(nrf24.available());
  if (nrf24.available()) {
    uint8_t data[WIRELESS_DATA_LENGTH];
    uint8_t data_len = sizeof(data);
    if (nrf24.recv(data, &data_len)) {
      //Serial.print("got request: ");
      Serial.println((char*)data);

      drive(data[0], data[1]);

      uint8_t arm_value = arm_servo.read();
      uint8_t grip_value = grip_servo.read();
      if (data[2] && !data[3]) {
        grip_value += 1;
      }
      if (!data[2] && data[3]) {
        arm_value -= 1;
      }
      if (data[4] && !data[5]) {
        arm_value += 1;
      }
      if (!data[4] && data[5]) {
        arm_value -= 1;
      }

      /*
      uint8_t data[] = "reply";
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      */      
    }
  }

  SoftwareServo::refresh();
}

