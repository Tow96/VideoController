/** actions.h
 * Copyright (c) Jose Tow, 2021
 * Header file that contains the mapping for the actions that each button will
 * execute. Also contains the class for the Queue
 */

#ifndef ACTIONS_H_
  #define ACTIONS_H_

  // If there is a configuration file present, utilizes the variables set in it
  // If not present in the configuration file, the code won't compile
  #ifdef CONFIGURATION_H_
    #include "configuration.h"
  #else
    #include <Arduino.h>
    #define actionQueueSize 3
    #define DEBUG
    #define DEBUG_DUMP_ACTIONS
  #endif

  // These define special actions that take more than one keystroke
  #define NO_ACTION 0x00
  #define ACTION_SOURCE_MONITOR_WINDOW -1
  #define ACTION_TIMELINES_WINDOW -2
  #define ACTION_PROGRAM_WINDOW -3
  #define ACTION_AUDIO_CLIP_MIXER_WINDOW -4
  #define ACTION_EXPORT_MEDIA -5
  #define ACTION_TRIM_BACK -6
  #define ACTION_TRIM_FORWARD -7
  #define ACTION_ADD_EDIT -8

  // Sets the action for each button, ordered by number.
  // Check the documentation in folder "Schematics" to see which button is which
  const long buttonAssignments[] = {
    ACTION_EXPORT_MEDIA,
    ACTION_ADD_EDIT,
    KEY_F,
    NO_ACTION,
    ACTION_SOURCE_MONITOR_WINDOW,
    ACTION_TIMELINES_WINDOW,
    ACTION_PROGRAM_WINDOW,
    NO_ACTION,
    ACTION_AUDIO_CLIP_MIXER_WINDOW,
    KEY_V,
    KEY_C,
    KEY_T,
    KEY_S,
    KEY_Z,
    NO_ACTION,
    KEY_SPACE
  };

  // Sets the actions for each knob movement (CW, CCW)
  const long topKnobAssignments[] = {ACTION_TRIM_FORWARD, ACTION_TRIM_BACK};
  const long midKnobAssignments[] = {KEY_RIGHT_BRACE, KEY_SLASH};
  const long botKnobAssignments[] = {KEY_DOWN, KEY_UP};
  const long wheelAssignments[] = {KEY_RIGHT, KEY_LEFT};


  // Function that executes a keyboard action
  void performAction(int action) {
  switch (action) {

    case NO_ACTION:
      break;

    case ACTION_TIMELINES_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_1);
      Keyboard.release(KEY_1);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;

    case ACTION_SOURCE_MONITOR_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_2);
      Keyboard.release(KEY_2);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;

    case ACTION_PROGRAM_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_8);
      Keyboard.release(KEY_8);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;

    case ACTION_AUDIO_CLIP_MIXER_WINDOW:
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_9);
      Keyboard.release(KEY_9);
      Keyboard.release(MODIFIERKEY_SHIFT);
      break;

    case ACTION_EXPORT_MEDIA:
      performAction(ACTION_TIMELINES_WINDOW);
      Keyboard.press(MODIFIERKEY_CTRL);
      Keyboard.press(KEY_M);
      Keyboard.release(KEY_M);
      Keyboard.release(MODIFIERKEY_CTRL);
      break;

    case ACTION_TRIM_BACK:
      Keyboard.press(MODIFIERKEY_CTRL);
      Keyboard.press(KEY_LEFT);
      Keyboard.release(KEY_LEFT);
      Keyboard.release(MODIFIERKEY_CTRL);
      break;

    case ACTION_TRIM_FORWARD:
      Keyboard.press(MODIFIERKEY_CTRL);
      Keyboard.press(KEY_RIGHT);
      Keyboard.release(KEY_RIGHT);
      Keyboard.release(MODIFIERKEY_CTRL);
      break;

    case ACTION_ADD_EDIT:
      Keyboard.press(MODIFIERKEY_CTRL);
      Keyboard.press(KEY_K);
      Keyboard.release(KEY_K);
      Keyboard.release(MODIFIERKEY_CTRL);
      break;

    default:
      Keyboard.press(action);
      Keyboard.release(action);
  }
}

  /** ActionQueue
   * Object that contains all the functions utilized for the Queue
   * 
   * The function of the queue is to limit the amount of actions that can be 
   * executed per frame (loop) of the device. Also blocks any action that was
   * executed on the previous frame or already queued for this frame in order 
   * to provide another layer of debouncing
   * 
   * The queue exists becaise of the USB Keyboard 500 keystrokes per second limit
   * According to documentation some OS's limit it further to 62.5 keystrokes
   */ 
  class ActionQueue {
    private:
      int actionsForThisFrame[actionQueueSize] =  {};
      int lastActions[actionQueueSize] = {};
      byte actionIndex = 0;

    public:
      ActionQueue(){};  // Constructor
      ~ActionQueue(){}; // Destructor

      // Adds an action to the queue
      void queueAction(int action) {
        // If the queue is full, does nothing
        if (actionIndex >= actionQueueSize) {
          #if defined(DEBUG) && defined(DEBUG_DUMP_ACTIONS)
            Serial.println("Queue full for this frame");
          #endif
          return;
        }

        if (action == NO_ACTION) return;

        // If the same action is already queued for this frame, skips it
        for (int i = 0; i < actionIndex; i++) {
          if (action == actionsForThisFrame[i]){
            #if defined(DEBUG) && defined(DEBUG_DUMP_ACTIONS)
              Serial.println("Queue already has this action queued");
            #endif
            return;
          }
        }

        this->actionsForThisFrame[this->actionIndex] = action;
        this->actionIndex++;
      }

      // Executes all actions and clears the queue
      void executeActions() {

        for (int i = 0; i < actionQueueSize; i++) {
          
          if (this->actionsForThisFrame[i] == NO_ACTION) continue;
          
          // Checks if the action was already executed last frame
          bool actionAlreadyDone = false;
          for (int j = 0; j < actionQueueSize; j++) {
            if (this->lastActions[j] == this->actionsForThisFrame[i]) {
              actionAlreadyDone = true;
              break;
            }
          }

          // if the action was not done in the last frame, executes it
          if (!actionAlreadyDone) performAction( this->actionsForThisFrame[i] );

          // Clears the action slot
          this->actionsForThisFrame[i] = NO_ACTION;
        }

        // Resets the action index once finished
        this->actionIndex = 0;
      }
  };

#endif