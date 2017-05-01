#include <Adafruit_NeoPixel.h>
#include "Robot.h"

static bool button_pressed(uint16_t analog_read, uint16_t previous_read) {
  return (analog_read > HIGH_THRESHOLD) && !(previous_read > HIGH_THRESHOLD);
}

/**
 * 
 */
Robot::Robot() {
  address = (uint8_t) 0xf0;
  RH_NRF24 nrf24;
  previous_read = 1023;
  control_led = Adafruit_NeoPixel(2, LED_DIN, NEO_RGB + NEO_KHZ800);
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

  set_address(0xf0);
    
  // Pin initalization
  pinMode(A_ENABLE, OUTPUT);
  pinMode(A_PHASE, OUTPUT);
  pinMode(B_ENABLE, OUTPUT);
  pinMode(B_PHASE, OUTPUT);

  pinMode(A6, INPUT);
  
  control_led.begin();
  control_led.setPixelColor(0, control_led.Color(255, 0, 0));
  control_led.setPixelColor(1, control_led.Color(255, 0, 0));
  control_led.show();
}

/**
 * 
 */
void Robot::drive(uint8_t forward, uint8_t side) {
  short s_forw = forward - WIRELESS_DATA_CENTER;
  short s_side = side - WIRELESS_DATA_CENTER;

  // Do a deadzone check
  if (((abs(s_forw) - DEADZONE) <= 0) && ((abs(s_side) - DEADZONE) <= 0)) {
    analogWrite(A_ENABLE, 0);
    analogWrite(B_ENABLE, 0);
    return;
  }

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
  
  // Write the motor values to the controller
  if (left_motor >= 0) {
    digitalWrite(A_PHASE, 0);
    if (left_motor >= 128) {
      analogWrite(A_ENABLE, 255);
    } else {
      analogWrite(A_ENABLE, abs(left_motor * 2));
    }
  }
  else {
    digitalWrite(A_PHASE, 1);
    if (left_motor <= -128) {
      analogWrite(A_ENABLE, 255);
    } else {
      analogWrite(A_ENABLE, abs(left_motor * 2));
    }
  }

  if (right_motor >= 0) {
    digitalWrite(B_PHASE, 1);
    if (right_motor >= 128) {
      analogWrite(B_ENABLE, 255);
    } else {
      analogWrite(B_ENABLE, abs(right_motor * 2));
    }
  }
  else {
    digitalWrite(B_PHASE, 0);
    if (right_motor <= -128) {
      analogWrite(B_ENABLE, 255);
    } else {
      analogWrite(B_ENABLE, abs(right_motor * 2));
    }
  }
    
   
}

void Robot::set_address(uint8_t addr_end) {
  uint8_t * address = (uint8_t * ) malloc(sizeof(uint8_t) * 4); // shut up the compiler
  address[0] = (uint8_t) 0xe7;
  address[1] = (uint8_t) 0xe7;
  address[2] = (uint8_t) 0xe7;
  address[3] = addr_end;

  nrf24.setNetworkAddress(address ,4);
  free(address);
}

/**
 * Increment the address of the NRF module
 * 
 * @note Uses range of 0xe7e7e7f0 to 0xe7e7e7f3
 */
void Robot::increment_address() {
  if (address == 0xf3) { // loop back
    address = (uint8_t) 0xf0;
  } else {
    address += 1;
  }

  if (address == 0xf0) {
    control_led.setPixelColor(0, control_led.Color(0, 255, 0));
    control_led.setPixelColor(1, control_led.Color(0, 255, 0));
  } else if (address == 0xf1) {
    control_led.setPixelColor(0, control_led.Color(0, 255, 0));
    control_led.setPixelColor(1, control_led.Color(255, 0, 0));
  } else if (address == 0xf2) {
    control_led.setPixelColor(0, control_led.Color(0, 0, 255));
    control_led.setPixelColor(1, control_led.Color(0, 255, 0));
  } else if (address == 0xf3) {
    control_led.setPixelColor(0, control_led.Color(0, 0, 255));
    control_led.setPixelColor(1, control_led.Color(255, 0, 0));
  }

  control_led.show();
  set_address(address);
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
      drive(data[0], data[1]);
    }
  }

  uint16_t change_channel_read = analogRead(CHANNEL_PIN);
  if (button_pressed(change_channel_read, previous_read)) {
    increment_address();
  }
  previous_read = change_channel_read;
}

