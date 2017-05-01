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

#define DEADZONE 5

#define ARMSPEED 6
#define GRIPSPEED 4

#define ARM_MAX 145
#define ARM_MIN 50

#define GRIP_MAX 175
#define GRIP_MIN 130

/**
 * Robot class definitions
 */
class Robot {
  private:
  RH_NRF24 nrf24; // Wireless module
  uint8_t address; // Address of the wireless module
  uint16_t previous_read;
  Adafruit_NeoPixel control_led;
  void drive(uint8_t forward, uint8_t side);
  void set_arm_pos(uint8_t arm_value, uint8_t grip_value);
  void increment_address();
  void set_address(uint8_t addr_end);

  public:
  Robot();
  void init();
  void update_loop();
  
  
};

#endif
