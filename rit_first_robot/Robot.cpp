#include "Robot.h"

/**
 * 
 */
Robot::Robot(uint8_t address, SoftwareServo arm_servo, SoftwareServo grip_servo) {
  address = address;
  arm_servo = arm_servo;
  grip_servo = grip_servo;
  RH_NRF24 nrf24;

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
 * Get the address of the robot
 * 
 * @return the address of the robot
 */
uint8_t Robot::getAddress() {
  return address;
}




