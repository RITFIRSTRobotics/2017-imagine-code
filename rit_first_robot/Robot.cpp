#include "Robot.h"

/**
 * 
 */
Robot::Robot(SoftwareServo _arm_servo, SoftwareServo _grip_servo) {
  address = (uint8_t) 0xe7e7e7f0;
  arm_servo = _arm_servo;
  grip_servo = _grip_servo;
  RH_NRF24 nrf24;
}

void Robot::init() {
  // Radio initalization
  if (!nrf24.init()) {
    Serial.println("init failed");
  }
  if (!nrf24.setChannel(2)) {
    Serial.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
    Serial.println("setRF failed");
  }
/*
  if (!nrf24.setNetworkAddress(&address, 4)) {
    Serial.println("Error setting address");
    Serial.println("Debug info:");
    Serial.print("address: \t");
    Serial.print(address);
  }
*/
  
    
  // Pin initalization
  pinMode(A_ENABLE, OUTPUT);
  pinMode(A_PHASE, OUTPUT);
  pinMode(B_ENABLE, OUTPUT);
  pinMode(B_PHASE, OUTPUT);

  //pinMode(A6, INPUT_PULLUP);
}

/**
 * 
 */
void Robot::drive(uint8_t forward, uint8_t side) {
  
  
  short s_forw = forward - WIRELESS_DATA_CENTER;
  short s_side = side - WIRELESS_DATA_CENTER;

  short left_motor = 0;
  short right_motor = 0;

  // Calculate the motor outputs
  if (s_forw > 0) {
    if (s_side > 0) {
      left_motor = s_forw - s_side;
      right_motor = max(s_forw, s_side);
    } else {
      left_motor = max(s_forw, -s_side);
      right_motor = s_forw + s_side;
    }
  } else {
    if (s_side > 0) {
      left_motor = -1 * max(-1 * s_forw, s_side);
      right_motor = s_forw + s_side;
    } else {
      left_motor = s_forw - s_side;
      right_motor = -1 * max(-1 * s_forw, -s_side);
    }
  }

  Serial.print("drive: ");
  Serial.print(left_motor);
  Serial.print(", ");
  Serial.print(right_motor);
  Serial.println();

  left_motor *= -1;
  right_motor *= -1;


  // Write the motor values to the controller
  if (left_motor >= 0) {
    digitalWrite(B_PHASE, 0);
    if (left_motor >= 255) {
      analogWrite(B_ENABLE, 255);
    } else {
      analogWrite(B_ENABLE, abs(left_motor * 2));
    }
  }
  else {
    digitalWrite(B_PHASE, 1);
    if (left_motor <= -255) {
      analogWrite(B_ENABLE, 255);
    } else {
      analogWrite(B_ENABLE, abs(left_motor * 2));
    }
  }

  if (right_motor >= 0) {
    digitalWrite(A_PHASE, 1);
    if (right_motor >= 255) {
      analogWrite(A_ENABLE, 255);
    } else {
      analogWrite(A_ENABLE, abs(right_motor * 2));
    }
  }
  else {
    digitalWrite(A_PHASE, 0);
    if (right_motor <= -255) {
      analogWrite(A_ENABLE, 255);
    } else {
      analogWrite(A_ENABLE, abs(right_motor * 2));
    }
  }
    
   
}

/**
 * Increment the address of the NRF module
 * 
 * @note Uses range of 0xe7e7e7f0 to 0xe7e7e7f3
 */
void Robot::increment_address() {
  if (address == 0xe7e7e7f3) { // loop back
    address = (uint8_t) 0xe7e7e7f0;
  } else {
    address += 1;
  }
  if (!nrf24.setNetworkAddress(&address, 4)) {
    Serial.println("Error setting address");
    Serial.println("Debug info:");
    Serial.print("address: \t");
    Serial.print(address);
  }
}

/**
 * 
 */
void Robot::set_arm_pos(uint8_t arm_value, uint8_t grip_value) {
  arm_servo.write(arm_value);
  grip_servo.write(grip_value);
}

void Robot::update_loop() {
  
  if (nrf24.available()) {
    uint8_t data[WIRELESS_DATA_LENGTH];
    uint8_t data_len = sizeof(data);
    if (nrf24.recv(data, &data_len)) {
      //Serial.print("got request: ");
      Serial.println((char*)data);

      drive(data[0], data[1]);
/*
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
*/
      
      uint8_t data[] = "reply";
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();      
    }
  }

  SoftwareServo::refresh();
}

