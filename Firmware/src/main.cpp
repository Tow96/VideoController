/*
 * Video Controller macro pad firmware
 * Copyright April 2021 José Tow
 * 
 * This code is designed to run on the following hardware: https://github.com/Tow96/VideoController
 * 
 * This code is heavily based in the project made by Zack Freedman:
 *   https://github.com/ZackFreedman/Big-Wheel
 *   https://www.youtube.com/watch?v=72a85tWOJVY
 */

#include <Arduino.h>
#include <Encoder.h>

#include "configuration.h"

#include "pins.h"
#include "actions.h"
#include "buttons.h"
#include "knobs.h"

ActionQueue actionQueue;
Button buttonMatrix[16];
Knob* knobs[4] = {
  new Knob(topKnobA, topKnobB),
  new Knob(midKnobA, midKnobB),
  new Knob(botKnobA, botKnobB),
  new Knob(wheelA, wheelB)
};

void setup()
{
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  // Sets the keyboard matrix up
  for (int i = 0; i < 4; i++)
  {
    // Rows
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);

    // Columns
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop()
{
  // Reads the keyboard matrix and stores it in the buttons
  for (int i = 0; i < 4; i++) {
    // Sets the row pin to Low to read each column
    digitalWrite(rowPins[i], LOW);

    // Reads each column
    for (int j = 0; j < 4; j++) {
        #if defined(DEBUG) && defined(DEBUG_DUMP_MATRIX)
          Serial.print("Pressed key: ");
          Serial.print(j);
          Serial.print(",");
          Serial.println(i);
        #endif

        int pos =  (j*4) + i;
        buttonMatrix[pos].setState(!digitalRead(colPins[j]));

    }

    // Sets the row pin back to HIGH
    digitalWrite(rowPins[i], HIGH);
  }

  // Requests the debounced state of the buttons and if being pressed (rising edge) queues the correct action. 
  // Since this is a macro pad, no aditional inputs for the same key will be queued if left pressed.
  for (int i = 0; i < 16; i++) {
    int state = buttonMatrix[i].getState();

    if (state == BUTTON_RISINGEDGE) {
      #if defined(DEBUG) && defined(DEBUG_DUMP_ACTIONS)
        Serial.print("Queue action set for key: ");
        Serial.println(i);
      #endif

      actionQueue.queueAction(buttonAssignments[i]);
    }
  }

  // Reads the knobs and if moved, queues the action
  for (int i = 0; i < 4; i++) {
    knobs[i]->read();

    int delta = knobs[i]->getDelta();

    if ( delta != 0 ) {

      #if defined(DEBUG) && defined(DEBUG_DUMP_KNOBPOSITIONS)
        Serial.print("Knob ");
        Serial.print(i);
        Serial.print(" --> ");
      #endif

      // CW movement
      if ( delta > 0 ) {
        #if defined(DEBUG) && defined(DEBUG_DUMP_KNOBPOSITIONS)
          Serial.println("CW");
        #endif

        // Enqueues the respective action
        switch (i) {
          case 0:
            actionQueue.queueAction(topKnobAssignments[0]);
            break;
          case 1:
            actionQueue.queueAction(midKnobAssignments[0]);
            break;
          case 2:
            actionQueue.queueAction(botKnobAssignments[0]);
            break;
          case 3:
            actionQueue.queueAction(wheelAssignments[0]);
            break;
          default:
            break;
        }
      }

      // CCW movement
      if ( delta < 0 ) {
        #if defined(DEBUG) && defined(DEBUG_DUMP_KNOBPOSITIONS)
          Serial.println("CCW");
        #endif

        // Enqueues the respective action
        switch (i) {
          case 0:
            actionQueue.queueAction(topKnobAssignments[1]);
            break;
          case 1:
            actionQueue.queueAction(midKnobAssignments[1]);
            break;
          case 2:
            actionQueue.queueAction(botKnobAssignments[1]);
            break;
          case 3:
            actionQueue.queueAction(wheelAssignments[1]);
            break;
          default:
            break;
        }
      }

    }

  }

  // Executes the queued actions
  actionQueue.executeActions();
}