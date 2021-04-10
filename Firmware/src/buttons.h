/** buttons.h
 * Copyright (c) Jose Tow, 2021
 * Header file that contains the classes utilized to contain the states of a
 * button so it easier to work with.
 */

#ifndef BUTTONS_H_
  #define BUTTONS_H_

  // If there is a configuration file present, utilizes the variables set in it
  // If not present in the configuration file, the code won't compile
  #ifdef CONFIGURATION_H_
    #include "configuration.h"
  #else
    #include <Arduino.h>
    #define switchDebounceTime 500
  #endif

  // These define the states of the button
  // TODO: Add states for leaving the button pressed or unpressed for x time
  #define BUTTON_UNPRESSED 0
  #define BUTTON_RISINGEDGE 1
  #define BUTTON_PRESSED 2
  #define BUTTON_FALLINGEDGE 3

  /** Button
   * Object that contains all the states utilized for a button
   * Does not read from IO so the pins can be read in any fashion (e.g. Keyboard Matrix)
   */ 
  class Button {
    private:
      bool state = 0;
      bool lastState = 0;

      elapsedMillis debounceTimestamp = 0;
      bool debouncedState = 0;
      bool lastDebouncedState = 0;

    public:
      Button(){};  // constructor
      ~Button(){}; // Destructor

      // Stores a new state and passes the old one to previous state
      // Also runs the debouncing of the button
      void setState(bool newState) {
        // Moves the current state to lastState and sets the new state
        this->lastState = this->state;
        this->state = newState;

        // If the previous and new state are different, resets the debouncing timestamp
        if (this->state != this->lastState){
          this-> debounceTimestamp = 0;
        }
      }

      // Checks and returns if the debounced button is in one of previously defined states
      int getState(){
        
        // Sets a new value for the state if enough time since the last debounce has passed
        // This function trusts that the setState function is called faster than the "switchDebounceTime"
        if (debounceTimestamp >= switchDebounceTime){
          this->lastDebouncedState = this->debouncedState;
          this->debouncedState = this->state;
        }

        // Returns a value depending on the state and last state of the debounced button:
        // FALLING EDGE
        if (this->lastDebouncedState && !this->debouncedState) return BUTTON_FALLINGEDGE;
        // PRESSED
        if (this->lastDebouncedState && this->debouncedState) return BUTTON_PRESSED;
        // RISING EDGE
        if (!this->lastDebouncedState && this->debouncedState) return BUTTON_RISINGEDGE;
        // Default: UNPRESSED
        return BUTTON_UNPRESSED;
      }

  };

#endif