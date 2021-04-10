/** configuration.h
 * Copyright (c) Jose Tow, 2021
 * Header file that contains the variables for the whole firmware
 */

#ifndef CONFIGURATION_H_
  #define CONFIGURATION_H_
  // ----  Debug Mode
  #define DEBUG
  //#define DEBUG_DUMP_MATRIX
  //#define DEBUG_DUMP_KNOBPOSITIONS
  #define DEBUG_DUMP_ACTIONS

  #define switchDebounceTime 50  // Sets the amount of miliseconds to count a key press

  #define pulsesPerDetent 4     // Encoder pulses that count as a movement

  #define actionQueueSize 6     // Amount of actions that can be executed per frame

#endif