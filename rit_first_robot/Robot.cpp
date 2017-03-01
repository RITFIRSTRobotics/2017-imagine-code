#include "Robot.h"



/**
 * 
 */
Robot::Robot(uint64_t address, arm_servo, grip_servo) {
  this.address = address;
  this.arm_servo = arm_servo;
  this.grip_servo = grip_servo;
}

/**
 * Get the address of the robot
 * 
 * @return the address of the robot
 */
uint64_t Robot::getAddress() {
  return this.address;
}




