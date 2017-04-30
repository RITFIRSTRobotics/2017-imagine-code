#include <SPI.h>
#include <RH_NRF24.h>

#include "Robot.h"

Robot bot;

void setup() {
  Serial.begin(9600);
  bot.init();
}

void loop() {
  bot.update_loop();
  
  delay(20);  
}
