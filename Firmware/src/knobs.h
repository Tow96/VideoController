/** knobs.h
 * Copyright (c) Jose Tow, 2021
 * Header file that contains the classes utilized to contain the states of a
 * knob so it easier to work with.
 */

#ifndef KNOBS_H_
  #define KNOBS_H_

  #ifdef CONFIGURATION_H_
    #include "configuration.h"
  #else
    #include <Arduino.h>
    #define pulsesPerDetent 4
  #endif

  // States of the knob movement
  // TODO: Add more states for modes for different speed turning
  #define KNOB_CCW   -1
  #define KNOB_STILL  0
  #define KNOB_CW     1

  #include <Encoder.h>

  /** Knob
   * Object that contains all the states utilized for a knob
   * The constructor requires the pin Associations to run the Encoder library
   */ 
  class Knob {
    private:
      Encoder* encoder;
      long position = 0;
      long lastPosition = 0;
      int delta = 0;

    public:
      // Constructor
      Knob (int pinA, int pinB) { this->encoder = new Encoder(pinA, pinB); }
      ~Knob(){};  // Destructor

      // Stores the current position of the knob
      void read() { this->position = this->encoder->read(); }

      // Indicates if the knob moved in a determined direction
      int getDelta() {

        // Compares the current position to the last position stored
        if ( abs(this->position - this->lastPosition) >= pulsesPerDetent) {

          // CW movement
          if (this->position > this->lastPosition) { delta = KNOB_CW; } 
          
          // CCW movement
          else { delta = KNOB_CCW; }

          // Stores the new position
          lastPosition = position;
        }
        else{ this->delta = 0; }
        
        return this->delta;
      } 

  };


#endif