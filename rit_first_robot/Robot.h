#ifndef ROBOT_HEADER
#define ROBOT_HEADER

#include <SoftwareServo.h>
#include <SPI.h>
#include <RH_NRF24.h>

#include <Arduino.h>

#include "Pinout.h"

/**
 * Robot definitions
 */
#define PICKUP_MIN 90
#define PICKUP_MAX 180

#define WIRELESS_DATA_LENGTH 6

/**
 * Robot class definitions
 */
class Robot {
  private:
  uint8_t address; // internal address of the robot
  RH_NRF24 nrf24; // Wireless module
  SoftwareServo arm_servo;
  SoftwareServo grip_servo;
  void drive(uint8_t left_side, uint8_t right_side);
  void set_arm_pos(uint8_t arm_value, uint8_t grip_value);

  public:
  Robot(uint8_t address, SoftwareServo arm_servo, SoftwareServo grip_servo);
  void update_loop();
  
  
};

#endif
