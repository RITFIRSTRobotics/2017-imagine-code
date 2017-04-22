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
#define WIRELESS_DATA_CENTER 127

#define HIGH_THRESHOLD 956

#define DEADZONE 4

/**
 * Robot class definitions
 */
class Robot {
  private:
  RH_NRF24 nrf24; // Wireless module
  uint8_t address; // Address of the wireless module
  uint16_t previous_read;
  SoftwareServo arm_servo;
  SoftwareServo grip_servo;
  void drive(uint8_t forward, uint8_t side);
  void set_arm_pos(uint8_t arm_value, uint8_t grip_value);
  void increment_address();

  public:
  Robot(SoftwareServo arm_servo, SoftwareServo grip_servo);
  void init();
  void update_loop();
  
  
};

#endif
