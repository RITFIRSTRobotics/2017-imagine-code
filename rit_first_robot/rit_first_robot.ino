#include <SoftwareServo.h>

#include <SPI.h>
#include <RH_NRF24.h>

#include "Robot.h"

SoftwareServo arm_servo;
SoftwareServo grip_servo;
Robot bot(0xE7, arm_servo, grip_servo);

void setup() {
  Serial.begin(9600);
  bot.init();

  //arm_servo.attach(14);
  //grip_servo.attach(15);
}

void loop() {
  bot.update_loop();  
  delay(20);  
}
