#ifndef ROBOT_HEADER
#define ROBOT_HEADER

#include <SoftwareServo.h>

#include "Pinout.h"

/**
 * Robot definitions
 */
#define PICKUP_MIN 90
#define PICKUP_MAX 180

/**
 * Robot class definitions
 */
class Robot {
  private:
  uint64_t address; // internal address of the robot
  RH_NRF24 nrf24; // Wireless module
  SoftwareServo arm_servo;
  SoftwareServo grip_servo;

  public:
  uint64_t getAddress();

  void init();
  void tankDrive(uint8_t left_side, uint8_t right_side);
  void updateArm(uint8_t arm_value, uint8_t grip_value);
  
}

#endif
